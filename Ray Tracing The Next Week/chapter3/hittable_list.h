//
// Created by jonny on 2022/4/18.
//

#ifndef RAYTRACING_HITTABLE_LIST_H
#define RAYTRACING_HITTABLE_LIST_H

#include <vector>
#include <memory>

#include "aabb.h"
#include "hittable.h"
class hittable_list : public hittable
{
public:
    hittable_list(){}
    void clear(){objects.clear();}
    void add(std::shared_ptr<hittable> object)
    {
        objects.push_back(object);
    }
    bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override
    {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = t_max;
        for(auto& object : objects)
        {
            if(object->hit(r, t_min,closest_so_far,temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }

    virtual bool bounding_box(
            double time0, double time1, aabb& output_box
            ) const override;

protected:
    std::vector<std::shared_ptr<hittable>> objects;
};

bool hittable_list::bounding_box(double time0, double time1, aabb &output_box) const
{
    if(objects.empty())
    {
        return false;
    }
    aabb temp_box;
    bool first_box = true;

    for(const auto& object : objects)
    {

    }

}

#endif //RAYTRACING_HITTABLE_LIST_H
