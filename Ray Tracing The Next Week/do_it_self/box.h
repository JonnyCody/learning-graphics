//
// Created by jonny on 2022/4/28.
//

#ifndef RAYTRACING_BOX_H
#define RAYTRACING_BOX_H

#include "aarect.h"
#include "hittable_list.h"
class box : public hittable
{
public:
    box(){}
    box(const point3& p0, const point3& p1, std::shared_ptr<material> mt)
    {
        point0 = p0;
        point1 = p1;
        auto up = std::make_shared<xy_rect>(point0.x(), point1.x(), point0.y(), point1.y(), point0.z(), mt);
        auto down = std::make_shared<xy_rect>(point0.x(), point1.x(), point0.y(), point1.y(), point1.z(), mt);
        auto front = std::make_shared<yz_rect>(point0.y(), point1.y(),point0.z(), point1.z(), point0.x(),mt);
        auto back = std::make_shared<yz_rect>(point0.y(), point1.y(), point0.z(),point1.z(), point1.x(),mt);
        auto left = std::make_shared<xz_rect>(point0.x(), point1.x(), point0.z(), point1.z(), point0.y(), mt);
        auto right = std::make_shared<xz_rect>(point0.x(), point1.x(), point0.z(),point1.z(), point1.y(), mt);
        sides.add(up);
        sides.add(down);
        sides.add(front);
        sides.add(back);
        sides.add(left);
        sides.add(right);
    }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override
    {
        return sides.hit(r, t_min, t_max, rec);
    }

    virtual bool bounding_box(double time0, double time1, aabb& outbox) const override
    {
        outbox = aabb(point0, point1);
        return true;
    }

protected:
    vec3 point0,point1;
    hittable_list sides;
};

#endif //RAYTRACING_BOX_H
