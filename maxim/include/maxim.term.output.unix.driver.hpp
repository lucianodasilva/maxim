#pragma once
#ifndef _maxim_term_output_unix_driver_hpp_
#define _maxim_term_output_unix_driver_hpp_

#include "maxim.term.output.driver.hpp"

namespace maxim {
    namespace term {

		struct stream;

		namespace output {

		    namespace _unix {

		        class driver : public maxim::term::output::driver {
                public:

                    virtual ~driver() = default;

                    virtual size get_size() const override;

                    virtual void send(maxim::term::stream &stream) const override;

                };
            }
        }

    }
}

#endif
