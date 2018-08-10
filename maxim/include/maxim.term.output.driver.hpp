#pragma once
#ifndef _maxim_term_output_driver_hpp_
#define _maxim_term_output_driver_hpp_

#include "maxim.term.structures.hpp"
#include "maxim.term.input.hpp"

#include <iterator>
#include <memory>
#include <vector>

namespace maxim {
    namespace term {

		struct stream;

		namespace output {

            class driver {
            public:

                virtual ~driver() = default;

                virtual size get_size() const = 0;

                virtual void send(maxim::term::stream &stream) const = 0;

				static std::unique_ptr < driver > create();
            };
        }

    }
}

#endif
