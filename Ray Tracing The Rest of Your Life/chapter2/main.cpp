//
// Created by jonny on 2022/4/28.
//

#include "utility.h"

#include <iostream>
#include <iomanip>
#include <stdlib.h>

int main()
{
    int N = 10000000;
    int inside_circle = 0;
    for(int i=0;i<N;++i)
    {
        auto x = random_double_number(-1, 1);
        auto y = random_double_number(-1,1 );
        if(x*x + y*y < 1)
        {
            inside_circle++;
        }
    }
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Estimate of Pi = " << 4*double (inside_circle) / N << '\n';
}