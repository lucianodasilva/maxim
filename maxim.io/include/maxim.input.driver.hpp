#pragma once
#ifndef MAXIM_INPUT_DRIVER_HPP
#define MAXIM_INPUT_DRIVER_HPP

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
