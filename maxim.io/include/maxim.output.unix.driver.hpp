#pragma once
#ifndef _maxim_output_unix_driver_hpp_
#define _maxim_output_unix_driver_hpp_

#include "maxim.output.driver.hpp"

namespace maxim {

    struct stream;

    namespace output::_unix {
        class driver : public maxim::output::driver {
        public:

            virtual ~driver() = default;

            virtual size get_size() const override;

            virtual void send(maxim::stream &stream) const override;

        };
    }
}

#endif
