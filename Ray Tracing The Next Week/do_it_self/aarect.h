//
// Created by jonny on 2022/4/27.
//

#ifndef RAYTRACING_AARECT_H
#define RAYTRACING_AARECT_H

#include "hittable.h"
class xy_rect : public hittable
{
public:
    xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, std::shared_ptr<material> m)
    : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mtr_ptr(m){}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override
    {
        double t = (k - r.get_origin().z()) / r.get_direction().z();
        if(t < t_min || t > t_max)
        {
            return false;
        }
        auto x = r.get_origin().x() + t*r.get_direction().x();
        auto y = r.get_origin().y() + t*r.get_direction().y();
        if(x < x0 || x > x1 || y < y0 || y > y1)
        {
            return false;
        }

        rec.t = t;
        rec.u = (x - x0) / (x1 - x0);
        rec.v = (y - y0) / (y1 - y0);
        auto outward_norm = vec3(0, 0, 1);
        rec.set_face_normal(r, outward_norm);
        rec.position = r.at(t);
        rec.mtr_ptr = mtr_ptr;
        return true;
    }
    virtual bool bounding_box(double time0, double time1, aabb& outbox) const override
    {
        outbox = aabb(point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001));
        return true;
    }

protected:
    double x0, x1, y0, y1;
    double k;
    std::shared_ptr<material> mtr_ptr;
};

class xz_rect : public hittable
{
public:
    xz_rect(double _x0, double _x1, double _z0, double _z1, double _k, std::shared_ptr<material> m)
            : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mtr_ptr(m){}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override
    {
        double t = (k - r.get_origin().y()) / r.get_direction().y();
        if(t < t_min || t > t_max)
        {
            return false;
        }
        auto x = r.get_origin().x() + t*r.get_direction().x();
        auto z = r.get_origin().z() + t*r.get_direction().z();
        if(x < x0 || x > x1 || z < z0 || z > z1)
        {
            return false;
        }

        rec.t = t;
        rec.u = (x - x0) / (x1 - x0);
        rec.v = (z - z0) / (z1 - z0);
        auto outward_norm = vec3(0, 1, 0);
        rec.set_face_normal(r, outward_norm);
        rec.position = r.at(t);
        rec.mtr_ptr = mtr_ptr;
        return true;
    }
    virtual bool bounding_box(double time0, double time1, aabb& outbox) const override
    {
        outbox = aabb(point3(x0, k- 0.0001, z0), point3(x1, k + 0.0001, z1));
        return true;
    }

protected:
    double x0, x1, z0, z1;
    double k;
    std::shared_ptr<material> mtr_ptr;
};

class yz_rect : public hittable
{
public:
    yz_rect(double _y0, double _y1, double _z0, double _z1, double _k, std::shared_ptr<material> m)
            : y0(_y0), y1(_y1), z0(_y0), z1(_z1), k(_k), mtr_ptr(m){}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override
    {
        double t = (k - r.get_origin().x()) / r.get_direction().x();
        if(t < t_min || t > t_max)
        {
            return false;
        }
        auto y = r.get_origin().y() + t*r.get_direction().y();
        auto z = r.get_origin().z() + t*r.get_direction().z();
        if(y < y0 || y > y1 || z < z0 || z > z1)
        {
            return false;
        }

        rec.t = t;
        rec.u = (y - y0) / (y1 - y0);
        rec.v = (z - z0) / (z1 - z0);
        auto outward_norm = vec3(1, 0, 0);
        rec.set_face_normal(r, outward_norm);
        rec.position = r.at(t);
        rec.mtr_ptr = mtr_ptr;
        return true;
    }
    virtual bool bounding_box(double time0, double time1, aabb& outbox) const override
    {
        outbox = aabb(point3(k-0.0001, y0, z0), point3(k + 0.0001, y1, z1));
        return true;
    }

protected:
    double y0, y1, z0, z1;
    double k;
    std::shared_ptr<material> mtr_ptr;
};


#endif //RAYTRACING_AARECT_H
