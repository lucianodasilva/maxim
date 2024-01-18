
add_executable (demo main.cpp)

target_link_libraries(demo PUBLIC
    maxim::io
    maxim::ui)