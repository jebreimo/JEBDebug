# ===========================================================================
# Copyright © 2021 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2021-04-03.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.
# ===========================================================================
cmake_minimum_required(VERSION 3.13)

project(TestMacros)

add_executable(${PROJECT_NAME}
    TestMacros.cpp)

target_link_libraries(${PROJECT_NAME}
    JEBDebug::JEBDebug
    )
