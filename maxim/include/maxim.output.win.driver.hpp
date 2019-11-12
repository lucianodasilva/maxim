#pragma once

#ifdef WIN32
#ifndef _maxim_term_output_win_driver_hpp_
#define _maxim_term_output_win_driver_hpp_

#include "maxim.term.output.driver.hpp"

#include <Windows.h>

namespace maxim {
    namespace term {

		struct stream;

		namespace output {

		    namespace win {

		        class driver : public maxim::term::output::driver {
                public:
					driver();

                    virtual ~driver();

                    virtual size get_size() const override;

                    virtual void send(maxim::term::stream & stream) const override;
				
				private:

					DWORD	_stdout_original_mode;
					HANDLE	_stdout_handle;

                };
            }
        }

    }
}

#endif
#endif