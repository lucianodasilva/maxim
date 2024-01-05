#pragma once
#ifndef _maxim_output_unix_driver_hpp_
#define _maxim_output_unix_driver_hpp_

#include "maxim.output.driver.hpp"

namespace maxim {

    struct stream;

    namespace output::_unix {
        class driver : public maxim::output::driver {
        public:

            driver ();
            ~driver() override = default;

            size get_size() const override;

            void send(span < char > data) const override;

        };
    }
}

#endif
