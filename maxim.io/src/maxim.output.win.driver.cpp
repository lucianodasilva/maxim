#ifdef WIN32

#include "maxim.output.win.driver.hpp"
#include "maxim.stream.hpp"

namespace maxim::output::win {

	sec_driver::sec_driver()
	{
		// enable ansi terminal sequence processing
		_stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		if (_stdout_handle == INVALID_HANDLE_VALUE)
			throw std::runtime_error("Failed to aquire valid stdout handle");

		if (!GetConsoleMode(
			_stdout_handle,
			&_stdout_original_mode
		))
			throw std::runtime_error("Failed to aquire console output mode");

		DWORD out_mode =
			_stdout_original_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;

		if (!SetConsoleMode(_stdout_handle, out_mode))
			throw std::runtime_error("Failed setting console output mode");
	}

	sec_driver::~sec_driver() {
		// restore original console input and output modes
		if (_stdout_handle != INVALID_HANDLE_VALUE)
			SetConsoleMode(
				_stdout_handle,
				_stdout_original_mode
		);
	}

	size sec_driver::get_size() const {
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

	void sec_driver::send(maxim::stream & stream) const {
		DWORD writen_length = 0;

		WriteConsole(
			GetStdHandle(STD_OUTPUT_HANDLE),
			reinterpret_cast <void *> (stream.data()),
			stream.size(),
			&writen_length,
			nullptr
		);
	}

}

#endif
