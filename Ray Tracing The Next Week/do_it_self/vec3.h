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

    double operator[](int i) const{return e[i];};
    double& operator[](int i) {return e[i];}
    const vec3& operator+(){return *this;}
    vec3 operator-() const{return vec3(-x(),-y(),-z());}

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

    vec3& operator+=(const vec3& v)
    {
        e[0]+=v.x();
        e[1]+=v.y();
        e[2]+=v.z();
        return *this;
    }

    vec3& operator-=(const vec3& v)
    {
        e[0]-=v.x();
        e[1]-=v.y();
        e[2]-=v.z();
        return *this;
    }

    vec3& operator*=(const double t)
    {
        e[0]*=t;
        e[1]*=t;
        e[2]*=t;
        return *this;
    }

    vec3& operator/=(const double t)
    {
        e[0]*=(1.0/t);
        e[1]*=(1.0/t);
        e[2]*=(1.0/t);
        return *this;
    }

    double length_square()
    {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    double length()
    {
        return std::sqrt(length_square());
    }
    bool near_zero()
    {
        double zero = 1e-8;
        return (fabs(e[0]) < zero) && (fabs(e[1]) < zero) && (fabs(e[2]) < zero);
    }
private:
    double e[3];
};

inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline double dot(const vec3& u, const vec3& v)
{
    return u.x()*v.x() + u.y()*v.y() + u.z()*v.z();
}

inline vec3 cross(const vec3& u, const vec3& v)
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

inline vec3 operator*(const double t, const vec3& v)
{
    return vec3(t*v.x(),t*v.y(),t*v.z());
}

inline vec3 operator*(const vec3& v, const double t)
{
    return vec3(t*v.x(),t*v.y(),t*v.z());
}

inline vec3 operator/(const vec3& v, const double t)
{
    return (1/t) * v;
}

inline vec3 unit_vector(vec3 v)
{
    return v/v.length();
}

using point3 = vec3;

using color = vec3;


#endif //RAY_TRACING_VEC3_H
