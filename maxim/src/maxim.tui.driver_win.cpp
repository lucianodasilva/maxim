#ifdef WIN32

#include "maxim.term.driver.hpp"
#include "maxim.term.stream.hpp"

#include <Windows.h>

namespace maxim {
    namespace term {

		struct driver::internals{
			DWORD
				stdout_original_mode,
				stdin_original_mode;

			HANDLE
				stdout_handle,
				stdin_handle;
		};

		driver::driver() :
			_internals { std::make_unique<internals>() }
		{
			// enable ansi terminal sequence processing
			_internals->stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

			if (_internals->stdout_handle == INVALID_HANDLE_VALUE)
				throw std::runtime_error("Failed to aquire valid stdout handle");

			if (!GetConsoleMode(
				_internals->stdout_handle, 
				&_internals->stdout_original_mode
			))
				throw std::runtime_error("Failed to aquire console output mode");

			DWORD out_mode = 
				_internals->stdout_original_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;

			if (!SetConsoleMode(_internals->stdout_handle, out_mode))
				throw std::runtime_error("Failed setting console output mode");

			// enable direct input
			_internals->stdin_handle = GetStdHandle(STD_INPUT_HANDLE);

			if (_internals->stdin_handle == INVALID_HANDLE_VALUE)
				throw std::runtime_error("Failed to aquire valid stdin handle");

			if (!GetConsoleMode(
				_internals->stdin_handle,
				&_internals->stdin_original_mode
			))
				throw std::runtime_error("Failed to aquire console input mode");

			DWORD in_mode = ENABLE_WINDOW_INPUT;

			if (!SetConsoleMode(
				_internals->stdin_handle,
				in_mode
			))
				throw std::runtime_error("Failed setting console input mode");
		}

		driver::~driver() {
			// restore original console input and output modes
			if (_internals->stdout_handle != INVALID_HANDLE_VALUE)
				SetConsoleMode(
					_internals->stdout_handle,
					_internals->stdout_original_mode
				);

			if (_internals->stdin_handle != INVALID_HANDLE_VALUE)
				SetConsoleMode(
					_internals->stdin_handle,
					_internals->stdin_original_mode
				);
		}

		size driver::get_size() const {
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

		void driver::send(maxim::term::stream & stream) const {
			DWORD writen_length = 0;

			WriteConsole(
				GetStdHandle(STD_OUTPUT_HANDLE),
				reinterpret_cast < void * > (stream.data()),
				stream.size(),
				&writen_length,
				nullptr
			);
		}

		std::vector <input_event_info> driver::get_input_events() const {
			if (_internals->stdin_handle == INVALID_HANDLE_VALUE)
				return {};

			DWORD len = 0;
			GetNumberOfConsoleInputEvents(
				_internals->stdin_handle,
				&len
			);

			std::vector<INPUT_RECORD> native_input(len);

			if (!ReadConsoleInput(
				_internals->stdin_handle,
				native_input.data(),
				len,
				&len
			))
				throw std::runtime_error("Failed to read console input");

			std::vector <input_event_info> event_args;
			event_args.reserve(len);

			for (decltype(len) i = 0; i < len; ++i) {
				auto & input = native_input[i];

				if (input.EventType == KEY_EVENT) {
					event_args.push_back(
						input_event_info {
							input.Event.KeyEvent.uChar.AsciiChar,
							static_cast < virtual_key > (input.Event.KeyEvent.wVirtualKeyCode),
							input.Event.KeyEvent.bKeyDown == TRUE
						}
					);
				}
			}

			return event_args;
		}

    }
}

#endif
