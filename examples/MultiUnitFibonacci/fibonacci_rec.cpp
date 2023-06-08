//
// Created by JBreimo on 08.06.2023.
//

#include "fibonacci_rec.hpp"
#include "JEBDebug/Profiler.hpp"

long fibonacci_rec(long n)
{
    JEB_PROFILE();
    if (n <= 1)
        return 1;
    else
        return fibonacci_rec(n - 1) + fibonacci_rec(n - 2);
}
