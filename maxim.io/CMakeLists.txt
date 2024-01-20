
add_library (maxim_io STATIC
        include/maxim.character.hpp
        include/maxim.character_buffer.hpp
        include/maxim.character_change_set.hpp
        include/maxim.details.hpp
        include/maxim.encoder.hpp
        include/maxim.graphics.hpp
        include/maxim.input.driver.hpp
        include/maxim.input.hpp
        include/maxim.input.unix.driver.hpp
        include/maxim.input.unix.key_state_emulator.hpp
        include/maxim.input.unix.termio_parser.hpp
        include/maxim.input.win.driver.hpp
        include/maxim.io.hpp
        include/maxim.output.driver.hpp
        include/maxim.output.unix.driver.hpp
        include/maxim.output.win.driver.hpp
        include/maxim.span.hpp
        include/maxim.stream.hpp
        src/maxim.character.cpp
        src/maxim.character_buffer.cpp
        src/maxim.device.cpp
        src/maxim.encoder.cpp
        src/maxim.graphics.cpp
        src/maxim.input.driver.cpp
        src/maxim.input.unix.driver.cpp
        src/maxim.input.unix.key_state_emulator.cpp
        src/maxim.input.unix.termio_parser.cpp
        src/maxim.input.win.driver.cpp
        src/maxim.output.driver.cpp
        src/maxim.output.unix.driver.cpp
        src/maxim.output.win.driver.cpp)

target_include_directories(
        maxim_io
        PUBLIC include/
        PRIVATE src/
)

target_compile_features (
        maxim_io PUBLIC cxx_std_17
)

add_library (maxim::io ALIAS maxim_io)