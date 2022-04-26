//
// Created by jonny on 2022/4/18.
//

#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include "hittable.h"

class sphere : public hittable
{
public:
    sphere(const vec3 cen, double rad, std::shared_ptr<material> m) : center(cen), radius(rad), mtr_ptr(m) {}
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
        if(root < t_min || root > t_max)
        {
            root = (-half_b + sqrtd) / a;
            if(root < t_min || root > t_max)
            {
                return false;
            }
        }
        rec.t = root;
        rec.position = r.at(root);
        vec3 outward_norm = (rec.position - center) / radius;
        rec.set_face_normal(r, outward_norm);
        get_sphere_uv(outward_norm, rec.u, rec.v);
        rec.mtr_ptr = mtr_ptr;
        return true;
    }

    virtual bool bounding_box(double time0, double time1, aabb& outbox) const override
    {
        outbox = aabb(center - vec3(radius, radius, radius),
                      center + vec3(radius, radius, radius));
        return true;
    }

private:
    static void get_sphere_uv(const point3& p, double& u, double& v) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi;

        u = phi / (2*pi);
        v = theta / pi;
    }

protected:
    point3 center;
    double radius;
    std::shared_ptr<material> mtr_ptr;
};

#endif //RAYTRACING_SPHERE_H
