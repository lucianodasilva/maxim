#pragma once
#ifndef _maxim_input_driver_hpp_
#define _maxim_input_driver_hpp_

#include "maxim.input.hpp"
#include <memory>
#include <vector>

namespace maxim::input {

    class driver {
    public:
        virtual ~driver() = default;

        virtual std::vector<key_event_args> get_input_events() const = 0;

        static std::unique_ptr<driver> create();
    };
}

#endif
