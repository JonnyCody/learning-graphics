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
    int N = 1000000;
    auto sum = 0.0;
    for (int i = 0; i < N; i++) {
        auto r1 = random_double_number();
        auto r2 = random_double_number();
        auto x = cos(2*pi*r1)*2*sqrt(r2*(1-r2));
        auto y = sin(2*pi*r1)*2*sqrt(r2*(1-r2));
        auto z = 1 - r2;
        sum += z*z*z / (1.0/(2.0*pi));
    }
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Pi/2     = " << pi/2 << '\n';
    std::cout << "Estimate = " << sum/N << '\n';
}
