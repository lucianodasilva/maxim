#pragma once
#ifndef _maxim_io_input_driver_hpp_
#define _maxim_io_input_driver_hpp_

#include "maxim.input.hpp"
#include <memory>
#include <vector>

namespace maxim {

    class input_event_handler;

    namespace input {

        class driver {
        public:
            virtual ~driver() = default;

            virtual void dispatch_events (input_event_handler & handler) const = 0;

            static std::unique_ptr<driver> create();
        };
    }
}

#endif
