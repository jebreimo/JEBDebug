# JEBDebug: C++ macros and functions for debugging and profiling
# Copyright 2014 Jan Erik Breimo
# All rights reserved.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.

cmake_minimum_required(VERSION 2.8)

project(Fibonacci)

set(SRC
    Fibonacci.cpp
    )

add_executable(${PROJECT_NAME} ${SRC})
