
# maxim core project
project (maxim_core)

add_library(
	maxim_core
	STATIC
		include/maxim.hpp
		src/maxim.cpp
		include/maxim.term.input.unix.driver.hpp include/maxim.term.input.driver.hpp src/maxim.term.input.driver.cpp include/maxim.term.input.unix.termio_parser.hpp include/maxim.term.input.unix.key_state_emulator.hpp src/maxim.term.input.unix.termio_parser.cpp src/maxim.term.input.unix.key_state_emulator.cpp)

target_include_directories(maxim_core 
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
	PRIVATE
	src
)

target_link_libraries(
	maxim_core
	PUBLIC
)

set_target_properties(
		maxim_core
		PROPERTIES
		CXX_STANDARD 14
)

project (maxim_tui)

add_library(
	maxim_tui
	STATIC
		include/maxim.application.hpp
		include/maxim.term.character.hpp
		include/maxim.term.character_change_set.hpp
		include/maxim.term.device.hpp
		include/maxim.term.driver.hpp
		include/maxim.term.encoding.hpp
		include/maxim.event.hpp
		include/maxim.event_dispatcher.hpp
		include/maxim.term.graphics.hpp
		include/maxim.term.input.hpp
		include/maxim.term.structures.hpp
		include/maxim.threading.hpp
		src/maxim.application.cpp
		src/maxim.term.character.cpp
		src/maxim.term.device.cpp
		src/maxim.term.input.unix.driver.cpp
		src/maxim.tui.driver_win.cpp
		src/maxim.term.encoding.cpp
		src/maxim.term.graphics.cpp
)

target_include_directories(maxim_tui 
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
	PRIVATE
	src
)

set_target_properties(
		maxim_tui
		PROPERTIES
		CXX_STANDARD 14
)

target_link_libraries (maxim_tui maxim_core)

project (maxim)

add_executable(
	maxim
	src/main.cpp
)

target_link_libraries(maxim maxim_core maxim_tui)

set_target_properties(
		maxim
		PROPERTIES
		CXX_STANDARD 14
)

add_executable(maxim_test
	test/main.cpp
	test/unix_input_parser_test.cpp
        test/unix_key_state_emulator_test.cpp)

target_link_libraries(maxim_test maxim_core maxim_tui Catch2::Catch2)

catch_discover_tests(maxim_test)

set_target_properties(
		maxim_test
		PROPERTIES
		CXX_STANDARD 14
)