# JEBDebug: C++ macros and functions for debugging and profiling
# Copyright 2014 Jan Erik Breimo
# All rights reserved.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.

cmake_minimum_required(VERSION 2.8)

project(Debug)
set(JEB_DIR JEBDebug)


set(PUBLIC_HEADERS
    CpuTimer.hpp
    Debug.hpp
    Profiler.hpp
    )

if (DEFINED JEB_INSTALL_API)
    install(FILES ${PUBLIC_HEADERS} DESTINATION include/${JEB_DIR})
endif(DEFINED JEB_INSTALL_API)
