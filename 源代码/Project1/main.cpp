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

//记录用户画了几个点来产生贝塞尔曲线
int POINTSUM = 0;



// 创建一个粒子系统
CParticle ParticleSystem;

//用来设置粒子的属性值
float x, y, z, vx, vy, vz, ax, ay, az, _size, lifetime, _dec;
int r, g, b;
float cx = 0, cy = 0, cz = 0;






// 初始化粒子系统
bool InitParticle()
{
	for (int i = 0; i < ParticleSystem.GetNumOfParticle(); ++i)
	{
		//初始化颜色
		//金属和木制分别对应了不同的碎屑颜色
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

		//初始化坐标
		x = cx;
		y = cy;
		z = (float)(rand() % 10) / 10.0;
		ParticleSystem.SetPosition(i, x, y, z);

		//初始化速度
		vx = 0.0001 * (rand() % 100);
		vy = 0.000002 * (rand() % 28000);
		vz = 0;
		ParticleSystem.SetVelocity(i, vx, vy, vz);

		//初始化加速度
		ax = 0;
		ay = 0;
		az = 0;
		ParticleSystem.SetAcceleration(i, ax, ay, az);

		//初始化生命周期
		lifetime = 5;
		ParticleSystem.SetLifeTime(i, lifetime);

		//消失速度
		_dec = 0.01 * (rand() % 50);
		ParticleSystem.SetDec(i, _dec);

		//初始化大小
		ParticleSystem.SetSize(i, 0.0015f);
	}
	return true;
}

// 更新粒子系统
void UpdateParticleSystem()
{
	// 更新位置
	x += vx;
	y += vy;

	// 更新速度
	vy += ay;

	// 更新生存时间 
	lifetime -= _dec;


	// 如果粒子消失或生命结束
	if (y >= 1 || x >= 1 || lifetime <= 0)
	{
		//初始化坐标
		x = cx;
		y = cy;
		z = (float)(rand() % 10) / 10.0;

		//初始化速度
		vx = 0.0001 * (rand() % 100);
		vy = 0.000002 * (rand() % 28000);
		vz = 0;

		//初始化加速度
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


	// 初始化粒子
	InitParticle();
}

// 绘制粒子
void DrawParticle()
{

	for (int i = 0; i < ParticleSystem.GetNumOfParticle(); ++i)
	{
		// 获得粒子的所有属性
		ParticleSystem.GetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, _size, lifetime, _dec);
		glColor4ub(r, g, b, 255);
		// 画出粒子
		glBegin(GL_POLYGON);
		glVertex3f(x - _size, y - _size * 3.75, z);
		glVertex3f(x - _size, y + _size * 3.75, z);
		glVertex3f(x + _size, y + _size * 3.75, z);
		glVertex3f(x + _size, y - _size * 3.75, z);
		glEnd();

		//更新粒子属性
		UpdateParticleSystem();
		ParticleSystem.SetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, _size, lifetime, _dec);
	}
	glutPostRedisplay();//发送渲染请求
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
	//判断线段是否平行
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
//用于存储点的集合,因为绘制的都是4个点的贝塞尔曲线，所以数组大小为4
static point bazier[4];



void ShengJin(double a, double b, double c, double d, vector<double>& X123)
{
	//盛金公式求三次方程的解
	double A = b * b - 3 * a * c;
	double B = b * c - 9 * a * d;
	double C = c * c - 3 * b * d;
	double f = B * B - 4 * A * C;
	double i_value;
	double Y1, Y2;
	if (fabs(A) < 1e-6 && fabs(B) < 1e-6)//公式1
	{
		X123.push_back(-b / (3 * a));
		X123.push_back(-b / (3 * a));
		X123.push_back(-b / (3 * a));
	}
	else if (fabs(f) < 1e-6)   //公式3
	{
		double K = B / A;
		X123.push_back(-b / a + K);
		X123.push_back(-K / 2);
		X123.push_back(-K / 2);
	}
	else if (f > 1e-6)      //公式2
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
	else if (f < -1e-6)   //公式4
	{
		double T = (2 * A * b - 3 * a * B) / (2 * A * sqrt(A));
		double S = acos(T);
		X123.push_back((-b - 2 * sqrt(A) * cos(S / 3)) / (3 * a));
		X123.push_back((-b + sqrt(A) * (cos(S / 3) + sqrt(3.0) * sin(S / 3))) / (3 * a));
		X123.push_back((-b + sqrt(A) * (cos(S / 3) - sqrt(3.0) * sin(S / 3))) / (3 * a));
	}
}



