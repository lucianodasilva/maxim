#pragma once
#ifndef _maxim_term_input_driver_hpp_
#define _maxim_term_input_driver_hpp_

#include "maxim.term.input.hpp"
#include <memory>
#include <vector>

namespace maxim {
    namespace term {
        namespace input {

            class driver {
            public:
                virtual ~driver() = default;

                virtual std::vector<input_event_info> get_input_events() const = 0;

                static std::unique_ptr<driver> create();
            };

        }
    }
}

#endif
