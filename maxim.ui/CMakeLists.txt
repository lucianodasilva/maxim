
add_library (maxim_ui STATIC
        include/maxim.application.hpp
        include/maxim.button.hpp
        include/maxim.checkbox.hpp
        include/maxim.control.hpp
        include/maxim.control_container.hpp
        include/maxim.details.hpp
        include/maxim.event.hpp
        include/maxim.focus.hpp
        include/maxim.grid.hpp
        include/maxim.key_handler.hpp
        include/maxim.label.hpp
        include/maxim.layout.hpp
        include/maxim.listbox.hpp
        include/maxim.numupdown.hpp
        include/maxim.locked_value.hpp
        include/maxim.paintable.hpp
        include/maxim.panel.hpp
        include/maxim.progress_bar.hpp
        include/maxim.property.hpp
        include/maxim.stack.hpp
        include/maxim.style.hpp
        include/maxim.style_property.hpp
        include/maxim.tab_panel.hpp
        include/maxim.textbox.hpp
        include/maxim.traits.hpp
        src/maxim.application.cpp
        src/maxim.button.cpp
        src/maxim.checkbox.cpp
        src/maxim.control.cpp
        src/maxim.control_container.cpp
        src/maxim.event.cpp
        src/maxim.focus.cpp
        src/maxim.grid.cpp
        src/maxim.label.cpp
        src/maxim.layout.cpp
        src/maxim.listbox.cpp
        src/maxim.numupdown.cpp
        src/maxim.panel.cpp
        src/maxim.progress_bar.cpp
        src/maxim.stack.cpp
        src/maxim.style.cpp
        src/maxim.style_property.cpp
        src/maxim.tab_panel.cpp
        src/maxim.textbox.cpp)

target_include_directories(
        maxim_ui
        PUBLIC include/
        PRIVATE src/)

target_compile_features (
        maxim_ui PUBLIC cxx_std_17)

target_link_libraries(
        maxim_ui PUBLIC maxim::io)

add_library (maxim::ui ALIAS maxim_ui)