//
// Created by jonny on 2022/4/19.
//

#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H

/*#include "utility.h"
#include "ray.h"*/
#include "hittable.h"
// struct hit_record;
class material
{
public:
    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
            ) const = 0;
};

class lambertian : public material
{
public:
    lambertian(const color& c) : albedo(c){}
    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override
    {
        vec3 scatter = rec.normal + random_unit_vec3();
        if(scatter.near_zero())
        {
            scatter = rec.normal;
        }
        scattered = ray(rec.position, scatter);
        attenuation = albedo;
        return true;
    }
private:
    color albedo;
};

class metal : public material
{
public:
    metal(const color& c) : albedo(c){}
    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override
    {
        auto reflected = reflect(unit_vector(r_in.get_direction()), rec.normal);
        scattered = ray(rec.position, reflected);
        attenuation = albedo;
        return dot(scattered.get_direction(), rec.normal) > 0;
    }
private:
    color albedo;
};


#endif //RAYTRACING_MATERIAL_H
