//
// Created by jonny on 2022/4/21.
//

#ifndef RAYTRACING_MOVING_SPHERE_H
#define RAYTRACING_MOVING_SPHERE_H

#include "utility.h"
#include "hittable.h"

class moving_sphere : public hittable
{
public:
    moving_sphere(){}
    moving_sphere(
            point3 cen0, point3 cen1, double _time0, double _time1, double r, std::shared_ptr<material> m
            )
            : center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mtr_ptr(m){};
    virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec
            ) const override;
    virtual bool bounding_box(
            double _time0, double _time1, aabb& output_box
            ) const override;

    point3 center(double time) const;
protected:
    point3 center0, center1;
    double time0, time1;
    double radius;
    std::shared_ptr<material> mtr_ptr;
};

point3 moving_sphere::center(double time) const
{
    return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}

bool moving_sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    vec3 oc = r.get_origin() - center(r.get_time());
    auto a = r.get_direction().length_square();
    auto half_b = dot(oc, r.get_direction());
    auto c = oc.length_square() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if(discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range
    auto root = (-half_b - sqrtd) / a;
    if(root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a;
        if(root < t_min || t_max < root)
        {
            return false;
        }
    }

    rec.t = root;
    rec.position = r.at(rec.t);
    auto outward_normal = (rec.position - center(r.get_time())) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mtr_ptr = mtr_ptr;

    return true;
}

bool moving_sphere::bounding_box(double _time0, double _time1, aabb &output_box) const
{
    aabb box0(center(_time0) - vec3(radius, radius, radius),
              center(_time0) + vec3(radius, radius, radius));

    aabb box1(center(_time1) - vec3(radius, radius, radius),
              center(_time1) + vec3(radius, radius, radius));

    output_box = surrounding_box(box0, box1);
    return true;
}

#endif //RAYTRACING_MOVING_SPHERE_H
