#pragma once
#ifndef MAXIM_OUTPUT_DRIVER_HPP
#define MAXIM_OUTPUT_DRIVER_HPP

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

            virtual size terminal_size() const = 0;

            virtual void send(view < char > data) const = 0;

            static std::unique_ptr < driver > create();
        };
    }
}

#endif
