//
// Created by jonny on 2022/4/16.
//

#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H

#include "vec3.h"

class ray
{
public:
    ray(){}
    ray(const point3& org, const vec3& dir, const double _time):origin(org),direction(dir), time(_time){}

    vec3 get_origin() const {return origin;}
    vec3 get_direction() const {return direction;}

    point3 at(double t) const
    {
        return origin+ t*direction;
    }

    double get_time() const
    {
        return time;
    }

protected:
    vec3 origin;
    vec3 direction;
    double time;
};


#endif //RAYTRACING_RAY_H
