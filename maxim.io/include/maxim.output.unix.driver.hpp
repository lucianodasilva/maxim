#pragma once
#ifndef MAXIM_OUTPUT_UNIX_DRIVER_HPP
#define MAXIM_OUTPUT_UNIX_DRIVER_HPP

#include "maxim.output.driver.hpp"

namespace maxim {

    struct stream;

    namespace output::unix {
        class driver : public maxim::output::driver {
        public:

            driver ();
            ~driver() override = default;

            size terminal_size() const override;

            void send(view < char > data) const override;

        };
    }
}

#endif
