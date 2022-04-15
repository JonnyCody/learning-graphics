//
// Created by jonny on 2022/4/15.
//

#ifndef METAL_MATERIAL_H
#define METAL_MATERIAL_H

#include "rtweekend.h"

struct hit_record;

class material
{
public:
    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
            ) const = 0;
};

#endif //METAL_MATERIAL_H
