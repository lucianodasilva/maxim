#include "maxim.tui.application.hpp"

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

			}

			// call on_exit event
			on_exit.invoke(*this);
		}

		void application::exit() {
			_is_running = false;
		}

	}
}