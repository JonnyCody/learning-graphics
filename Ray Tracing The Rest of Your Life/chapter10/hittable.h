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
    double u;
    double v;
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
    virtual double pdf_value(const point3& o, const vec3& v) const
    {
        return 0.0;
    }
    virtual vec3 random(const vec3& o) const {
        return vec3(1, 0, 0);
    }
};

class translate : public hittable
{
public:
    translate(const point3& m, std::shared_ptr<hittable> p) : offset(m), ptr(p){}
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override
    {
        ray moved_r(r.get_origin() - offset, r.get_direction(), r.get_time());
        if(!ptr->hit(moved_r, t_min, t_max, rec))
        {
            return false;
        }
        rec.position+=offset;
        rec.set_face_normal(moved_r, rec.normal);
        return true;
    }

    virtual bool bounding_box(double time0, double time1, aabb& outbox) const override
    {
        if(!ptr->bounding_box(time0, time1, outbox))
        {
            return false;
        }
        outbox = aabb(outbox.min() + offset, outbox.max() + offset);
        return true;
    }

protected:
    point3 offset;
    std::shared_ptr<hittable> ptr;
};

class rotate_y : public hittable
{
public:
    rotate_y(std::shared_ptr<hittable> p, double theta)
    {
        ptr = p;
        auto radians = degree_to_radian(theta);
        sin_theta = sin(radians);
        cos_theta = cos(radians);
        hasbox = ptr->bounding_box(0, 1, bbox);

        point3 min( infinity,  infinity,  infinity);
        point3 max(-infinity, -infinity, -infinity);

        for(int i=0;i<2;++i)
        {
            for(int j=0;j<2;++j)
            {
                for(int k=0;k<2;++k)
                {
                    auto x = i*bbox.max().x() + (1-i)*bbox.min().x();
                    auto y = j*bbox.max().y() + (1-j)*bbox.min().y();
                    auto z = k*bbox.max().z() + (1-k)*bbox.min().z();

                    auto newx =  cos_theta*x + sin_theta*z;
                    auto newz = -sin_theta*x + cos_theta*z;

                    vec3 tester(newx, y, newz);

                    for (int c = 0; c < 3; c++)
                    {
                        min[c] = fmin(min[c], tester[c]);
                        max[c] = fmax(max[c], tester[c]);
                    }
                }
            }
        }
        bbox = aabb(min, max);
    }
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override
    {
        auto origin = r.get_origin();
        auto direction = r.get_direction();

        origin[0] = cos_theta*r.get_origin()[0] - sin_theta*r.get_origin()[2];
        origin[2] = sin_theta*r.get_origin()[0] + cos_theta*r.get_origin()[2];

        direction[0] = cos_theta*r.get_direction()[0] - sin_theta*r.get_direction()[2];
        direction[2] = sin_theta*r.get_direction()[0] + cos_theta*r.get_direction()[2];

        ray rotated_r(origin, direction, r.get_time());

        if (!ptr->hit(rotated_r, t_min, t_max, rec))
            return false;

        auto p = rec.position;
        auto normal = rec.normal;

        p[0] =  cos_theta*rec.position[0] + sin_theta*rec.position[2];
        p[2] = -sin_theta*rec.position[0] + cos_theta*rec.position[2];

        normal[0] =  cos_theta*rec.normal[0] + sin_theta*rec.normal[2];
        normal[2] = -sin_theta*rec.normal[0] + cos_theta*rec.normal[2];

        rec.position = p;
        rec.set_face_normal(rotated_r, normal);

        return true;
    }
    virtual bool bounding_box(double time0, double time1, aabb& outbox) const override
    {
        outbox = bbox;
        return hasbox;
    }

protected:
    std::shared_ptr<hittable> ptr;
    double sin_theta;
    double cos_theta;
    bool hasbox;
    aabb bbox;
};

class flip_face : public hittable {
public:
    flip_face(std::shared_ptr<hittable> p) : ptr(p) {}

    virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override {

        if (!ptr->hit(r, t_min, t_max, rec))
            return false;

        rec.front_face = !rec.front_face;
        return true;
    }

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
        return ptr->bounding_box(time0, time1, output_box);
    }

public:
    std::shared_ptr<hittable> ptr;
};

#endif //RAYTRACING_HITTABLE_H
