//
// Created by jonny on 2022/4/16.
//
#include "camera.h"
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

color ray_color(const ray& r, hittable_list& world, hit_record& rec, int depth)
{
    if(depth <= 0)
    {
        return color(1, 0, 0);
    }
    if (world.hit(r, 0, infinity, rec))
    {
        ray scatter;
        color attenuation;
        if(rec.mtr_ptr->scatter(r, rec, attenuation, scatter))
        {
            return attenuation * ray_color(scatter, world, rec, depth - 1);
        }
        return color(0, 0, 0);
        /*auto target = rec.position + rec.normal + random_in_unit_sphere();
        return 0.5 * ray_color(ray(rec.position, reflect(r.get_direction(), rec.normal)), world, rec, depth-1);*/
        // return 0.5 * ray_color(ray(rec.position, target - rec.position), world, rec, depth-1);
    }
    vec3 unit_direction = unit_vector(r.get_direction());
    auto t= 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main()
{
    // Image
    double aspect_ratio = 16.0/9.0;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // Camera
    camera cam;

    //world
    hittable_list world;
    hit_record rec;
    auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto material_left = std::make_shared<metal>(color(0.8, 0.8, 0.8));
    auto material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2));

    world.add(std::make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100, material_ground));
    world.add(std::make_shared<sphere>(point3( 0.0, 0.0,    -1.0), 0.5, material_center));
    world.add(std::make_shared<sphere>(point3(-1.0, 0.0,    -1.0), 0.5, material_left));
    world.add(std::make_shared<sphere>(point3( 1.0, 0.0,    -1.0), 0.5, material_right));

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
                double u = double(i + random_number())/image_width;
                double v = double(j + random_number())/image_height;
                ray r(cam.get_origin(), cam.get_low_left_corner()+u*cam.get_horizontal() + v*cam.get_vertical());
                pixel_color += ray_color(r, world, rec, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
}
