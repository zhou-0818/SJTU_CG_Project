#include<stdlib.h>
#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <Windows.h>

#include "texture.h"
#include "Particle.h"

#define  PI 3.1415
using namespace std;

bool flag = false;
float ww, hh;
bool use_bz = false;
bool draw_knife = false;

float angle = 0;

//��¼�û����˼���������������������
int POINTSUM = 0;



// ����һ������ϵͳ
CParticle ParticleSystem;

//�����������ӵ�����ֵ
float x, y, z, vx, vy, vz, ax, ay, az, _size, lifetime, _dec;
int r, g, b;
float cx = 0, cy = 0, cz = 0;






// ��ʼ������ϵͳ
bool InitParticle()
{
	for (int i = 0; i < ParticleSystem.GetNumOfParticle(); ++i)
	{
		//��ʼ����ɫ
		//������ľ�Ʒֱ��Ӧ�˲�ͬ����м��ɫ
		if (name == "metal") {
			r = 206;
			g = 206;
			b = 206;
		}
		else {
			r = 251;
			g = 232;
			b = 206;
		}
		ParticleSystem.SetColor(i, r, g, b);

		//��ʼ������
		x = cx;
		y = cy;
		z = (float)(rand() % 10) / 10.0;
		ParticleSystem.SetPosition(i, x, y, z);

		//��ʼ���ٶ�
		vx = 0.0001 * (rand() % 100);
		vy = 0.000002 * (rand() % 28000);
		vz = 0;
		ParticleSystem.SetVelocity(i, vx, vy, vz);

		//��ʼ�����ٶ�
		ax = 0;
		ay = 0;
		az = 0;
		ParticleSystem.SetAcceleration(i, ax, ay, az);

		//��ʼ����������
		lifetime = 5;
		ParticleSystem.SetLifeTime(i, lifetime);

		//��ʧ�ٶ�
		_dec = 0.01 * (rand() % 50);
		ParticleSystem.SetDec(i, _dec);

		//��ʼ����С
		ParticleSystem.SetSize(i, 0.0015f);
	}
	return true;
}

// ��������ϵͳ
void UpdateParticleSystem()
{
	// ����λ��
	x += vx;
	y += vy;

	// �����ٶ�
	vy += ay;

	// ��������ʱ�� 
	lifetime -= _dec;


	// ���������ʧ����������
	if (y >= 1 || x >= 1 || lifetime <= 0)
	{
		//��ʼ������
		x = cx;
		y = cy;
		z = (float)(rand() % 10) / 10.0;

		//��ʼ���ٶ�
		vx = 0.0001 * (rand() % 100);
		vy = 0.000002 * (rand() % 28000);
		vz = 0;

		//��ʼ�����ٶ�
		ax = 0.000005f;
		ay = 0.000005f;
		az = 0;


		lifetime = 5;
		_dec = 0.01 * (rand() % 50);
	}
}


void init()
{
	bID = LoadBitmap("background.bmp");
	ID = LoadBitmap((name + ".bmp").c_str());
	kID = LoadBitmap("razer.bmp");
	if (bID == -1 || ID == -1)
		exit(0);



	GLfloat ambient[] = { .3f,.3f,.3f,1.f };
	GLfloat diffuse[] = { 1.f,1.f,1.f,1.f };
	GLfloat specular[] = {1.f,1.f,1.f,1.f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	GLfloat position[] = { 0.5f,0.0f,1.f,0.f };
	glLightfv(GL_LIGHT0, GL_POSITION, position);


	// ��ʼ������
	InitParticle();
}

// ��������
void DrawParticle()
{

	for (int i = 0; i < ParticleSystem.GetNumOfParticle(); ++i)
	{
		// ������ӵ���������
		ParticleSystem.GetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, _size, lifetime, _dec);
		glColor4ub(r, g, b, 255);
		// ��������
		glBegin(GL_POLYGON);
		glVertex3f(x - _size, y - _size * 3.75, z);
		glVertex3f(x - _size, y + _size * 3.75, z);
		glVertex3f(x + _size, y + _size * 3.75, z);
		glVertex3f(x + _size, y - _size * 3.75, z);
		glEnd();

		//������������
		UpdateParticleSystem();
		ParticleSystem.SetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, _size, lifetime, _dec);
	}
	glutPostRedisplay();//������Ⱦ����
}

