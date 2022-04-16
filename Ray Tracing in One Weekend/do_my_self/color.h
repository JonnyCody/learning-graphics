//
// Created by jonny on 2022/4/16.
//

#ifndef RAYTRACING_COLOR_H
#define RAYTRACING_COLOR_H

#include <iostream>

#include "vec3.h"

void write_color(std::ostream& out, color& pixel_color)
{
    out << static_cast<int>(255*pixel_color.x()) << ' '
        << static_cast<int>(255*pixel_color.y()) << ' '
        << static_cast<int>(255*pixel_color.z()) << ' ';
}

#endif //RAYTRACING_COLOR_H
