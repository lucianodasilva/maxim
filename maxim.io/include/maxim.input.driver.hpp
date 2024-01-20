#pragma once
#ifndef _maxim_io_input_driver_hpp_
#define _maxim_io_input_driver_hpp_

#include <memory>

namespace maxim {

    class input_event_handler;

    namespace input {

		/// Input driver interface
        class driver {
        public:
            virtual ~driver() = default;

			/// Dispatch events to the event handler
			/// \param handler - Event handler
            virtual void dispatch_events (input_event_handler & handler) const = 0;

			/// Create a new input driver for this platform
            static std::unique_ptr<driver> create();
        };
    }
}

#endif
