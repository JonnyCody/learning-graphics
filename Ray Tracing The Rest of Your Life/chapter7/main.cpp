//
// Created by jonny on 2022/4/29.
//

#include <random>
#include <iostream>
#include <iomanip>
#include <math.h>

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double random_double_number()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

int main()
{
    for (int i = 0; i < 200; i++) {
        auto r1 = random_double_number();
        auto r2 = random_double_number();
        auto x = cos(2*pi*r1)*2*sqrt(r2*(1-r2));
        auto y = sin(2*pi*r1)*2*sqrt(r2*(1-r2));
        auto z = 1 - 2*r2;
        std::cout << x << " " << y << " " << z << '\n';
    }
}
