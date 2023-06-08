//
// Created by JBreimo on 08.06.2023.
//

#define JEB_INSTANTIATE_PROFILER
#include "JEBDebug/Profiler.hpp"
#include "JEBDebug/Debug.hpp"
#include <iostream>
#include "fibonacci_it.hpp"
#include "fibonacci_rec.hpp"


int main()
{
    JEB_TIMEIT();

    {
        const int n = 27;
        JEB_PROFILE();
        JEB_SHOW(n, fibonacci_rec(n), fibonacci_it(n));
    }
    JEB_PROFILER_REPORT();
    return 0;
}
