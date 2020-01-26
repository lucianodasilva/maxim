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
			    // handle inputs
			    _device.dispatch_input_events(_event_proxy);

			    // update layout and render
				auto g = maxim::graphics { _device.backbuffer() };

				g.clear ();

                if (root)
                    root->paint(g, {{0, 0}, _device.get_size ()});

				this->on_draw.invoke(*this, g);

				// present
				_device.present();

				// yield to the operating system
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

	application::io_event_proxy::io_event_proxy(maxim::application &app) :
	    _application { app }
	{}

	void application::io_event_proxy::on_key_up (input::key_info const & key) {
        _application.on_key_up.invoke (_application, key);
	}

    void application::io_event_proxy::on_key_down (input::key_info const & key) {
        _application.on_key_down.invoke(_application, key);
    }

}