#include "maxim.tui.application.hpp"
#include <iostream>

namespace maxim {
	namespace tui {

		void application::run() {
			bool exp = false;

			// check and set running
			if (!_is_running.compare_exchange_strong(exp, true))
				return; // TODO: possibly throw exception

			// call on_start event
			on_start.invoke(*this);

			while (_is_running) {
				dispatch_input_events();
			}

			// call on_exit event
			on_exit.invoke(*this);
		}

		void application::exit() {
			_is_running = false;
		}

		void application::dispatch_input_events() {
			auto args = _device.driver().get_input_events();

			for (auto & arg : args) {
				auto & e = (arg.is_down ? on_key_down : on_key_up);
				e.invoke(*this, { arg.char_value, arg.key });
			}
		}

	}
}