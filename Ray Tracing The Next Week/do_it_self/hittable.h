//
// Created by jonny on 2022/4/18.
//

#ifndef RAYTRACING_HITTABLE_H
#define RAYTRACING_HITTABLE_H

#include <memory>

#include "aabb.h"
#include "ray.h"
#include "utility.h"
class material;
struct hit_record
{
    point3 position;
    vec3 normal;
    double t;
    bool front_face;
    std::shared_ptr<material> mtr_ptr;
    inline void set_face_normal(const ray& r, const vec3& outward_normal)
    {
        front_face = dot(r.get_direction(), outward_normal) < 0;
        normal = front_face > 0 ? outward_normal : -outward_normal;
    }
};

class hittable
{
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(double time0, double time1, aabb& outbox) const = 0;
};

#endif //RAYTRACING_HITTABLE_H
