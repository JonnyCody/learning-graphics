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

double hit_sphere(const point3& center, double radius, const ray& r)
{
    vec3 oc = r.get_origin() - center;
    auto a = r.get_direction().length_square();
    auto half_b = dot(oc, r.get_direction());
    auto c = oc.length_square() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if(discriminant < 0)
    {
        return -1.0;
    }
    auto sqrtd = sqrt(discriminant);

    auto root = (-half_b - sqrtd) / a;
    return root;
}

color ray_color(const ray& r, sphere& sp, hit_record& rec)
{
    if(sp.hit(r, 0, infinity, rec))
    {
        return 0.5 * (rec.normal + color(1,1,1));
    }
    vec3 unit_direction = unit_vector(r.get_direction());
    double t= 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0)
    {
        return color(0,1, 0);
    }
    vec3 unit_direction = unit_vector(r.get_direction());
    t= 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

color ray_color(const ray& r, hittable_list& world, hit_record& rec) {

    if (world.hit(r, 0, infinity, rec))
    {
        return 0.5 * (rec.normal + color(1,1,1));
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
    const int samples_per_pixel = 20;

    // Camera
    camera cam;

    //objects
    hit_record rec;
    hittable_list world;
    world.add(std::make_shared<sphere>(vec3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(vec3(0, -100.5, -1), 100));


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
                pixel_color += ray_color(r, world, rec);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
}