struct point {
	float x;
	float y;
	point() :x(), y() {}
	point(float x, float y) :x(x), y(y) {}
};

struct line {
	point p1;
	point p2;
	float A = 0;
	float B = 0;
	float C = 0;
	line() :p1(), p2() {}
	line(point p1, point p2) :p1(p1), p2(p2) {
		A = p2.y - p1.y;
		B = p1.x - p2.x;
		C = p2.x * p1.y - p1.x * p2.y;
	}
};


bool getIntersection(line m, line n, point& p) {
	//�ж��߶��Ƿ�ƽ��
	if (m.A * n.B == n.A * m.B) {
		return false;
	}
	float y = (m.C * n.A - n.C * m.A) / (m.A * n.B - n.A * m.B);
	float x = (n.C * m.B - m.C * n.B) / (m.A * n.B - n.A * m.B);
	p = point(x, y);
	return true;
}


point pre;
vector<point> points;
//���ڴ洢��ļ���,��Ϊ���ƵĶ���4����ı��������ߣ����������СΪ4
static point bazier[4];



void ShengJin(double a, double b, double c, double d, vector<double>& X123)
{
	//ʢ��ʽ�����η��̵Ľ�
	double A = b * b - 3 * a * c;
	double B = b * c - 9 * a * d;
	double C = c * c - 3 * b * d;
	double f = B * B - 4 * A * C;
	double i_value;
	double Y1, Y2;
	if (fabs(A) < 1e-6 && fabs(B) < 1e-6)//��ʽ1
	{
		X123.push_back(-b / (3 * a));
		X123.push_back(-b / (3 * a));
		X123.push_back(-b / (3 * a));
	}
	else if (fabs(f) < 1e-6)   //��ʽ3
	{
		double K = B / A;
		X123.push_back(-b / a + K);
		X123.push_back(-K / 2);
		X123.push_back(-K / 2);
	}
	else if (f > 1e-6)      //��ʽ2
	{
		Y1 = A * b + 3 * a * (-B + sqrt(f)) / 2;
		Y2 = A * b + 3 * a * (-B - sqrt(f)) / 2;
		double Y1_value = (Y1 / fabs(Y1)) * pow((double)fabs(Y1), 1.0 / 3);
		double Y2_value = (Y2 / fabs(Y2)) * pow((double)fabs(Y2), 1.0 / 3);
		X123.push_back((-b - Y1_value - Y2_value) / (3 * a));
		i_value = sqrt(3.0) / 2 * (Y1_value - Y2_value) / (3 * a);
		if (fabs(i_value) < 1e-1)
		{
			X123.push_back((-b + 0.5 * (Y1_value + Y2_value)) / (3 * a));
		}
	}
	else if (f < -1e-6)   //��ʽ4
	{
		double T = (2 * A * b - 3 * a * B) / (2 * A * sqrt(A));
		double S = acos(T);
		X123.push_back((-b - 2 * sqrt(A) * cos(S / 3)) / (3 * a));
		X123.push_back((-b + sqrt(A) * (cos(S / 3) + sqrt(3.0) * sin(S / 3))) / (3 * a));
		X123.push_back((-b + sqrt(A) * (cos(S / 3) - sqrt(3.0) * sin(S / 3))) / (3 * a));
	}
}



