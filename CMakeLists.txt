# JEBDebug: C++ macros and functions for debugging and profiling
# Copyright 2014 Jan Erik Breimo
# All rights reserved.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.

cmake_minimum_required(VERSION 2.8)

project(JEBDebug)

set(JEB_INCLUDE_UNIT_TESTS 1)
set(JEB_INSTALL_API 1)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

include_directories(${CMAKE_SOURCE_DIR})

if (APPLE)
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -Wall")
    set(CMAKE_INSTALL_PREFIX "/Users/janeb/usr/local")
    include_directories("/Users/janeb/usr/local/include")
    link_directories("/Users/janeb/usr/local/lib")
elseif (WIN32)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    set(CMAKE_INSTALL_PREFIX "D:/Libraries")
    include_directories("D:/Libraries/include")
    link_directories("D:/Libraries/lib")
endif(APPLE)

add_subdirectory(JEBDebug)
add_subdirectory(Examples)
