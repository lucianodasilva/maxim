cmake_minimum_required(VERSION 3.9)

# third parties
add_subdirectory (3rd_party/catch2)

include(CTest)
include(3rd_party/catch2/contrib/Catch.cmake)
# project
add_subdirectory (maxim)
