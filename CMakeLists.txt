cmake_minimum_required(VERSION 3.10)

include (3rd_party/treemake/cmake/treemake.cmake)
# third parties
set (CATCH_BUILD_TESTING OFF CACHE BOOL "")
add_subdirectory (3rd_party/catch2)

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

if (NOT MAXIM_BUILD_TEST)
    set (TREEMAKE_ENABLE_TESTING CACHE INTERNAL FALSE)
endif()

project (maxim)

add_library_dir(maxim.io STATIC)

add_library_dir(maxim.ui STATIC
        PUBLIC maxim.io)

if (MAXIM_BUILD_DEMO)
    add_executable_dir(demo PUBLIC maxim.io maxim.ui)
endif()