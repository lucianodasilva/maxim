#pragma once
#ifndef _maxim_output_driver_hpp_
#define _maxim_output_driver_hpp_

#include "maxim.details.hpp"
#include "maxim.input.hpp"

#include <iterator>
#include <memory>
#include <vector>

namespace maxim {

    struct stream;

    namespace output {

        class driver {
        public:

            virtual ~driver() = default;

            virtual size get_size() const = 0;

            virtual void send(maxim::stream &stream) const = 0;

            static std::unique_ptr < driver > create();
        };
    }
}

#endif
