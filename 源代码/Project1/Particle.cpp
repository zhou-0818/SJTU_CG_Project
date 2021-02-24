#include "Particle.h"


CParticle::CParticle()
{
    data = NULL;
    numparticle = 0;

}

CParticle::~CParticle()
{
    delete[]data;
    data = NULL;
}

// 创建一个包含num个元素的粒子数组
int CParticle::Create(long num)
{

    if (data)
        delete[]data;

    if (data = new Particle[num])
    {
        memset(data, 0, sizeof(Particle) * numparticle);
        numparticle = num;


        return numparticle;
    }
    return 0;
}



int CParticle::SetColor(GLint r, GLint g, GLint b)
{
    for (int index = 0; index < numparticle; ++index)
    {
        data[index].r = r;
        data[index].g = g;
        data[index].b = b;
    }
    return true;
}

int CParticle::SetColor(GLint index, GLint r, GLint g, GLint b)
{
    if (index >= 0 && index < numparticle)
    {
        data[index].r = r;
        data[index].g = g;
        data[index].b = b;
        return true;
    }
    return false;
}

int CParticle::GetColor(GLint index, GLint& r, GLint& g, GLint& b)
{
    if (index >= 0 && index < numparticle)
    {
        r = data[index].r;
        g = data[index].g;
        b = data[index].b;
        return true;
    }
    return false;
}

int CParticle::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
    for (int index = 0; index < numparticle; ++index)
    {
        data[index].x = x;
        data[index].y = y;
        data[index].z = z;
    }
    return true;
}

int CParticle::SetPosition(GLint index, GLfloat x, GLfloat y, GLfloat z)
{
    if (index >= 0 && index < numparticle)
    {
        data[index].x = x;
        data[index].y = y;
        data[index].z = z;
        return true;
    }
    return false;
}

int CParticle::GetPosition(GLint index, GLfloat& x, GLfloat& y, GLfloat& z)
{
    if (index >= 0 && index < numparticle)
    {
        x = data[index].x;
        y = data[index].y;
        z = data[index].z;
        return true;
    }
    return false;
}

int CParticle::SetAcceleration(GLfloat ax, GLfloat ay, GLfloat az)
{
    for (int index = 0; index < numparticle; ++index)
    {
        data[index].ax = ax;
        data[index].ay = ay;
        data[index].az = az;
    }
    return true;
}

int CParticle::SetAcceleration(GLint index, GLfloat ax, GLfloat ay, GLfloat az)
{
    if (index >= 0 && index < numparticle)
    {
        data[index].ax = ax;
        data[index].ay = ay;
        data[index].az = az;
        return true;
    }
    return false;
}


int CParticle::GetAcceletation(GLint index, GLfloat& ax, GLfloat& ay, GLfloat& az)
{
    if (index >= 0 && index < numparticle)
    {
        ax = data[index].ax;
        ay = data[index].ay;
        az = data[index].az;
        return true;
    }
    return false;
}




int CParticle::SetVelocity(GLfloat vx, GLfloat vy, GLfloat vz)
{
    for (int index = 0; index < numparticle; ++index)
    {
        data[index].vx = vx;
        data[index].vy = vy;
        data[index].vz = vz;
    }
    return true;
}

int CParticle::SetVelocity(GLint index, GLfloat vx, GLfloat vy, GLfloat vz)
{
    if (index >= 0 && index < numparticle)
    {
        data[index].vx = vx;
        data[index].vy = vy;
        data[index].vz = vz;
        return true;
    }
    return false;
}

int CParticle::GetVelocity(GLint index, GLfloat& vx, GLfloat& vy, GLfloat& vz)
{
    if (index >= 0 && index < numparticle)
    {
        vx = data[index].vx;
        vy = data[index].vy;
        vz = data[index].vz;
        return true;
    }
    return false;
}



int CParticle::SetSize(GLfloat size)
{
    for (int index = 0; index < numparticle; ++index)
    {
        data[index].size = size;
    }
    return true;
}

int CParticle::SetSize(GLint index, GLfloat size)
{
    if (index >= 0 && index < numparticle)
    {
        data[index].size = size;
        return true;
    }
    return false;
}

int CParticle::GetSize(GLint index, GLfloat& size)
{
    if (index >= 0 && index < numparticle)
    {
        size = data[index].size;
        return true;
    }
    return false;
}

int CParticle::SetDec(GLfloat dec)
{
    for (int index = 0; index < numparticle; ++index)
    {
        data[index].dec = dec;
    }
    return true;
}

int CParticle::SetDec(GLint index, GLfloat dec)
{
    if (index >= 0 && index < numparticle)
    {
        data[index].dec = dec;
        return true;
    }
    return false;
}

int CParticle::GetDec(GLint index, GLfloat& dec)
{
    if (index >= 0 && index < numparticle)
    {
        dec = data[index].dec;
        return true;
    }
    return false;
}

int CParticle::SetLifeTime(GLfloat lifetime)
{
    for (int index = 0; index < numparticle; ++index)
    {
        data[index].lifetime = lifetime;
    }
    return true;
}

int CParticle::SetLifeTime(GLint index, GLfloat lifetime)
{
    if (index >= 0 && index < numparticle)
    {
        data[index].lifetime = lifetime;
        return true;
    }
    return false;
}


int CParticle::GetLifeTime(GLint index, GLfloat& lifetime)
{
    if (index >= 0 && index < numparticle)
    {
        lifetime = data[index].lifetime;
        return true;
    }
    return false;
}

// 获取粒子的所有属性 
int CParticle::GetAll(int index, GLint& r, GLint& g, GLint& b,         // 粒子的颜色 
    GLfloat& x, GLfloat& y, GLfloat& z,     // 位置 
    GLfloat& vx, GLfloat& vy, GLfloat& vz,  // 速度 
    GLfloat& ax, GLfloat& ay, GLfloat& az,  // 加速度 
    GLfloat& size,                            // 大小 
    GLfloat& lifetime,                        // 生命时间 
    GLfloat& dec                          // 消失速度 
)
{
    if (index >= 0 && index < numparticle)
    {
        r = data[index].r;
        g = data[index].g;
        b = data[index].b;
        x = data[index].x;
        y = data[index].y;
        z = data[index].z;
        vx = data[index].vx;
        vy = data[index].vy;
        vz = data[index].vz;
        ax = data[index].ax;
        ay = data[index].ay;
        az = data[index].az;
        lifetime = data[index].lifetime;
        size = data[index].size;
        dec = data[index].dec;
        return true;
    }
    return false;
}

//设置粒子的所有属性
int CParticle::SetAll(int index, GLint r, GLint g, GLint b,        // 粒子的颜色
    GLfloat x, GLfloat y, GLfloat z,        // 位置
    GLfloat vx, GLfloat vy, GLfloat vz, // 速度
    GLfloat ax, GLfloat ay, GLfloat az, //加速度
    GLfloat size,                     //大小
    GLfloat lifetime,                 //生命时间
    GLfloat dec                           // 消失速度
)
{
    if (index >= 0 && index < numparticle)
    {
        data[index].r = r;
        data[index].g = g;
        data[index].b = b;
        data[index].x = x;
        data[index].y = y;
        data[index].z = z;
        data[index].vx = vx;
        data[index].vy = vy;
        data[index].vz = vz;
        data[index].ax = ax;
        data[index].ay = ay;
        data[index].az = az;
        data[index].lifetime = lifetime;
        data[index].size = size;
        data[index].dec = dec;
        return true;
    }
    return false;
}