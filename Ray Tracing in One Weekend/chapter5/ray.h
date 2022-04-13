//
// Created by jonny on 2022/4/12.
//

#ifndef CHAPTER4_RAY_H
#define CHAPTER4_RAY_H

#include "vec3.h"

class ray
{
public:
    ray(){}
    ray(const point3& origin, const vec3& direction)
    :orig(origin), dir(direction)
    {}

    point3 origin() const {return orig;}
    vec3 direction() const {return dir;}

    point3 at(double t) const
    {
        return orig + t*dir;
    }

public:
    point3 orig;
    vec3 dir;
};

#endif //CHAPTER4_RAY_H
