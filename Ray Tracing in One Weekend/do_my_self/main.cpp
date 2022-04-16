//
// Created by jonny on 2022/4/16.
//
#include "camera.h"
#include "vec3.h"
#include "ray.h"
#include "color.h"

color ray_color(ray& r)
{
    vec3 unit_direction = unit_vector(r.get_direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
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


    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    for(int j = image_height-1;j>=0;--j)
    {
        for(int i = 0; i < image_width; ++i)
        {
            double u = double(i)/image_width;
            double v = double(j)/image_height;
            ray r(cam.get_origin(), cam.get_low_left_corner()+point3(u,v,0)-cam.get_origin());
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
}
