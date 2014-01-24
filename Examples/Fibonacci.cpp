/* JEBTest: A C++ unit testing framework
 * Copyright 2014 Jan Erik Breimo
 * All rights reserved.
 *
 * This file is distributed under the BSD License.
 * License text is included with the source distribution.
 */
#include "JEBDebug/Profiler.hpp"
#include <iostream>

long fibonacci_rec(long n)
{
    JEB_PROFILE();
    if (n <= 1)
        return 1;
    else
        return fibonacci_rec(n - 1) + fibonacci_rec(n - 2);
}

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

int main()
{
    JEB_TIMEIT();
    int n = 25;
    {
        JEB_PROFILE();
        JEB_SHOW3(n, fibonacci_rec(n), fibonacci_it(n));
    }
    JEB_PROFILER_REPORT();
    return 0;
}
