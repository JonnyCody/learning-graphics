//
// Created by jonny on 2022/4/29.
//

#include <random>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>

inline double random_double_number()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double_number(double min, double max)
{
    return min + (max - min) * random_double_number();
}

inline double pdf(double x) {
    return 0.5;
}

int main()
{
    int N = 1000000;
    auto sum = 0.0;
    for (int i = 0; i < N; i++) {
        auto x = random_double_number(0, 2);
        sum += x*x / pdf(x);
    }
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "I = " << sum/N << '\n';
}
