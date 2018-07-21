
# maxim core project
project (maxim_core)

add_library(
	maxim_core
	STATIC
		include/maxim.hpp
		src/maxim.cpp
)

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
		include/maxim.tui.application.hpp
		include/maxim.tui.character.hpp
		include/maxim.tui.character_change_set.hpp
		include/maxim.tui.device.hpp
		include/maxim.tui.driver.hpp
		include/maxim.tui.encoding.hpp
		include/maxim.tui.event.hpp
		include/maxim.tui.event_dispatcher.hpp
		include/maxim.tui.graphics.hpp
		include/maxim.tui.input.hpp
		include/maxim.tui.structures.hpp
		src/maxim.tui.application.cpp
		src/maxim.tui.character.cpp
		src/maxim.tui.device.cpp
		src/maxim.tui.driver_unix.cpp 
		src/maxim.tui.driver_win.cpp
		src/maxim.tui.encoding.cpp
		src/maxim.tui.event_dispatcher.cpp
		src/maxim.tui.graphics.cpp
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