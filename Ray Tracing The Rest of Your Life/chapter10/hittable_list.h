//
// Created by jonny on 2022/4/18.
//

#ifndef RAYTRACING_HITTABLE_LIST_H
#define RAYTRACING_HITTABLE_LIST_H

#include <vector>
#include <memory>

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
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const
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

    virtual bool bounding_box(double time0, double time1, aabb& box) const
    {
        if(objects.empty())
        {
            return false;
        }
        bool first_box = true;
        aabb temp_box;
        for(auto object : objects)
        {
            if(!object->bounding_box(time0, time1, temp_box))
            {
                return false;
            }
            box = first_box ? temp_box : surrounding_box(box, temp_box);
            first_box = false;
        }
        return true;
    }
    auto& get_objects() const
    {
        return objects;
    }

protected:
    std::vector<std::shared_ptr<hittable>> objects;
};

#endif //RAYTRACING_HITTABLE_LIST_H