//����ĳ�����Ƿ������α��������ߵľ����Ϸ�
bool isUpBazier(point p, point p0, point p1, point p2,point p3) {
	//���ȸ���x���t
	double a = -p0.x + 3 * p1.x - 3 * p2.x + p3.x;
	double b = 3 * p0.x - 6 * p1.x + 3 * p2.x;
	double c = 3 * p1.x-3 * p0.x;
	double d = p0.x-p.x;

	vector<double> ts;

	ShengJin(a, b, c, d, ts);

	//����ȥ��[0,1]֮���t
	vector<double>::iterator it;
	for (it = ts.begin(); it != ts.end();)
	{
		 if (*it < 0  || *it > 1)
			 it = ts.erase(it);    //ɾ��Ԫ�أ�����ֵָ����ɾ��Ԫ�ص���һ��λ��    
		 else
		     ++it;    //ָ����һ��λ��
	}

	//Ȼ�����ÿ��t�����һ��y���ҵ�����y
	double y = -100;
	for (it = ts.begin(); it != ts.end();it++) {
		double t = *it;
		double a1 = pow((1 - t), 3);
		double a2 = pow((1 - t), 2) * 3 * t;
		double a3 = 3 * t * t * (1 - t);
		double a4 = t * t * t;
		double tmp = a1 * p0.y + a2 * p1.y + a3 * p2.y + a4 * p3.y;
		if (tmp > y) y = tmp;
	}

	if (y > p.y) {
		return false;
	}
	else {
		return true;
	}

}


//�ж�һ�����Ƿ���һ���ߵ��Ϸ�
bool upLine(point p, line l) {
	if (l.B * (l.A * p.x + l.B * p.y + l.C) > 0) {
		return true;
	}
	else {
		return false;
	}
}


//�ж�һ�����Ƿ���һ������
bool inLine(point p, line l) {

	if (((p.x - l.p1.x) * (l.p2.y - l.p1.y) == (p.y - l.p1.y) * (l.p2.x - l.p1.x)) && ((p.x - l.p1.x)*(p.x - l.p2.x) <= 0)) {
		return true;
	}
	else return false;
}

void mymouse(int button, int state, int x, int y) {
	

	// �������
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {

		//���ȼ���Ƿ�ѡ����ʹ�ñ��������߽���
		//���������߽���ģʽ֮�£�ֻ�ܽ��е�ѡ���챴��������
		if (use_bz) {
			draw_knife = false;
			if (POINTSUM < 4) {
				bazier[POINTSUM].x = (x - ww/2) / (ww / 2);
				bazier[POINTSUM].y = (hh - y - hh/2) / (hh / 2);
				POINTSUM++;
			}
		}
		//�ڷǱ��������߽���ģʽ֮��
		else {

			//���µ�������
			cx = (x - ww / 2) / (ww / 2);
			cy = (hh - y - (hh / 2)) / (hh / 2);


			//����pre����
			pre.x = x;
			pre.y = hh - y;

			//һ���ں��ʷ�Χ�ڵ�����ͻ���ֵ���
			//�ж�����Ƿ�λ���������Ϸ�
			for (int i = 0; i < points.size() - 1; i++) {
				line tmp = { points[i] ,points[i + 1] };
				//һ�����λ��ĳ���߶��·���Ҳ��λ�ڸ����߶��ϣ���˵�����λ�����·�
				if (pre.x <= tmp.p2.x && pre.x >= tmp.p1.x && (upLine(pre, tmp) || inLine(pre, tmp))) {
					draw_knife = true;
					break;
				}
			}

			//һ��������ƫ������ƫ�ң�Ҳ������ֵ���,���������
			if (pre.x <= 405|| pre.x >= 1130||  pre.y <= 210) {
				draw_knife = false;
			}

		}
	}
	//�ɿ����
	if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
		//һ���ɿ���굶�߾ͻ���ʧ,������Ҳ���������ںľ�
		draw_knife = false;
		flag = false;
		for (int i = 0; i < ParticleSystem.GetNumOfParticle(); ++i) {
			ParticleSystem.SetLifeTime(i, 0);
		}
	}


}


