//
// Created by jonny on 2022/4/25.
//

#ifndef RAYTRACING_AABB_H
#define RAYTRACING_AABB_H


#include "utility.h"

class aabb
{
public:
    aabb(){}
    aabb(const point3& a, const point3& b) : minimum(a), maximum(b){}

    point3 min() const {return minimum;}
    point3 max() const {return maximum;}

    bool hit(const ray& r, double t_min, double t_max)const
    {
        for(int i = 0;i<3;++i)
        {
            auto t0 = fmin((minimum[i]-r.get_origin()[i])/r.get_direction()[i],
                           (maximum[i]-r.get_origin()[i])/r.get_direction()[i]);
            auto t1 = fmax((minimum[i]-r.get_origin()[i])/r.get_direction()[i],
                           (maximum[i]-r.get_origin()[i])/r.get_direction()[i]);
            t_min = fmax(t0, t_min);
            t_max = fmin(t1, t_max);
            if(t_min>=t_max)
            {
                return false;
            }
        }
        return true;
    }
protected:
    point3 minimum;
    point3 maximum;
};

aabb surrounding_box(aabb box1, aabb box2)
{
    aabb out_box(point3(fmin(box1.min()[0], box2.min()[0]),
                        fmin(box1.min()[1], box2.min()[1]),
                        fmin(box1.min()[2], box2.min()[2])),
                 point3(fmax(box1.max()[0], box2.max()[0]),
                        fmax(box1.max()[1], box2.max()[1]),
                        fmax(box1.max()[2], box2.max()[2])));
    return out_box;
}

#endif //RAYTRACING_AABB_H
