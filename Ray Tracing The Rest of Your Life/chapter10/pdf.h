//
// Created by jonny on 2022/5/7.
//

#ifndef RAYTRACING_PDF_H
#define RAYTRACING_PDF_H

#include "vec3.h"
#include "utility.h"
#include "onb.h"
class pdf
{
public:
    virtual ~pdf(){}

    virtual double value(const vec3& direction) const = 0;
    virtual vec3 generate() const = 0;
};

class cosine_pdf : public pdf
{
public:
    cosine_pdf(const vec3& w) { uvw.build_from_w(w); }
    virtual double value(const vec3& direction) const override {
        auto cosine = dot(unit_vector(direction), uvw.w());
        return (cosine <= 0) ? 0 : cosine/pi;
    }
    virtual vec3 generate() const override {
        return uvw.local(random_cosine_direction());
    }
protected:
    onb uvw;
};

class hittable_pdf : public pdf
{
public:
    hittable_pdf(std::shared_ptr<hittable> p, const point3& origin) : ptr(p), o(origin) {}
    virtual double value(const vec3& direction) const override
    {
        return ptr->pdf_value(o, direction);
    }

    virtual vec3 generate() const override
    {
        return ptr->random(o);
    }
protected:
    point3 o;
    std::shared_ptr<hittable> ptr;
};

#endif //RAYTRACING_PDF_H
