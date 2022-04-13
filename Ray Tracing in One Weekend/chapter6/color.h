//
// Created by jonny on 2022/4/12.
//

#ifndef CHAPTER3_COLOR_H
#define CHAPTER3_COLOR_H
#include "vec3.h"
#include <iostream>

void write_color(std::ostream &out, color pixel_color) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#endif //CHAPTER3_COLOR_H
