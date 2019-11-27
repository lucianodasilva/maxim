#ifndef _maxim_application_hpp_
#define _maxim_application_hpp_

#include "maxim.device.hpp"
#include "maxim.event.hpp"
#include "maxim.input.hpp"
#include "maxim.input.driver.hpp"

#include <atomic>

namespace maxim {

    template < class _sender_t >
    using key_down_event = event < _sender_t, key_info >;

    template < class _sender_t >
    using key_up_event = event < _sender_t, key_info >;

	class application {
	public:

		using application_start_event = event < application >;
		using application_close_event = event < application, bool & >;

		using application_draw_event = event < application, graphics & >;

		application_start_event on_start;
		application_close_event on_exit;

		application_draw_event on_draw;

		input::key_down_event < application >	on_key_down;
		input::key_up_event < application >	on_key_up;

		void run();

		void exit();

		maxim::device & device();

	private:

		maxim::device _device;

		std::atomic_bool
			_is_running{ false };
	};

}

#endif