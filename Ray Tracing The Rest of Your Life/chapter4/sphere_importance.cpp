//
// Created by jonny on 2022/4/29.
//

#include <random>
#include <iostream>
#include <iomanip>
#include <math.h>

#include "vec3.h"
#include "utility.h"

inline double pdf(const vec3& p) {
    return 1 / (4*pi);
}

int main()
{
    int N = 1000000;
    auto sum = 0.0;
    for (int i = 0; i < N; i++) {
        vec3 d = random_unit_vec3();
        auto cosine_squared = d.z()*d.z();
        sum += cosine_squared / pdf(d);
    }
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "I = " << sum/N << '\n';
}
