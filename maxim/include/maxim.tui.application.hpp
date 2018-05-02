#ifndef _MAXIM_TUI_APPLICATION_HPP_
#define _MAXIM_TUI_APPLICATION_HPP_

#include "maxim.tui.event.hpp"

#include <atomic>

namespace maxim {
	namespace tui {

		class application {
		public:

			using application_event = event < application >;

			application_event on_start;
			application_event on_exit;

			void run();

			void exit();

		private:

			std::atomic_bool _is_running{ false };

		};

	}
}

#endif