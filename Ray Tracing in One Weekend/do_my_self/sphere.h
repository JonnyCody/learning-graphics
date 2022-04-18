//
// Created by jonny on 2022/4/18.
//

#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include "hittable.h"

class sphere : public hittable
{
public:
    sphere(const vec3& cen, double rad) : center(cen), radius(rad) {}
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override
    {
        vec3 oc = r.get_origin() - center;
        auto a = r.get_direction().length_square();
        auto half_b = dot(oc, r.get_direction());
        auto c = oc.length_square() - radius*radius;

        auto discriminant = half_b*half_b - a*c;
        if(discriminant < 0)
        {
            return false;
        }
        auto sqrtd = sqrt(discriminant);

        auto root = (-half_b - sqrtd) / a;
        if(root < t_min || root > t_min)
        {
            root = (-half_b + sqrtd) / a;
            if(root < t_min || root > t_min)
            {
                return false;
            }
        }
        rec.t = root;
        rec.position = r.at(root);
        rec.normal = (rec.position - center) / radius;
        return true;
    }

private:
    point3 center;
    double radius;
};

#endif //RAYTRACING_SPHERE_H
