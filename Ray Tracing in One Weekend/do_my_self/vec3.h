//
// Created by jonny on 2022/4/16.
//

#ifndef RAY_TRACING_VEC3_H
#define RAY_TRACING_VEC3_H

#include <iostream>
#include <cmath>

class vec3
{
public:
    vec3(){e[0]=e[1]=e[2]=0;}
    vec3(double x, double y, double z):e{x,y,z}{}
    double x() const {return e[0];}
    double y() const {return e[1];}
    double z() const {return e[2];}

    double operator[](int i)
    {
        if(i>=0&&i<3)
            return e[i];
        return 0;
    };

    vec3 operator-()
    {
        return vec3(-x(),-y(),-z());
    }

    vec3& operator+=(double t)
    {
        e[0]+=t;
        e[1]+=t;
        e[2]+=t;
        return *this;
    }

    vec3& operator-=(double t)
    {
        e[0]-=t;
        e[1]-=t;
        e[2]-=t;
        return *this;
    }

    vec3& operator+=(vec3 v)
    {
        e[0]+=v.x();
        e[1]+=v.y();
        e[2]+=v.z();
        return *this;
    }

    vec3& operator-=(vec3 v)
    {
        e[0]-=v.x();
        e[1]-=v.y();
        e[2]-=v.z();
        return *this;
    }

    vec3& operator*=(double t)
    {
        e[0]*=t;
        e[1]*=t;
        e[2]*=t;
        return *this;
    }

    vec3& operator/=(double t)
    {
        return *this*=1.0/t;
    }

    double length_squre()
    {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    double length()
    {
        return std::sqrt(length_squre());
    }

private:
    double e[3];
};

inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline double dot(vec3& u, vec3& v)
{
    return u.x()*v.x() + u.y()*v.y() + u.z()*v.z();
}

inline vec3 cross(vec3& u, vec3& v)
{
    return vec3(u.y()*v.z()-u.z()*v.y(),
                u.z()*v.x()-u.x()*v.z(),
                u.x()*v.y()-u.y()*v.x());
}

inline vec3 operator+(const vec3& u, const vec3& v)
{
    return vec3(u.x()+v.x(), u.y()+v.y(), u.z()+v.z());
}

inline vec3 operator-(const vec3& u, const vec3& v)
{
    return vec3(u.x()-v.x(),u.y()-v.y(),u.z()-v.z());
}

inline vec3 operator*(const vec3& u, const vec3& v)
{
    return vec3(u.x()*v.x(),u.y()*v.y(),u.z()*v.z());
}

inline vec3 operator*(double t, const vec3& v)
{
    return vec3(t*v.x(),t*v.y(),t*v.z());
}

inline vec3 operator/(vec3& v, double t)
{
    return vec3(v.x()/t, v.y()/t, v.z()/t);
}

inline vec3 unit_vector(vec3 v)
{
    return v/v.length();
}

using point3 = vec3;

using color = vec3;


#endif //RAY_TRACING_VEC3_H
