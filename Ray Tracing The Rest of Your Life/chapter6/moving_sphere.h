//
// Created by jonny on 2022/4/26.
//

#ifndef RAYTRACING_MOVING_SPHERE_H
#define RAYTRACING_MOVING_SPHERE_H

#include "hittable.h"

class moving_sphere : public hittable
{
public:
    moving_sphere(const point3& _center0, const point3& _center1,
                  const double& _radius, const double& _time0,
                  const double& _time1, std::shared_ptr<material> m)
                  :center0(_center0), center1(_center1), radius(_radius),
                  time0(_time0), time1(_time1), mtr_ptr(m){}
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override
    {
        vec3 oc = r.get_origin() - center(r.get_time());
        auto a = r.get_direction().length_square();
        auto half_b = dot(oc, r.get_direction());
        auto c = oc.length_square() - radius*radius;

        auto discriminant = half_b*half_b - a*c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (root < t_min || t_max < root) {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || t_max < root)
                return false;
        }

        rec.t = root;
        rec.position = r.at(rec.t);
        auto outward_normal = (rec.position - center(r.get_time())) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mtr_ptr = mtr_ptr;

        return true;
    }

    virtual bool bounding_box(double time0, double time1, aabb& outbox) const override
    {
        aabb box0(center(time0) - point3(radius, radius, radius), center(time0) + point3(radius, radius, radius));
        aabb box1(center(time1) - point3(radius, radius, radius), center(time1) + point3(radius, radius, radius));
        outbox = surrounding_box(box0, box1);
        return true;
    }

    point3 center(double time) const
    {
        return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
    }
protected:
    point3 center0, center1;
    double radius;
    double time0, time1;
    std::shared_ptr<material> mtr_ptr;
};

#endif //RAYTRACING_MOVING_SPHERE_H
