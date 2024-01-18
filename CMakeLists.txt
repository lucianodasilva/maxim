cmake_minimum_required(VERSION 3.20)

# build options
option (MAXIM_BUILD_DEMO "Build Drivers debug executable"                       FALSE)
option (MAXIM_BUILD_TEST "Compile Drivers unit tests"                           FALSE)
option (MAXIM_BUILD_TEST_COVERAGE "Compile Drivers with unit tests coverage"    FALSE)

option (MAXIM_BUILD_ALL "Compile all targets"                                   FALSE)

if (MAXIM_BUILD_ALL)
    set (MAXIM_BUILD_DEMO ON)
    set (MAXIM_BUILD_TEST ON)
    set (MAXIM_BUILD_TEST_COVERAGE ON)
else ()
    if (MAXIM_BUILD_TEST_COVERAGE)
        set (MAXIM_BUILD_TEST ON)
    endif()
endif()

project (maxim)

add_subdirectory(maxim.io)

add_subdirectory(maxim.ui)

if (MAXIM_BUILD_DEMO)
    add_subdirectory(demo)
endif()