//计算某个点是否在三次贝塞尔曲线的绝对上方
bool isUpBazier(point p, point p0, point p1, point p2,point p3) {
	//首先根据x解出t
	double a = -p0.x + 3 * p1.x - 3 * p2.x + p3.x;
	double b = 3 * p0.x - 6 * p1.x + 3 * p2.x;
	double c = 3 * p1.x-3 * p0.x;
	double d = p0.x-p.x;

	vector<double> ts;

	ShengJin(a, b, c, d, ts);

	//首先去除[0,1]之外的t
	vector<double>::iterator it;
	for (it = ts.begin(); it != ts.end();)
	{
		 if (*it < 0  || *it > 1)
			 it = ts.erase(it);    //删除元素，返回值指向已删除元素的下一个位置    
		 else
		     ++it;    //指向下一个位置
	}

	//然后对于每个t，算出一个y，找到最大的y
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


//判断一个点是否在一条线的上方
bool upLine(point p, line l) {
	if (l.B * (l.A * p.x + l.B * p.y + l.C) > 0) {
		return true;
	}
	else {
		return false;
	}
}


//判断一个点是否在一条线上
bool inLine(point p, line l) {

	if (((p.x - l.p1.x) * (l.p2.y - l.p1.y) == (p.y - l.p1.y) * (l.p2.x - l.p1.x)) && ((p.x - l.p1.x)*(p.x - l.p2.x) <= 0)) {
		return true;
	}
	else return false;
}

void mymouse(int button, int state, int x, int y) {
	

	// 按下左键
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {

		//首先检查是否选择了使用贝塞尔曲线交互
		//贝塞尔曲线交互模式之下，只能进行点选构造贝塞尔曲线
		if (use_bz) {
			draw_knife = false;
			if (POINTSUM < 4) {
				bazier[POINTSUM].x = (x - ww/2) / (ww / 2);
				bazier[POINTSUM].y = (hh - y - hh/2) / (hh / 2);
				POINTSUM++;
			}
		}
		//在非贝塞尔曲线交互模式之下
		else {

			//更新刀尖坐标
			cx = (x - ww / 2) / (ww / 2);
			cy = (hh - y - (hh / 2)) / (hh / 2);


			//更新pre坐标
			pre.x = x;
			pre.y = hh - y;

			//一旦在合适范围内点击鼠标就会出现刀具
			//判断鼠标是否位于轮廓线上方
			for (int i = 0; i < points.size() - 1; i++) {
				line tmp = { points[i] ,points[i + 1] };
				//一旦鼠标位于某个线段下方，也不位于该条线段上，则说明鼠标位于其下方
				if (pre.x <= tmp.p2.x && pre.x >= tmp.p1.x && (upLine(pre, tmp) || inLine(pre, tmp))) {
					draw_knife = true;
					break;
				}
			}

			//一旦鼠标过于偏左或过于偏右，也不会出现刀具,或进刀过深
			if (pre.x <= 405|| pre.x >= 1130||  pre.y <= 210) {
				draw_knife = false;
			}

		}
	}
	//松开左键
	if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
		//一旦松开鼠标刀具就会消失,且粒子也会生命周期耗尽
		draw_knife = false;
		flag = false;
		for (int i = 0; i < ParticleSystem.GetNumOfParticle(); ++i) {
			ParticleSystem.SetLifeTime(i, 0);
		}
	}


}


