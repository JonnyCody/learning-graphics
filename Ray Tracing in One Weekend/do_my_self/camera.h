//
// Created by jonny on 2022/4/16.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include <cmath>

#include "vec3.h"
#include "utility.h"

class camera
{
public:
    camera()
    {
        origin = point3(0, 0, -1);
        vfov = 120;
        double aspect_ratio = 16.0/9.0;
        double h = tan(degree_to_radian(vfov)/2);
        double viewport_height = 2.0;
        double viewport_width = viewport_height*aspect_ratio;
        double focal_length = viewport_height / h;
        origin = point3(0, 0, 0);
        vertical = vec3(0, viewport_height, 0);
        horizontal = vec3(viewport_width, 0, 0);
        low_left_corner = point3(origin - vertical/2 - horizontal/2 - vec3(0,0, focal_length));
    }
    point3 get_origin() const {return origin;}
    point3 get_low_left_corner() const {return low_left_corner;}
    vec3 get_horizontal() const { return horizontal;}
    vec3 get_vertical() const {return vertical;}
private:
    point3 origin;
    point3 low_left_corner;
    vec3 vertical;
    vec3 horizontal;
    double vfov;
};

#endif //RAYTRACING_CAMERA_H
