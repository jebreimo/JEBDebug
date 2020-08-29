# ===========================================================================
# Copyright © 2014 Jan Erik Breimo. All rights reserved.
# Created by Jan Erik Breimo on 2020-01-07.
#
# This file is distributed under the BSD License.
# License text is included with the source distribution.
# ===========================================================================

# JEBDebug: C++ macros and functions for debugging and profiling

cmake_minimum_required(VERSION 3.13)

project(JEBDebug VERSION 1.0.0)

# Set JEBDEBUG_MASTER_PROJECT to ON if Argos is the root level project.
if (NOT DEFINED JEBDEBUG_MASTER_PROJECT)
    if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(JEBDEBUG_MASTER_PROJECT ON)
    else ()
        set(JEBDEBUG_MASTER_PROJECT OFF)
    endif ()
endif ()

# Install option
option(JEBDEBUG_INSTALL "Generate the install target" ${JEBDEBUG_MASTER_PROJECT})

add_library(JEBDebug INTERFACE)
target_include_directories(JEBDebug
    INTERFACE
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    )

add_library(JEBDebug::JEBDebug ALIAS JEBDebug)

export(TARGETS JEBDebug
    NAMESPACE JEBDebug::
    FILE JEBDebugConfig.cmake)

if (JEBDEBUG_INSTALL)

    include(GNUInstallDirs)

    install(TARGETS JEBDebug
        EXPORT JEBDebugConfig
        )

    install(EXPORT JEBDebugConfig
        FILE
            JEBDebugConfig.cmake
        NAMESPACE
            JEBDebug::
        DESTINATION
            ${CMAKE_INSTALL_LIBDIR}/cmake/JEBDebug
        )

    file(GLOB_RECURSE PUBLIC_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
    install(
        FILES
            ${PUBLIC_INCLUDES}
        DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR}/JEBDebug
        )

    include(CMakePackageConfigHelpers)

    write_basic_package_version_file(JEBDebugConfigVersion.cmake
        COMPATIBILITY SameMajorVersion
        )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/JEBDebugConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/JEBDebug
        )
endif()