void mymotion(int x, int y) {
	//����Ƿ��ڱ���������ģʽ������ڣ�������������Ϊ����������
	if (use_bz) {
		return;
	}

	//��ȡ��ǰ�������꣬���µ���λ��
	point now = { (float)x,hh - (float)y };
	cx = (now.x - ww / 2) / (ww / 2);
	cy = (now.y - hh / 2) / (hh / 2);


	//cout << now.x << "," << now.y << endl;


	//�ж���������Ƿ�λ���������·�
	bool underline = false;
	for (int i = 0; i < points.size() - 1; i++) {
		line tmp = { points[i] ,points[i + 1] };
		if (now.x <= tmp.p2.x && now.x >= tmp.p1.x && !upLine(now, tmp)) {
			underline = true;
			break;
		}
	}

	bool online = false;
	//�ж����ǰһ���Ƿ�λ����������
	for (int i = 0; i < points.size() - 1; i++) {
		line tmp = { points[i] ,points[i + 1] };
		//һ�����λ��ĳ���߶��·���Ҳ��λ�ڸ����߶��ϣ���˵�����λ�����·�
		if (pre.x <= tmp.p2.x && pre.x >= tmp.p1.x && (upLine(pre, tmp)||inLine(pre,tmp))) {
			online = true;
			break;
		}
	}
	/*
	if (online== false) {
		cout << "ddd";
	}
	*/

	flag = underline && online && now.x > 405  && now.x < 1130 && now.y > 210;




	//�жϵ�ǰ����Ƿ������˻��ı��������ߣ�����������޷��н�
	if (POINTSUM == 4 && use_bz == false && flag) {
		point cur = { (now.x - ww/2) / (ww/2),(now.y -hh/2) / (hh/2) };
		if (isUpBazier(point((pre.x - ww / 2) / (ww / 2),(pre.y - hh / 2) / (hh / 2)), bazier[0], bazier[1], bazier[2], bazier[3]) &&  !isUpBazier(cur, bazier[0], bazier[1], bazier[2],bazier[3])) {
			POINT cp;
			GetCursorPos(&cp);
			SetCursorPos(cp.x - (now.x - pre.x) ,cp.y + (now.y - pre.y));
			return;
		}
	}


	draw_knife = online && now.x>405 && now.x < 1130 && now.y>210;

	
	

	if (flag) {

		if (fabs(now.x - pre.x) <= fabs(now.y - pre.y)) {

			//�������еĵ����У���ӽ��㲢ɾ����ĳЩ��
			bool flag1 = false;
			point p1;
			point p2;
			bool flag2 = false;
			point down = now.y < pre.y ? now : pre;
			line left = { point(down.x - 1,down.y + 1),down };
			line right = { point(down.x + 1,down.y + 1),down };
			for (int i = 0; i < points.size() - 1; i++) {
				line l = { points[i],points[i + 1] };
				point inter;
				//���󽻵�
				//��һ����
				if (!flag1) {
					if (getIntersection(left, l, inter) && !(inter.x <= points[i].x || inter.x>=points[i + 1].x || inter.y <= down.y)) {
						points.insert(points.begin() + i + 1, inter);
						i++;
						flag1 = true;
					}
				}
				l = { points[i],points[i + 1] };
				//�ڶ�����
				if (!flag2) {
					if (getIntersection(right, l, inter) && !(inter.x <= points[i].x || inter.x>=points[i + 1].x || inter.y <= down.y)) {
						points.insert(points.begin() + i + 1, inter);
						i++;
						flag2 = true;
					}
				}
				if (flag1 && flag2) {
					break;
				}

			}
			//˵��û�н���
			if (!flag1 && !flag2) {
				pre = now;
				return;
			}
			for (int i = 0; i < points.size(); i++) {
				point p = points[i];
				//ɾ����
				if (upLine(p, left) && upLine(p, right)) {
					points.erase(points.begin() + i);
					/*
					cout << "delete " << "(" << p.x << "," << p.y << ")" << endl;
					if (points[points.size()-2].y != 300) {
						cout << 0;
					}
					*/
					i--;
				}
			}
			for (int i = 0; i < points.size(); i++) {
				point p = points[i];
				if (now.x < p.x) {
					points.insert(points.begin() + i, now);
					break;
				}
			}
		}
		else {
			//�������Ƭɨ���������ߣ�3����
			point p1 = pre.x < now.x ? pre : now;
			point p2 = pre.x > now.x ? pre : now;
			line bottom = { p1,p2 };
			line left = { p1,point(p1.x + 1,p1.y - 1) };
			line right = { p2,point(p2.x + 1,p2.y + 1) };

			bool flag1 = false;
			bool flag2 = false;
			bool flag3 = false;
			//����µĽ���
			//���
			for (int i = 0; i < points.size() - 1; i++) {
				//���󽻵�
				//��һ����
				line l = { points[i],points[i + 1] };
				point inter;
				if (!flag1 && getIntersection(left, l, inter) && !(inter.x <= points[i].x || inter.x>=points[i + 1].x || inter.y <= p1.y)) {
					points.insert(points.begin() + i + 1, inter);
					i++;
					flag1 = true;
				}
				l = { points[i],points[i + 1] };
				//�ڶ�����
				if (!flag3 && getIntersection(bottom, l, inter) && !(inter.x <= points[i].x || inter.x>=points[i + 1].x || inter.x <= p1.x || inter.x >= p2.x)) {
					points.insert(points.begin() + i + 1, inter);
					flag2 = true;
					i++;
				}
				l = { points[i],points[i + 1] };
				//��������
				if (!flag3 && getIntersection(right, l, inter) && !(inter.x <= points[i].x || inter.x>=points[i + 1].x || inter.y <= p2.y)) {
					points.insert(points.begin() + i + 1, inter);
					i++;
					flag3 = true;
				}
				if (flag1 && flag3 && flag2) {
					break;
				}

			}
			/*
			cout << "ɾ��֮ǰ" << endl;
			for (int i = 0; i < points.size(); i++) {
				cout << "(" << points[i].x << "," << points[i].y << ")" << " ";
			}
			cout << endl;
			*/


			bool flag_p = false;
			for (int i = 0; i < points.size(); i++) {
				point p = points[i];
				//ɾ����
				if (upLine(p, left) && upLine(p, bottom) && upLine(p, right)) {
					points.erase(points.begin() + i);
					/*
					cout << "delete " << "(" << p.x << "," << p.y << ")" << endl;
					if (points[points.size()-2].y != 300) {
						cout << 0;
					}
					*/
					i--;
				}
			}
			for (int i = 0; i < points.size(); i++) {
				point p = points[i];
				if (now.x < p.x) {
					points.insert(points.begin() + i, now);
					break;
				}
			}
			/*
			for (int i = 0; i < points.size() - 1; i++) {
				if (points[i].x == points[i + 1].x) {
					cout << 22;
				}
			}
			*/

		}
		glutPostRedisplay();
	
	}

	pre = now;
}




