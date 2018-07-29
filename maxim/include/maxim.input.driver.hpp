#ifndef _MAXIM_INPUT_DRIVER_HPP_
#define _MAXIM_INPUT_DRIVER_HPP_

#include "maxim.input.hpp"
#include <memory>
#include <vector>

namespace maxim {
    namespace input {

        class driver {
        public:
            virtual ~driver () = default;

            virtual std::vector<input_event_info> get_input_events() const = 0;
        };

    }
}

#endif //_MAXIM_INPUT_DRIVER_HPP_
