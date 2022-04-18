//
// Created by jonny on 2022/4/16.
//
#include "camera.h"
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "hittable.h"
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

int main()
{
    // Image
    double aspect_ratio = 16.0/9.0;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    camera cam;

    //objects
    hit_record rec;
    point3 cen(0, 0, -1);
    sphere sphere1(cen, 0.5);


    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    for(int j = image_height-1;j>=0;--j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for(int i = 0; i < image_width; ++i)
        {
            double u = double(i)/image_width;
            double v = double(j)/image_height;
            ray r(cam.get_origin(), cam.get_low_left_corner()+u*cam.get_horizontal() + v*cam.get_vertical());
            color pixel_color = ray_color(r, sphere1, rec);
            write_color(std::cout, pixel_color);
        }
    }
}
