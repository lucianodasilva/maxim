cmake_minimum_required(VERSION 3.10)

# build options
option (MAXIM_BUILD_DEMO "Build debug executable"       FALSE)
option (MAXIM_BUILD_TEST "Compile Drivers unit tests"   FALSE)

# include tools
include (cmake/tools.cmake)
include (FetchContent)

project (maxim)

add_subdirectory(maxim.io)

if (MAXIM_BUILD_DEMO)
    add_subdirectory(demo)
endif()