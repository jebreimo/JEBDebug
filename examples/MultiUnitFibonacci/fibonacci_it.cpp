//
// Created by JBreimo on 08.06.2023.
//

#include "fibonacci_it.hpp"
#include "JEBDebug/Profiler.hpp"

long fibonacci_it(long n)
{
    JEB_PROFILE();
    long a = 1, b = 1;
    for (long i = 1; i < n; ++i)
    {
        long tmp = b;
        b = a;
        a += tmp;
    }
    return a;
}
