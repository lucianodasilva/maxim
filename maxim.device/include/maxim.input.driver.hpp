#pragma once
#ifndef _maxim_input_driver_hpp_
#define _maxim_input_driver_hpp_

#include "maxim.input.hpp"
#include <memory>
#include <vector>

namespace maxim {

    class application;

    namespace input {

        class driver {
        public:
            virtual ~driver() = default;

            virtual void dispatch_events (application & app) const = 0;

            static std::unique_ptr<driver> create();
        };
    }
}

#endif
