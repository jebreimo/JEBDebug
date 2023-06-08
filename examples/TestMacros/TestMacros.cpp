//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-04-03.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <JEBDebug/Debug.hpp>
#include <numeric>
#include <sstream>
#include <vector>

void test_SHOW_CONTAINER()
{
    std::vector<int> foo(12);
    std::iota(foo.begin(), foo.end(), 100);
    JEB_SHOW_CONTAINER(foo);
    JEB_SHOW_RANGE(foo.begin(), foo.end());
    JEB_SHOW_CONTAINER_FLAT(foo);
    JEB_SHOW_RANGE_FLAT(foo.begin(), foo.end());
}

void test_HEXDUMP()
{
    std::vector<int> foo(32);
    std::iota(foo.begin(), foo.end(), 1);
    JEB_HEXDUMP(foo);
    JEB_HEXDUMP(foo.data(), foo.size() * sizeof(int));
}

void test_SHOW()
{
    JEB_CHECKPOINT();
    JEB_MESSAGE("Hello world!");
    int i = 3, j = 1000, k = -45;
    std::string foo = "Bar baz!";
    double x = 1, y = 2, z = 1.5, w = 0;
    JEB_SHOW(i, j, k, foo, x, y, z, w, i * x, j * y);
}

void test_REDIRECT()
{
    std::stringstream ss;
    JEBDebug::STREAM.set_stream(ss);
    JEB_CHECKPOINT();
    JEB_MESSAGE("Hello world!");
    std::cout << "Contents of ss:\n"
        << ss.str() << std::flush;
}

int main()
{
    test_SHOW_CONTAINER();
    test_HEXDUMP();
    test_SHOW();
    test_REDIRECT();
    return 0;
}