struct pointRect {
	float x;
	float y;
	float z;
	pointRect() :x(), y(), z() {}
	pointRect(float x, float y, float z) :x(x), y(y), z(z) {}
};



pointRect changeCoord(point p, float fai) {
	pointRect _p = { p.x,p.y * cos(fai), p.y * sin(fai) };
	return _p;
}

vector<vector<pointRect>> matrix;



void Reshape(int w, int h) {
	//��ֹ�߶ȵ���0������ȿ���Ϊ0����Ϊh�ǳ���
	if (h == 0) {
		h = 1;
	}
	float ratio = 1.0 * w / h;

	//���ӿ�����Ϊ�������ڣ�0��0���ӿ����½�
	glViewport(0, 0, w, h);

	//����ͶӰ����
	glMatrixMode(GL_PROJECTION);
	//��λ��ͶӰ����
	glLoadIdentity();

	//������ȷ��ͶӰ����
	//gluPerspective(45, ratio, 2, 10);
	glOrtho(-1, 1, -1, 1, -5, 5);


	//����ģ����ͼ����
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

	ww = w;
	hh = h;
}



vector<vector<pointRect>> generateMatrix(vector<point> line) {
	vector<vector<pointRect>> matrix;
	for (float fai = 0; fai < 2 * PI; fai += 0.05) {
		vector<pointRect> next;
		for (int i = 0; i < line.size(); i++) {
			next.push_back(changeCoord(line[i], fai));
		}
		matrix.push_back(next);
	}
	return matrix;
}


vector<point> changeScale(vector<point> points) {
	for (int i = 0; i < points.size(); i++) {
		points[i].x = (points[i].x - ww/2) / (ww/2);
		points[i].y = (points[i].y - hh/2) / (hh/2);
	}
	return points;
}


