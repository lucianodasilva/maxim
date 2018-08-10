#ifndef _MAXIM_TUI_APPLICATION_HPP_
#define _MAXIM_TUI_APPLICATION_HPP_

#include "maxim.term.device.hpp"
#include "maxim.event.hpp"
#include "maxim.term.input.driver.hpp"

#include <atomic>

namespace maxim {

	class application {
	public:

		using application_start_event = event < application >;
		using application_close_event = event < application, bool & >;

		application_start_event on_start;
		application_close_event on_exit;

		term::input::key_down_event < application >	on_key_down;
		term::input::key_up_event < application >	on_key_up;

		void run();

		void exit();

		term::device & device();

	private:

		term::device _device;

		std::atomic_bool
			_is_running{ false };

		void dispatch_input_events();
	};

}

#endif