#pragma once
#ifndef _maxim_io_output_driver_hpp_
#define _maxim_io_output_driver_hpp_

#include "maxim.details.hpp"
#include "maxim.input.hpp"
#include "maxim.span.hpp"

#include <iterator>
#include <memory>
#include <vector>

namespace maxim {

    struct stream;

    namespace output {

		/// Output driver interface
        class driver {
        public:

            virtual ~driver() = default;

			/// Get the current size of the terminal
            virtual size get_size() const = 0;

			/// Send data to the terminal
            virtual void send(span < char > data) const = 0;

			/// Create a new output driver for this platform
            static std::unique_ptr < driver > create();
        };
    }
}

#endif
