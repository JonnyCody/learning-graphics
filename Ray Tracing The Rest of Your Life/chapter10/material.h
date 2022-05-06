//
// Created by jonny on 2022/4/19.
//

#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H

#include "hittable.h"
#include "onb.h"
#include "texture.h"
class material
{
public:
    virtual color emitted(const hit_record& rec, double u, double v, const point3& p) const
    {
        return color(0, 0, 0);
    };
    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, double& pdf) const
    {
        return false;
    }

    virtual double scattering_pdf(
            const ray& r_in, const hit_record& rec, const ray& scattered
            ) const
    {
        return 0;
    }

};

class lambertian : public material
{
public:
    lambertian(const color& a) : albedo(std::make_shared<solid_color>(a)) {}
    lambertian(std::shared_ptr<texture> a) : albedo(a) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& alb, ray& scattered, double& pdf
    ) const override {
        onb uvw;
        uvw.build_from_w(rec.normal);
        auto direction = uvw.local(random_cosine_direction());
        scattered = ray(rec.position, unit_vector(direction), r_in.get_time());
        alb = albedo->value(rec.u, rec.v, rec.position);
        pdf = dot(uvw.w(), scattered.get_direction()) / pi;
        return true;
    }

    double scattering_pdf(
            const ray& r_in, const hit_record& rec, const ray& scattered
    ) const {
        auto cosine = dot(rec.normal, unit_vector(scattered.get_direction()));
        return cosine < 0 ? 0 : cosine/pi;
    }

protected:
    std::shared_ptr<texture> albedo;
};

class metal : public material
{
public:
    metal(const color& c, double f) : albedo(c), fuzz(f){}
    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, double& pdf
    ) const override
    {
        auto reflected = reflect(unit_vector(r_in.get_direction()), rec.normal);
        scattered = ray(rec.position, reflected + fuzz * random_in_unit_sphere(), r_in.get_time());
        attenuation = albedo;
        return (dot(scattered.get_direction(), rec.normal) > 0);
    }
private:
    color albedo;
    double fuzz;
};

class dielectric : public material {
public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, double& pdf) const override
    {
        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

        vec3 unit_direction = unit_vector(r_in.get_direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double_number())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);

        scattered = ray(rec.position, direction, r_in.get_time());
        return true;
    }

public:
    double ir; // Index of Refraction
private:
    static double reflectance(double cosine, double ref_idx)
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }
};

class diffuse_light : public material
{
public:
    diffuse_light(std::shared_ptr<texture> a) : emit(a) {}
    diffuse_light(color c) : emit(std::make_shared<solid_color>(c)) {}
    virtual color emitted(const hit_record& rec, double u, double v, const point3& p) const override
    {
        if (rec.front_face)
            return emit->value(u, v, p);
        else
            return color(0,0,0);
    }

    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, double& pdf) const override
    {
        return false;
    }
protected:
    std::shared_ptr<texture> emit;
};

class isotropic : public material
{
public:
    isotropic(const color& c) : albedo(std::make_shared<solid_color>(c)){}
    isotropic(std::shared_ptr<texture> a) : albedo(a){}
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, double& pdf) const override
    {
        scattered = ray(rec.position, random_in_unit_sphere(), r_in.get_time());
        attenuation = albedo->value(rec.u, rec.v, rec.position);
        return true;
    }
protected:
    std::shared_ptr<texture> albedo;
};

#endif //RAYTRACING_MATERIAL_H
