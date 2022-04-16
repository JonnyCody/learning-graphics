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
    ray(const point3& origin, const vec3 direction):org(origin),dir(direction){}

    point3 origin(){return org;}
    vec3 direction(){return dir;}

    point3 at(double t)
    {
        return org + t*dir;
    }

private:
    point3 org;
    vec3 dir;
};


#endif //RAYTRACING_RAY_H
