#ifndef _MAXIM_TUI_APPLICATION_HPP_
#define _MAXIM_TUI_APPLICATION_HPP_


#include "maxim.event.hpp"
#include "maxim.input.driver.hpp"

#include <atomic>

namespace maxim {

	class application {
	public:

		using application_event = event < application >;

		application_event on_start;
		application_event on_exit;

		input::key_down_event < application >	on_key_down;
		input::key_up_event < application >	on_key_up;

		void run();

		void exit();

		inline input::driver const * input_driver () const { return _input_driver.get(); }

	private:

		std::atomic_bool
			_is_running{ false };

		std::unique_ptr < input::driver > _input_driver;

		void dispatch_input_events();
	};

}

#endif