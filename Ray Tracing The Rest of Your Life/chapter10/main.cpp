//
// Created by jonny on 2022/4/16.
//

#include <chrono>

#include "aarect.h"
#include "box.h"
#include "bvh.h"
#include "camera.h"
#include "constant_medium.h"
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "moving_sphere.h"
#include "sphere.h"
#include "material.h"
#include "utility.h"

hittable_list cornell_box() {
    hittable_list objects;

    auto red   = std::make_shared<lambertian>(color(.65, .05, .05));
    auto white = std::make_shared<lambertian>(color(.73, .73, .73));
    auto green = std::make_shared<lambertian>(color(.12, .45, .15));
    auto light = std::make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<flip_face>(std::make_shared<xz_rect>(213, 343, 227, 332, 554, light)));
    objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    std::shared_ptr<hittable> box1 = std::make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = std::make_shared<rotate_y>(box1, 15);
    box1 = std::make_shared<translate>(vec3(265,0,295), box1);
    objects.add(box1);

    std::shared_ptr<hittable> box2 = std::make_shared<box>(point3(0,0,0), point3(165,165,165), white);
    box2 = std::make_shared<rotate_y>(box2, -18);
    box2 = std::make_shared<translate>( vec3(130,0,65), box2);
    objects.add(box2);

    return objects;
}

color ray_color(const ray& r, hittable_list& world, color background, int depth)
{
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return background;

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color attenuation;
    color emitted = rec.mtr_ptr->emitted(rec, rec.u, rec.v, rec.position);
    double pdf;
    color albedo;

    if (!rec.mtr_ptr->scatter(r, rec, albedo, scattered, pdf))
        return emitted;

    auto on_light = point3(random_double_number(213,343), 554, random_double_number(227,332));
    auto to_light = on_light - rec.position;
    auto distance_squared = to_light.length_square();
    to_light = unit_vector(to_light);

    if(dot(to_light, rec.normal) < 0)
    {
        return emitted;
    }

    double light_area = (343-213)*(332-227);
    auto light_cosine = fabs(to_light.y());
    if(light_cosine < 0.000001)
        return emitted;

    pdf = distance_squared / (light_cosine*light_area);
    scattered = ray(rec.position, to_light, r.get_time());

    return emitted
           + albedo * rec.mtr_ptr->scattering_pdf(r, rec, scattered)
             * ray_color(scattered, world, background, depth-1) / pdf;
}

color ray_color(const ray& r, bvh_node& nodes, const color& background, int depth)
{
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return background;

    // If the ray hits nothing, return the background color.
    if (!nodes.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color attenuation;
    color emitted = rec.mtr_ptr->emitted(rec, rec.u, rec.v, rec.position);
    double pdf;
    color albedo;

    if (!rec.mtr_ptr->scatter(r, rec, albedo, scattered, pdf))
        return emitted;

    auto on_light = point3(random_double_number(213,343), 554, random_double_number(227,332));
    auto to_light = on_light - rec.position;
    auto distance_squared = to_light.length_square();
    to_light = unit_vector(to_light);

    if(dot(to_light, rec.normal) < 0)
    {
        return emitted;
    }

    double light_area = (343-213)*(332-227);
    auto light_cosine = fabs(to_light.y());
    if(light_cosine < 0.000001)
        return emitted;

    pdf = distance_squared / (light_cosine*light_area);
    scattered = ray(rec.position, to_light, r.get_time());

    return emitted
           + albedo * rec.mtr_ptr->scattering_pdf(r, rec, scattered)
             * ray_color(scattered, nodes, background, depth-1) / pdf;
}

int main()
{
    // Image
    const auto aspect_ratio = 1.0 / 1.0;
    const int image_width = 600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;
    const int max_depth = 50;

    // World
    auto world = cornell_box();
    color background(0,0,0);

    // Camera
    point3 lookfrom(278, 278, -800);
    point3 lookat(278, 278, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 40.0;
    auto time0 = 0.0;
    auto time1 = 1.0;
    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, time0, time1);

    // create bvh node
    bvh_node nodes(world, 0.001, infinity);

    auto start = std::chrono::system_clock::now();

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    for(int j = image_height-1;j>=0;--j)
    {
        std::cerr << "\rScan lines remaining: " << j << ' ' << std::flush;
        for(int i = 0; i < image_width; ++i)
        {
            color pixel_color(0, 0, 0);
            for(int s = 0;s<samples_per_pixel;++s)
            {
                double u = double(i + random_double_number())/image_width;
                double v = double(j + random_double_number())/image_height;
                ray r = cam.get_ray(u,v);
                pixel_color += ray_color(r, world, background, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
    auto stop = std::chrono::system_clock::now();

    std::cerr << "Time taken: " << std::chrono::duration_cast<std::chrono::hours>(stop - start).count() << " hours\n";
    std::cerr << "          : " << std::chrono::duration_cast<std::chrono::minutes>(stop - start).count() << " minutes\n";
    std::cerr << "          : " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " seconds\n";
}
