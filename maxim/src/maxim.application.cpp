#include "maxim.application.hpp"
#include <iostream>

namespace maxim {

	void application::run() {

		bool exit_cancel = false;
		bool exp = false;

		// call on_start event
		on_start.invoke(*this);

		do {
			// check and set running
			if (!_is_running.compare_exchange_strong(exp, true))
				return; // TODO: possibly throw exception

			while (_is_running) {
			    _device.dispatch_input_events(*this);

				auto g = _device.graphics();
				this->on_draw.invoke(*this, g);

				_device.present();
				std::this_thread::yield();
			}

			// call on_exit event
			on_exit.invoke(*this, exit_cancel);

		} while (exit_cancel);
	}

	void application::exit() {
		_is_running = false;
	}

	maxim::device & application::device() {
		return _device;
	}

}