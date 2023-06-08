# Created 2023-06-08 by JEB

cmake_minimum_required(VERSION 3.13)

project(MultiUnitFibonacci)

add_executable(MultiUnitFibonacci
    fibonacci_it.cpp
    fibonacci_it.hpp
    fibonacci_rec.cpp
    fibonacci_rec.hpp
    main.cpp
    )

target_link_libraries(MultiUnitFibonacci
    JEBDebug::JEBDebug
    )

target_compile_definitions(MultiUnitFibonacci
    PUBLIC JEB_SHARE_PROFILER
    )