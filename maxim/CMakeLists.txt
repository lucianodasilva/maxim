
# maxim core project
project (maxim)

add_library(
	maxim
	STATIC
		include/maxim.application.hpp
		include/maxim.event.hpp
		include/maxim.event_dispatcher.hpp
		include/maxim.hpp
		include/maxim.character.hpp
		include/maxim.character_change_set.hpp
		include/maxim.device.hpp
		include/maxim.encoding.hpp
		include/maxim.graphics.hpp
		include/maxim.input.driver.hpp
		include/maxim.input.hpp
		include/maxim.input.unix.driver.hpp
		include/maxim.input.unix.key_state_emulator.hpp
		include/maxim.input.unix.termio_parser.hpp
		include/maxim.output.driver.hpp
		include/maxim.output.unix.driver.hpp
		include/maxim.stream.hpp
		include/maxim.threading.hpp
		src/maxim.application.cpp
        src/maxim.cpp
		src/maxim.character.cpp
		src/maxim.device.cpp
		src/maxim.encoding.cpp
		src/maxim.graphics.cpp
		src/maxim.input.driver.cpp
		src/maxim.input.unix.driver.cpp
		src/maxim.input.unix.key_state_emulator.cpp
		src/maxim.input.unix.termio_parser.cpp
		src/maxim.input.win.driver.cpp
		src/maxim.output.driver.cpp
		src/maxim.output.unix.driver.cpp
		src/maxim.output.win.driver.cpp
)

target_include_directories(
		maxim
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
	PRIVATE
		src
)

target_compile_features(
		maxim
	PUBLIC
		cxx_std_17)

add_executable(
	maxim_demo
	src/main.cpp
)

target_link_libraries(maxim_demo maxim)

target_compile_features(
		maxim_demo
		PUBLIC
		cxx_std_17)

add_executable(maxim_test
	test/main.cpp
	test/unix_input_parser_test.cpp
        test/unix_key_state_emulator_test.cpp)

target_link_libraries(maxim_test maxim Catch2::Catch2)

catch_discover_tests(maxim_test)

target_compile_features(
		maxim_test
		PUBLIC
		cxx_std_17)