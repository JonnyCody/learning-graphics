//
// Created by jonny on 2022/4/16.
//

#ifndef RAYTRACING_UTILITY_H
#define RAYTRACING_UTILITY_H

#include <limits>
#include <random>

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degree_to_radian(double degrees)
{
    return pi*degrees/180.0;
}

inline double clamp(double x, double min, double max)
{
    if(x < min)
    {
        return min;
    }
    if(x > max)
    {
        return max;
    }
    return x;
}

inline double random_number()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_number(double min, double max)
{
    return min + (max - min) * random_number();
}

vec3 random_vec3()
{
    return vec3(random_number(), random_number(), random_number());
}

vec3 random_unit_vec3()
{
    auto v = random_vec3();
    return v/v.length();
}

point3 random_in_unit_sphere()
{
    while(true)
    {
        auto p = random_vec3();
        if(p.length_square() >= 1)
            continue;
        return p;
    }
}

point3 random_in_hemisphere(const vec3& normal)
{
    auto in_unit_sphere = random_in_unit_sphere();
    if(dot(in_unit_sphere, normal)>0)
    {
        return in_unit_sphere;
    }
    return -in_unit_sphere;
}

vec3 reflect(const vec3& v, const vec3& n)
{
    return v-2* dot(v,n)*n;
}

#endif //RAYTRACING_UTILITY_H