void CalNormal(pointRect p1, pointRect p2, pointRect p3, float* n) {
	float a[3], b[3];
	a[0] = p2.x - p1.x;
	a[1] = p2.y - p1.y;
	a[2] = p2.z - p1.z;       //ʸ��aΪp1p2

	b[0] = p3.x - p2.x;
	b[1] = p3.y - p2.y;
	b[2] = p3.z - p2.z;     //ʸ��bΪp2p3


	//����a,b��ʸ���Ĳ�ˣ��õ���ʸ��n
	n[0] = a[1] * b[2] - a[2] * b[1];
	n[1] = a[2] * b[0] - a[0] * b[2];
	n[2] = a[0] * b[1] - a[1] * b[0];
	//����ʸ��n��λ��
	double length = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
	if (length != 1)
	{
		n[0] = n[0] / length;
		n[1] = n[1] / length;
		n[2] = n[2] / length;
	}
}


void drawCylinder() {

	//���ù���
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	if (name == "metal") {
		//����
		GLfloat amb[] = { 0.292250, 0.292250, 0.292250, 1.000000 };
		GLfloat dif[] = { 0.807540, 0.807540, 0.807540, 1.000000 };
		GLfloat spec[] = { 4, 4, 4, 1.000000 };


		// ������
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
		// �������
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
		// ���淴���
		glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
		glMaterialf(GL_FRONT, GL_SHININESS,800);
	}
	if (name == "wood") {
		//ľ��
		GLfloat amb[] = { 0.292250, 0.292250, 0.292250, 1.000000 };
		GLfloat dif[] = { 1,1,1, 1 };
		GLfloat spec[] = { 0.01,0.01, 0.01,1 };

		// ������
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
		// �������
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
		// ���淴���
		glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	}



	glEnable(GL_TEXTURE_2D);//ִ������ӳ�䣬���ö�ά����
	glBindTexture(GL_TEXTURE_2D, ID);//������һ���󶨵�Ŀ������������Ƶ�����
	matrix = generateMatrix(changeScale(points));
	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix[i].size() - 1; j++) {
			int _i = (i + 1) % matrix.size();
			glBegin(GL_POLYGON);

			float n[3];
			CalNormal( matrix[_i][j + 1], matrix[i][j + 1], matrix[i][j], n);
			glNormal3f((GLfloat)n[0], (GLfloat)n[1], (GLfloat)n[2]);
			glTexCoord2f(0, 1);
			glVertex3f(matrix[i][j].x, matrix[i][j].y, matrix[i][j].z);
			glTexCoord2f(1,1);
			glVertex3f(matrix[i][j + 1].x, matrix[i][j + 1].y, matrix[i][j + 1].z);
			glTexCoord2f(1,0);
			glVertex3f(matrix[_i][j + 1].x, matrix[_i][j + 1].y, matrix[_i][j + 1].z);
			glTexCoord2f(0,0);
			glVertex3f(matrix[_i][j].x, matrix[_i][j].y, matrix[_i][j].z);
			glEnd();
		}
	}
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
}



void drawPoints() {

	glColor3f(1.0, 0.0, 0.0);
	glPointSize(2.0); //�趨��Ĵ�СΪ2*2���ص�
	glBegin(GL_POINTS);
	for (int i = 0; i < POINTSUM; i++) {
		glVertex3f(bazier[i].x, bazier[i].y, 1.0f);
	}
	glEnd();

}



void drawLines() {
	if (POINTSUM  < 2) {
		return;
	}
	
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(2);
	for (int i = 0; i < POINTSUM - 1; i++) {
		if (i % 2 != 0) {
			continue;
		}
		glBegin(GL_LINES);
		glVertex3f(bazier[i].x, bazier[i].y, 1.0f);
		glVertex3f(bazier[i+1].x, bazier[i+1].y, 1.0f);
		glEnd();

	}

}


