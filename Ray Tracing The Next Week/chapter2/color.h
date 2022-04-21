//
// Created by jonny on 2022/4/16.
//

#ifndef RAYTRACING_COLOR_H
#define RAYTRACING_COLOR_H

#include <iostream>

#include "vec3.h"

void write_color(std::ostream& out, color& pixel_color, int samples_per_pixel)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    auto scale = 1.0 / samples_per_pixel;
    /*r = sqrt(r * scale);
    g = sqrt(g * scale);
    b = sqrt(b * scale);*/
    r*=scale;
    g*=scale;
    b*=scale;
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << ' ';
}

#endif //RAYTRACING_COLOR_H
