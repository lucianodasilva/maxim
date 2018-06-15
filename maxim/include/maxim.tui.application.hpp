#ifndef _MAXIM_TUI_APPLICATION_HPP_
#define _MAXIM_TUI_APPLICATION_HPP_

#include "maxim.tui.device.hpp"
#include "maxim.tui.event.hpp"

#include <atomic>

namespace maxim {
	namespace tui {

		class application {
		public:

			using application_event = event < application >;

			application_event on_start;
			application_event on_exit;

			key_down_event < application >	on_key_down;
			key_up_event < application >	on_key_up;

			void run();

			void exit();

			inline maxim::tui::device const & device() const { return _device; }

		private:

			std::atomic_bool 
				_is_running{ false };
			maxim::tui::device			
				_device;

			void dispatch_input_events();
		};

	}
}

#endif