void drawBazier() {
	if (POINTSUM < 4) {
		return;
	}

	point p_pre = bazier[0];
	point p_cur;
	for (double t = 0.05; t <= 1.0; t += 0.05)
	{
		double a1 = pow((1 - t), 3);
		double a2 = pow((1 - t), 2) * 3 * t;
		double a3 = 3 * t * t * (1 - t);
		double a4 = t * t * t;
		p_cur.x = a1 * bazier[0].x + a2 * bazier[1].x + a3 * bazier[2].x+a4* bazier[3].x;
		p_cur.y = a1 * bazier[0].y + a2 * bazier[1].y + a3 * bazier[2].y + a4 * bazier[3].y;
		glColor3f(1.0, 0.0, 0.0);
		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex3f(p_pre.x, p_pre.y, 1.0f);
		glVertex3f(p_cur.x, p_cur.y, 1.0f);
		glEnd();
		p_pre = p_cur;
	}


}




void drawKinfe() {
	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);//ִ������ӳ�䣬���ö�ά����
	glBindTexture(GL_TEXTURE_2D, kID);//������һ���󶨵�Ŀ������������Ƶ�����
	glBegin(GL_POLYGON);

	//����
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(cx-0.01*1, cy+0.375*1,0.f);
	//����
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(cx + 0.01*1, cy + 0.375*1, 0.f);
	//����
	glTexCoord2f(1.0f, 0.1f);
	glVertex3f(cx + 0.01*1, cy + 0.01*3.75*1, 0.f);
	//����
	glTexCoord2f(0.5f, 0.0f);
	glVertex3f(cx,cy, 0.0f);
	//����
	glTexCoord2f(0.0f, 0.1f);
	glVertex3f(cx-0.01*1, cy+0.01*3.75*1, 0.f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


void drawSquare() {
	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);//ִ������ӳ�䣬���ö�ά����
	glBindTexture(GL_TEXTURE_2D, bID);//������һ���󶨵�Ŀ������������Ƶ�����
	glBegin(GL_POLYGON);
	glTexCoord2f(0.15f, 0.78f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.78f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.28f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.15f, 0.28f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(angle, 1.0, 0.0, 0.0);


	drawCylinder();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//glRotatef(0, 1.0, 0.0, 0.0);
	if (flag) {
		DrawParticle();
	}
	drawSquare();
	if (draw_knife) {
		drawKinfe();
	}
	glDisable(GL_TEXTURE_2D);
	if (use_bz) {
		drawPoints();
		drawLines();
	}

	drawBazier();
	glutSwapBuffers();
	angle+=1;
}


void processNormalKeys(unsigned char key, int x, int y)
{
	//���´�дE����Сдe���л�����
	if (key == 69 || key ==101) {
		if (name == "metal") {
			name = "wood";
			init();
			return;
		}
		if (name == "wood") {
			name = "metal";
			init();
			return;
		}
	}
	//����w/W�����/�˳�����������ģʽ������������ģʽ�£���겻�ܿ��Ƶ��ߵ��ƶ������ǿ��Ʊ��������ߵĲ��������ֻ����һ�����������ߴ���
	//���˳�����������ģʽ���ܹ��������Ƶ��ߣ������߲��ܳ�������������
	else if (key == 87|| key == 119) {
		use_bz = !use_bz;
	}
	//���´�дQ����Сдq��ɾ�����������ߣ���������ڱ���������ģʽ�ͷǱ���������ģʽ�¶�����ʹ��
	else if (key == 81 || key == 113) {
		POINTSUM = 0;
	}
	//����R��������������
	else if (key == 82 || key == 114) {
		points.clear();
		point p1 = { 305,300 };
		point p2 = { 1230,300 };
		points.push_back(p1);
		points.push_back(p2);;
	}
}



int main(int argc, char** argv)
{
	hh = 400;
	ww = 1500;
	glutInit(&argc, argv);
	point p1 = {305,300 };
	point p2 = { 1230,300 };
	points.push_back(p1);
	points.push_back(p2);;
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);





	glutInitWindowPosition(10, 10);
	glutInitWindowSize(ww, hh);
	glutCreateWindow("ģ�⳵��");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMouseFunc(mymouse);
	glutMotionFunc(mymotion);
	glutKeyboardFunc(processNormalKeys);
	glutReshapeFunc(Reshape);
	glEnable(GL_DEPTH_TEST);
	//����500������
	ParticleSystem.Create(100);
	init();
	glutMainLoop();
	return 0;
}