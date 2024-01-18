#include "maxim.application.hpp"
#include "maxim.control.hpp"

#include <thread>

namespace maxim {

	void application::run() {

		bool exit_cancel = false;
		bool exp = false;

		// call on_start event
		started.invoke(*this);

		do {
			// check and set running
			if (!_is_running.compare_exchange_strong(exp, true))
				return; // TODO: possibly throw exception

			while (_is_running) {
				_device.dispatch_input_events(*this);

				auto g = maxim::graphics { _device.backbuffer() };
				this->on_paint(g, { {0, 0}, _device.get_size() });

				_device.present();
				std::this_thread::yield();
			}

			// call on_exit event
			exit_cancel = false;
			exiting.invoke(*this, exit_cancel);

		} while (exit_cancel);
	}

	void application::stop() {
		_is_running = false;
	}

	maxim::device & application::device() {
		return _device;
	}

	void application::on_key_up (input::key_info const & key) {
		basic_key_handler<application>::on_key_up(key);

		if (current_focus() != nullptr) {
			current_focus()->on_key_up(key);
		}
	}

	void application::on_key_down (input::key_info const & key) {
		basic_key_handler<application>::on_key_down(key);

		if (current_focus() != nullptr) {
			current_focus()->on_key_down(key);
		}
	}

	void application::on_key_press (input::key_info const & key) {
		basic_key_handler<application>::on_key_press(key);

		if (current_focus() != nullptr) {
			current_focus()->on_key_press(key);
		}
	}

	void application::on_paint(maxim::graphics &g, maxim::rect bounds) {

		if (paint_order == application_paint_order::pre_controls) {
			basic_paintable < application >::on_paint(g, bounds);
		}

		if (active()) {
			active()->on_paint(g, bounds);
		}

		if (paint_order == application_paint_order::post_controls) {
			basic_paintable < application >::on_paint(g, bounds);
		}
	}

	void application::do_activate() {
		auto * root_ptr = active();

		if (root_ptr == nullptr) {
			return;
		}

		// call page shown event
		root_ptr->on_activate();

		// set focus to first control
		this->focus_next ();
	}

	void application::do_deactivate() {
		auto * root_ptr = active();

		if (root_ptr == nullptr) {
			return;
		}

		// call page hidden event
		root_ptr->on_deactivate();

		// reset focus
		this->focus_reset ();
	}

	void application::pop_active() {
		if (_active_stack.empty()) {
			return;
		}

		// call page hidden event
		do_deactivate();

		// remove root
		_active_stack.pop_back();

		// check if we need to stop
		if (close_on_empty_active_queue && _active_stack.empty()) {
			stop();
			return;
		}

		// activate new root
		do_activate();
	}

}