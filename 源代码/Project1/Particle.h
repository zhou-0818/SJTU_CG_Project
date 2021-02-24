#ifndef __opengl__Particle__
#define __opengl__Particle__

#include <gl/glut.h>

// 粒子结构 
struct Particle
{
    float x, y, z;            //粒子的位置 
    unsigned int  r, g, b;  // 粒子的颜色 
    float vx, vy, vz;        // 粒子的速度(x,y,z方向) 
    float ax, ay, az;        // 粒子在x，y，z上的加速度 
    float lifetime;         // 粒子生命值
    float size;             //粒子尺寸
    float dec;              //粒子消失的速度 
};


class CParticle
{
private:
    Particle* data;               // 粒子指针
    int         numparticle;        //粒子数目

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
