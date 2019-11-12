#ifndef _MAXIM_APPLICATION_HPP_
#define _MAXIM_APPLICATION_HPP_

#include "maxim.device.hpp"
#include "maxim.event.hpp"
#include "maxim.input.driver.hpp"

#include <atomic>

namespace maxim {

	class application {
	public:

		using application_start_event = event < application >;
		using application_close_event = event < application, bool & >;

		application_start_event on_start;
		application_close_event on_exit;

		input::key_down_event < application >	on_key_down;
		input::key_up_event < application >	on_key_up;

		void run();

		void exit();

		maxim::device & device();

	private:

		maxim::device _device;

		std::atomic_bool
			_is_running{ false };

		void dispatch_input_events();
	};

}

#endif