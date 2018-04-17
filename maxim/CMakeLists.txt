
# maxim core project
project (maxim_core)

add_library(
	maxim_core
	STATIC
		include/maxim.hpp
		include/maxim.tui.character.hpp
		include/maxim.tui.device.hpp
		include/maxim.tui.driver_xterm.hpp
		include/maxim.tui.renderer.hpp
		src/maxim.cpp
		src/maxim.tui.character.cpp
		src/maxim.tui.driver_xterm.cpp
		include/maxim.tui.driver.hpp include/maxim.tui.character_change_set.hpp src/maxim.tui.device.cpp)

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
		CXX_STANDARD 17
)

project (maxim)

add_executable(
	maxim
	src/main.cpp
)

target_link_libraries(maxim maxim_core)

set_target_properties(
		maxim
		PROPERTIES
		CXX_STANDARD 17
)