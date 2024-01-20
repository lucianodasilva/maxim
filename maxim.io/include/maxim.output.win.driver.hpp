#pragma once

#ifdef WIN32
#ifndef _maxim_io_output_win_driver_hpp_
#define _maxim_io_output_win_driver_hpp_

#include "maxim.output.driver.hpp"

#include <Windows.h>

namespace maxim {
	struct stream;

	namespace output::win {

		class driver : public maxim::output::driver {
        public:
			driver();

            ~driver() override;

            maxim::size get_size() const override;

            void send(span < char > data) const override;

		private:

			DWORD	_stdout_original_mode;
			HANDLE	_stdout_handle;

        };
    }    
}

#endif
#endif