void mymotion(int x, int y) {
	//检查是否在贝塞尔曲线模式，如果在，则其他所有行为都将被忽视
	if (use_bz) {
		return;
	}

	//获取当前鼠标的坐标，更新刀尖位置
	point now = { (float)x,hh - (float)y };
	cx = (now.x - ww / 2) / (ww / 2);
	cy = (now.y - hh / 2) / (hh / 2);


	//cout << now.x << "," << now.y << endl;


	//判断鼠标现在是否位于轮廓线下方
	bool underline = false;
	for (int i = 0; i < points.size() - 1; i++) {
		line tmp = { points[i] ,points[i + 1] };
		if (now.x <= tmp.p2.x && now.x >= tmp.p1.x && !upLine(now, tmp)) {
			underline = true;
			break;
		}
	}

	bool online = false;
	//判断鼠标前一刻是否位于轮廓线上
	for (int i = 0; i < points.size() - 1; i++) {
		line tmp = { points[i] ,points[i + 1] };
		//一旦鼠标位于某个线段下方，也不位于该条线段上，则说明鼠标位于其下方
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




	//判断当前鼠标是否碰到了画的贝塞尔曲线，如果碰到则无法行进
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

			//遍历现有的点序列，添加交点并删除掉某些点
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
				//先求交点
				//第一条线
				if (!flag1) {
					if (getIntersection(left, l, inter) && !(inter.x <= points[i].x || inter.x>=points[i + 1].x || inter.y <= down.y)) {
						points.insert(points.begin() + i + 1, inter);
						i++;
						flag1 = true;
					}
				}
				l = { points[i],points[i + 1] };
				//第二条线
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
			//说明没有交点
			if (!flag1 && !flag2) {
				pre = now;
				return;
			}
			for (int i = 0; i < points.size(); i++) {
				point p = points[i];
				//删除点
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
			//计算出刀片扫过的轮廓线（3条）
			point p1 = pre.x < now.x ? pre : now;
			point p2 = pre.x > now.x ? pre : now;
			line bottom = { p1,p2 };
			line left = { p1,point(p1.x + 1,p1.y - 1) };
			line right = { p2,point(p2.x + 1,p2.y + 1) };

			bool flag1 = false;
			bool flag2 = false;
			bool flag3 = false;
			//添加新的交点
			//左边
			for (int i = 0; i < points.size() - 1; i++) {
				//先求交点
				//第一条线
				line l = { points[i],points[i + 1] };
				point inter;
				if (!flag1 && getIntersection(left, l, inter) && !(inter.x <= points[i].x || inter.x>=points[i + 1].x || inter.y <= p1.y)) {
					points.insert(points.begin() + i + 1, inter);
					i++;
					flag1 = true;
				}
				l = { points[i],points[i + 1] };
				//第二条线
				if (!flag3 && getIntersection(bottom, l, inter) && !(inter.x <= points[i].x || inter.x>=points[i + 1].x || inter.x <= p1.x || inter.x >= p2.x)) {
					points.insert(points.begin() + i + 1, inter);
					flag2 = true;
					i++;
				}
				l = { points[i],points[i + 1] };
				//第三条线
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
			cout << "删除之前" << endl;
			for (int i = 0; i < points.size(); i++) {
				cout << "(" << points[i].x << "," << points[i].y << ")" << " ";
			}
			cout << endl;
			*/


			bool flag_p = false;
			for (int i = 0; i < points.size(); i++) {
				point p = points[i];
				//删除点
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
	//防止高度等于0，但宽度可以为0，因为h是除数
	if (h == 0) {
		h = 1;
	}
	float ratio = 1.0 * w / h;

	//把视口设置为整个窗口，0，0是视口左下角
	glViewport(0, 0, w, h);

	//设置投影矩阵
	glMatrixMode(GL_PROJECTION);
	//单位化投影矩阵
	glLoadIdentity();

	//设置正确的投影矩阵
	//gluPerspective(45, ratio, 2, 10);
	glOrtho(-1, 1, -1, 1, -5, 5);


	//设置模型视图矩阵
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
	a[2] = p2.z - p1.z;       //矢量a为p1p2

	b[0] = p3.x - p2.x;
	b[1] = p3.y - p2.y;
	b[2] = p3.z - p2.z;     //矢量b为p2p3


	//计算a,b两矢量的叉乘，得到法矢量n
	n[0] = a[1] * b[2] - a[2] * b[1];
	n[1] = a[2] * b[0] - a[0] * b[2];
	n[2] = a[0] * b[1] - a[1] * b[0];
	//将法矢量n单位化
	double length = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
	if (length != 1)
	{
		n[0] = n[0] / length;
		n[1] = n[1] / length;
		n[2] = n[2] / length;
	}
}


void drawCylinder() {

	//启用光照
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	if (name == "metal") {
		//金属
		GLfloat amb[] = { 0.292250, 0.292250, 0.292250, 1.000000 };
		GLfloat dif[] = { 0.807540, 0.807540, 0.807540, 1.000000 };
		GLfloat spec[] = { 4, 4, 4, 1.000000 };


		// 环境光
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
		// 漫反射光
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
		// 镜面反射光
		glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
		glMaterialf(GL_FRONT, GL_SHININESS,800);
	}
	if (name == "wood") {
		//木纹
		GLfloat amb[] = { 0.292250, 0.292250, 0.292250, 1.000000 };
		GLfloat dif[] = { 1,1,1, 1 };
		GLfloat spec[] = { 0.01,0.01, 0.01,1 };

		// 环境光
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
		// 漫反射光
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
		// 镜面反射光
		glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	}



	glEnable(GL_TEXTURE_2D);//执行纹理映射，启用二维文理
	glBindTexture(GL_TEXTURE_2D, ID);//允许建立一个绑定到目标纹理的有名称的纹理
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
	glPointSize(2.0); //设定点的大小为2*2像素的
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
	glEnable(GL_TEXTURE_2D);//执行纹理映射，启用二维文理
	glBindTexture(GL_TEXTURE_2D, kID);//允许建立一个绑定到目标纹理的有名称的纹理
	glBegin(GL_POLYGON);

	//左上
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(cx-0.01*1, cy+0.375*1,0.f);
	//右上
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(cx + 0.01*1, cy + 0.375*1, 0.f);
	//右下
	glTexCoord2f(1.0f, 0.1f);
	glVertex3f(cx + 0.01*1, cy + 0.01*3.75*1, 0.f);
	//刀尖
	glTexCoord2f(0.5f, 0.0f);
	glVertex3f(cx,cy, 0.0f);
	//左下
	glTexCoord2f(0.0f, 0.1f);
	glVertex3f(cx-0.01*1, cy+0.01*3.75*1, 0.f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


void drawSquare() {
	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);//执行纹理映射，启用二维文理
	glBindTexture(GL_TEXTURE_2D, bID);//允许建立一个绑定到目标纹理的有名称的纹理
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
	//按下大写E或者小写e则切换材质
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
	//按下w/W则进入/退出贝塞尔曲线模式，贝塞尔曲线模式下，鼠标不能控制刀具的移动，而是控制贝塞尔曲线的产生，最多只能有一条贝塞尔曲线存在
	//当退出贝塞尔曲线模式后，能够用鼠标控制刀具，但刀具不能超过贝塞尔曲线
	else if (key == 87|| key == 119) {
		use_bz = !use_bz;
	}
	//按下大写Q或者小写q则删除贝塞尔曲线，这个功能在贝塞尔曲线模式和非贝塞尔曲线模式下都可以使用
	else if (key == 81 || key == 113) {
		POINTSUM = 0;
	}
	//按下R键可以重置物料
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
	glutCreateWindow("模拟车床");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutMouseFunc(mymouse);
	glutMotionFunc(mymotion);
	glutKeyboardFunc(processNormalKeys);
	glutReshapeFunc(Reshape);
	glEnable(GL_DEPTH_TEST);
	//创建500个粒子
	ParticleSystem.Create(100);
	init();
	glutMainLoop();
	return 0;
}