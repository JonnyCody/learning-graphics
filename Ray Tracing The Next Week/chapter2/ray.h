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
    ray(const point3& org, const vec3 dir, double time = 0.0):origin(org),direction(dir), tm(time){}

    point3 get_origin() const {return origin;}
    vec3 get_direction() const {return direction;}
    double get_time() const {return tm;}

    point3 at(double t) const
    {
        return origin+ t*direction;
    }

protected:
    point3 origin;
    vec3 direction;
    double tm;
};


#endif //RAYTRACING_RAY_H
