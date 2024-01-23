
add_executable (demo_ui main.cpp)

target_link_libraries(demo_ui PUBLIC
    maxim::io
    maxim::ui)