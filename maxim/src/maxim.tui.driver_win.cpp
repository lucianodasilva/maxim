#ifdef WIN32

#include "maxim.tui.driver.hpp"
#include "maxim.tui.stream.hpp"

#include <Windows.h>

namespace maxim {
    namespace tui {

		driver::driver() {
			auto std_handle = GetStdHandle(STD_OUTPUT_HANDLE);

			if (std_handle == INVALID_HANDLE_VALUE)
				throw std::runtime_error("Failed to aquire valid stdout handle");

			DWORD mode = 0;

			if (!GetConsoleMode(std_handle, &mode))
				throw std::runtime_error("Failed to aquire consome mode!");

			mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

			if (!SetConsoleMode(std_handle, mode))
				throw std::runtime_error("Failed setting consome mode");
		}

		point driver::get_size() const {
			CONSOLE_SCREEN_BUFFER_INFO buffer_info;
			
			GetConsoleScreenBufferInfo(
				GetStdHandle(STD_OUTPUT_HANDLE),
				&buffer_info
			);

			return {
				buffer_info.dwSize.X,
				buffer_info.dwSize.Y
			};
		}

		void driver::send(maxim::tui::stream & stream) {
			DWORD writen_length = 0;

			WriteConsole(
				GetStdHandle(STD_OUTPUT_HANDLE),
				reinterpret_cast < void * > (stream.data()),
				stream.size(),
				&writen_length,
				nullptr
			);
		}

    }
}

#endif