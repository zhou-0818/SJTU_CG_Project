#ifndef __opengl__Particle__
#define __opengl__Particle__

#include <gl/glut.h>

// ���ӽṹ 
struct Particle
{
    float x, y, z;            //���ӵ�λ�� 
    unsigned int  r, g, b;  // ���ӵ���ɫ 
    float vx, vy, vz;        // ���ӵ��ٶ�(x,y,z����) 
    float ax, ay, az;        // ������x��y��z�ϵļ��ٶ� 
    float lifetime;         // ��������ֵ
    float size;             //���ӳߴ�
    float dec;              //������ʧ���ٶ� 
};


class CParticle
{
private:
    Particle* data;               // ����ָ��
    int         numparticle;        //������Ŀ

public:
    CParticle();        
    ~CParticle();        

    
    int Create(long num);


    int SetColor(GLint r, GLint g, GLint b);
    int SetColor(GLint index, GLint r, GLint g, GLint b);
    int GetColor(GLint index, GLint& r, GLint& g, GLint& b);

  
    int SetVelocity(GLfloat vx, GLfloat vy, GLfloat vz);
    int SetVelocity(GLint index, GLfloat vx, GLfloat vy, GLfloat vz);
    int GetVelocity(GLint index, GLfloat& vx, GLfloat& vy, GLfloat& vz);

 
    int SetPosition(GLfloat x, GLfloat y, GLfloat z);
    int SetPosition(GLint index, GLfloat x, GLfloat y, GLfloat z);
    int GetPosition(GLint index, GLfloat& x, GLfloat& y, GLfloat& z);

    int SetAcceleration(GLfloat ax, GLfloat ay, GLfloat az);
    int SetAcceleration(GLint index, GLfloat ax, GLfloat ay, GLfloat az);
    int GetAcceletation(GLint index, GLfloat& ax, GLfloat& ay, GLfloat& az);


    int SetSize(GLfloat size);
    int SetSize(GLint index, GLfloat size);
    int GetSize(GLint index, GLfloat& size);

  
    int SetDec(GLfloat dec);
    int SetDec(GLint index, GLfloat dec);
    int GetDec(GLint index, GLfloat& dec);

    
    int SetLifeTime(GLfloat lifetime);
    int SetLifeTime(GLint index, GLfloat lifetime);
    int GetLifeTime(GLint index, GLfloat& lifetime);

  
    Particle* GetParticle() { return data; }

    
    int GetNumOfParticle() { return numparticle; }

   
    int GetAll(int index,                              
        GLint& r, GLint& g, GLint& b,              
        GLfloat& x, GLfloat& y, GLfloat& z,        
        GLfloat& vx, GLfloat& vy, GLfloat& vz,
        GLfloat& ax, GLfloat& ay, GLfloat& az,
        GLfloat& size,                           
        GLfloat& lifetime,                       
        GLfloat& dec                        
    );

  
    int SetAll(int index,                          
        GLint r, GLint g, GLint b,        
        GLfloat x, GLfloat y, GLfloat z,       
        GLfloat vx, GLfloat vy, GLfloat vz,    
        GLfloat ax, GLfloat ay, GLfloat az,   
        GLfloat size,                        
        GLfloat lifetime,                    
        GLfloat dec                         
    );
};

#endif
