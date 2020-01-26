#ifdef WIN32

#define NOMINMAX

#include "maxim.input.win.driver.hpp"
#include "maxim.device.hpp"

#include <exception>
#include <stdexcept>

namespace maxim::input::win {

	driver::driver()
	{
		// enable direct input
		_stdin_handle = GetStdHandle(STD_INPUT_HANDLE);

		if (_stdin_handle == INVALID_HANDLE_VALUE)
			throw std::runtime_error("Failed to aquire valid stdin handle");

		if (!GetConsoleMode(
			_stdin_handle,
			&_stdin_original_mode
		))
			throw std::runtime_error("Failed to aquire console input mode");

		DWORD in_mode = ENABLE_WINDOW_INPUT;

		if (!SetConsoleMode(
			_stdin_handle,
			in_mode
		))
			throw std::runtime_error("Failed setting console input mode");
	}

	driver::~driver() {
		// restore original console input
		if (_stdin_handle != INVALID_HANDLE_VALUE)
			SetConsoleMode(
				_stdin_handle,
				_stdin_original_mode
			);
	}

	void driver::dispatch_events(maxim::input_event_handler& handler) const {
		if (_stdin_handle == INVALID_HANDLE_VALUE)
			return;

		DWORD len = 0;
		GetNumberOfConsoleInputEvents(
			_stdin_handle,
			&len
		);

		std::vector<INPUT_RECORD> native_input(len);

		if (!ReadConsoleInput(
			_stdin_handle,
			native_input.data(),
			len,
			&len
		))
			throw std::runtime_error("Failed to read console input");

		for (decltype(len) i = 0; i < len; ++i) {
			auto & input = native_input[i];

			if (input.EventType == KEY_EVENT) {

				key_info info{
					static_cast <char32_t> (input.Event.KeyEvent.uChar.AsciiChar),
					static_cast <virtual_key> (input.Event.KeyEvent.wVirtualKeyCode),
					cast_to_modifier(input.Event.KeyEvent.dwControlKeyState)
				};

				if (input.Event.KeyEvent.bKeyDown == TRUE) {
					handler.on_key_down(info);
				}
				else {
					handler.on_key_up(info);
				}

			}
		}
	}

	modifier_key driver::cast_to_modifier(DWORD const & key_event) {
		modifier_key key = {};
		auto mod = key_event;

		// evaluate alt
		key = key | (
			((mod & 0x0001) == 0x0001 || (mod & 0x0002) == 0x0002) ? 
			modifier_key::alt :
			modifier_key::none
		);

		// evaluate ctrl
		key = key | (
			(((mod & 0x0004) == 0x0004) || (mod & 0x0008) == 0x0008) ?
			modifier_key::control :
			modifier_key::none
			);

		// evaluate shift
		key = key | (
			((mod & 0x0010) == 0x0010) ?
			modifier_key::shift :
			modifier_key::none
			);

		return key;
	}
}

#endif
