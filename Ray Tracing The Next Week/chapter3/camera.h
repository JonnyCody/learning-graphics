//
// Created by jonny on 2022/4/16.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include <cmath>

#include "vec3.h"
#include "utility.h"
#include "ray.h"

class camera
{
public:
    camera()
    {
        origin = point3(0, 0, -1);
        double aspect_ratio = 16.0/9.0;
        double viewport_height = 2.0;
        double viewport_width = viewport_height*aspect_ratio;
        double focal_length = 1.0;
        origin = point3(0, 0, 0);
        vertical = vec3(0, viewport_height, 0);
        horizontal = vec3(viewport_width, 0, 0);
        low_left_corner = point3(origin - vertical/2 - horizontal/2 - vec3(0,0, focal_length));
    }

    camera(
            point3 lookfrom,
            point3 lookat,
            vec3 vup,
            double vfov,
            double aspect_ratio,
            double aperture,
            double focus_dist,
            double _time0 = 0,
            double _time1 = 0
            )
    {
        auto theta = degree_to_radian(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w,u);

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        low_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

        lens_radius = aperture / 2;
        time0 = _time0;
        time1 = _time1;
    }
    point3 get_origin() const {return origin;}
    point3 get_low_left_corner() const {return low_left_corner;}
    vec3 get_horizontal() const { return horizontal;}
    vec3 get_vertical() const {return vertical;}
    ray get_ray(double s, double t)
    {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();

        return ray(
                origin + offset,
                low_left_corner + s*horizontal + t*vertical - origin - offset,
                random_number_double(time0,time1)
        );
    }

protected:
    point3 origin;
    point3 low_left_corner;
    vec3 vertical;
    vec3 horizontal;
    vec3 u, v, w;
    double lens_radius;
    double time0, time1;
};

#endif //RAYTRACING_CAMERA_H
