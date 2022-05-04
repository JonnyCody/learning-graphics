//
// Created by jonny on 2022/4/16.
//
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

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(std::make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double_number();
            point3 center(a + 0.9*random_double_number(), 0.2, b + 0.9*random_double_number());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random_vec3() * random_vec3();
                    sphere_material = std::make_shared<lambertian>(albedo);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random_vec3(0.5, 1);
                    auto fuzz = random_double_number(0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    point3 center2 = center + point3(0, random_double_number(0, .5), 0);
                    world.add(std::make_shared<moving_sphere>(center, center2, 0.2, 0.0, 1.0, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<dielectric>(1.5);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<dielectric>(1.5);
    auto material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
    world.add(std::make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(std::make_shared<sphere>(point3(-1.0,    0.0, -1.0), -0.45, material_left));
    world.add(std::make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    return world;
}

hittable_list earth()
{
    hittable_list world;
    auto earth_texture = std::make_shared<image_texture>("../resource/earthmap.jpg");
    world.add(std::make_shared<sphere>(point3(0.0, 0.0, 0.0), 2, std::make_shared<lambertian>(earth_texture)));
    return world;
}

hittable_list simple_light()
{
    hittable_list world;
    auto earth_texture = std::make_shared<image_texture>("../resource/earthmap.jpg");
    world.add(std::make_shared<sphere>(point3(0, -1000, 0),
                                       1000, std::make_shared<lambertian>(std::make_shared<solid_color>(color(0, 1, 0)))));
    world.add(std::make_shared<sphere>(point3(0, 2, 0), 2, std::make_shared<lambertian>(earth_texture)));

    auto difflight = std::make_shared<diffuse_light>(color(4, 4, 4));
    world.add(std::make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));
    world.add(std::make_shared<sphere>(point3(0, 7, 0), 2, difflight));

    return world;
}

hittable_list cornell_box() {
    hittable_list objects;

    auto red   = std::make_shared<lambertian>(color(.65, .05, .05));
    auto white = std::make_shared<lambertian>(color(.73, .73, .73));
    auto green = std::make_shared<lambertian>(color(.12, .45, .15));
    auto light = std::make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<xz_rect>(213, 343, 227, 332, 554, light));
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

hittable_list cornell_smoke()
{
    hittable_list objects;

    auto red   = std::make_shared<lambertian>(color(.65, .05, .05));
    auto white = std::make_shared<lambertian>(color(.73, .73, .73));
    auto green = std::make_shared<lambertian>(color(.12, .45, .15));
    auto light = std::make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(std::make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    std::shared_ptr<hittable> box1 = std::make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = std::make_shared<rotate_y>(box1, 15);
    box1 = std::make_shared<translate>(vec3(265,0,295), box1);

    std::shared_ptr<hittable> box2 = std::make_shared<box>(point3(0,0,0), point3(165,165,165), white);
    box2 = std::make_shared<rotate_y>(box2, -18);
    box2 = std::make_shared<translate>( vec3(130,0,65), box2);

    objects.add(std::make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
    objects.add(std::make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));

    return objects;
}

hittable_list final_scene()
{
    hittable_list boxes1;
    auto ground = std::make_shared<lambertian>(color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for(int i=0;i<boxes_per_side;++i)
    {
        for(int j=0;j<boxes_per_side;++j)
        {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double_number(1,101);
            auto z1 = z0 + w;

            boxes1.add(std::make_shared<box>(point3(x0,y0,z0), point3(x1,y1,z1), ground));
        }
    }

    hittable_list objects;

    objects.add(std::make_shared<bvh_node>(boxes1, 0, 1));

    auto light = std::make_shared<diffuse_light>(color(7, 7, 7));
    objects.add(std::make_shared<xz_rect>(123, 423, 147, 412, 554, light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30,0,0);
    auto moving_sphere_material = std::make_shared<lambertian>(color(0.7, 0.3, 0.1));
    objects.add(std::make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(std::make_shared<sphere>(point3(260, 150, 45), 50, std::make_shared<dielectric>(1.5)));
    objects.add(std::make_shared<sphere>(
            point3(0, 150, 145), 50, std::make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = std::make_shared<sphere>(point3(360,150,145), 70, std::make_shared<dielectric>(1.5));
    objects.add(boundary);
    objects.add(std::make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = std::make_shared<sphere>(point3(0, 0, 0), 5000, std::make_shared<dielectric>(1.5));
    objects.add(std::make_shared<constant_medium>(boundary, .0001, color(1,1,1)));

    auto emat = std::make_shared<lambertian>(std::make_shared<image_texture>("../resource/earthmap.jpg"));
    objects.add(std::make_shared<sphere>(point3(400,200,400), 100, emat));
    auto pertext = std::make_shared<noise_texture>(0.1);
    objects.add(std::make_shared<sphere>(point3(220,280,300), 80, std::make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = std::make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(std::make_shared<sphere>(random_vec3(0,165), 10, white));
    }

    objects.add(std::make_shared<translate>(vec3(-100,270,395),
            std::make_shared<rotate_y>(std::make_shared<bvh_node>(boxes2, 0.0, 1.0), 15))
    );

    return objects;
}
color ray_color(const ray& r, hittable_list& world, color background, int depth)
{
    hit_record rec;
    if (world.hit(r, 0.001, infinity, rec))
    {
        ray scattered;
        color attenuation;
        color emitted = rec.mtr_ptr->emitted(rec.u, rec.v, rec.position);
        if(depth > 0 && rec.mtr_ptr->scatter(r, rec, attenuation, scattered))
        {
            return emitted +  attenuation * ray_color(scattered, world, background, depth - 1);
        }
        else
        {
            return emitted;
        }
    }
    return background;
}

color ray_color(const ray& r, bvh_node& nodes, const color& background, int depth)
{
    hit_record rec;
    if (nodes.hit(r, 0.001, infinity, rec))
    {
        ray scattered;
        color attenuation;
        color emitted = rec.mtr_ptr->emitted(rec.u, rec.v, rec.position);
        if(depth > 0 && rec.mtr_ptr->scatter(r, rec, attenuation, scattered))
        {
            return emitted +  attenuation * ray_color(scattered, nodes, background, depth - 1);
        }
        else
        {
            return emitted;
        }
    }
    return background;
}

int main()
{
    // Image
    double aspect_ratio = 1.0/1.0;
    int image_width = 600;
    int samples_per_pixel = 100;
    const int max_depth = 50;


    //world
    hittable_list world;
    point3 lookfrom;
    point3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;
    color background(0, 0, 0);
    switch (4) {
        case 1:
            world = random_scene();
            background = color(0.70, 0.80, 1.00);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            aperture = 0.1;
            break;
        case 2:
            world = earth();
            background = color(0.70, 0.80, 1.00);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            break;
        case 3:
            world = simple_light();
            background = color(0, 0, 0);
            lookfrom = point3(26, 3, 6);
            lookat = point3(0, 2, 0);
            vfov = 40.0;
            break;
        case 4:
            world = cornell_box();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 200;
            background = color(0,0,0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            break;
        case 5:
            world = cornell_smoke();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 200;
            background = color(0,0,0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            break;
        case 6:
            world = final_scene();
            aspect_ratio = 1.0;
            image_width = 800;
            samples_per_pixel = 10000;
            background = color(0,0,0);
            lookfrom = point3(478, 278, -600);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            break;
    }

    // create bvh node
    bvh_node nodes(world, 0.001, infinity);

    // Camera
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
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
                pixel_color += ray_color(r, nodes, background, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
}
