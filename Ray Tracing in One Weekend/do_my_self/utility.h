//
// Created by jonny on 2022/4/16.
//

#ifndef RAYTRACING_UTILITY_H
#define RAYTRACING_UTILITY_H

#include <limits>

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degree_to_radian(double degrees)
{
    return pi*degrees/180.0;
}

#endif //RAYTRACING_UTILITY_H
