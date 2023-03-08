cmake_minimum_required(VERSION 3.10)

# build options
option (MAXIM_BUILD_DEMO "Build Drivers debug executable"                       FALSE)
option (MAXIM_BUILD_TEST "Compile Drivers unit tests"                           FALSE)
option (MAXIM_BUILD_ALL "Compile all targets"                                   FALSE)

if (MAXIM_BUILD_ALL)
    set (MAXIM_BUILD_DEMO ON)
    set (MAXIM_BUILD_TEST ON)
endif()

# include tools
include (cmake/tools.cmake)

project (maxim)

add_subdirectory(maxim.io)

if (MAXIM_BUILD_DEMO)
    add_subdirectory(demo)
endif()