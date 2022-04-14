//
// Created by jonny on 2022/4/13.
//

#ifndef RAY_RTWEEKEND_H
#define RAY_RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>

#include "ray.h"
#include "vec3.h"

// Using
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

#endif //RAY_RTWEEKEND_H
