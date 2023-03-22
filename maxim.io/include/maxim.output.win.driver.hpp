#pragma once

#ifdef WIN32
#ifndef _maxim_output_win_driver_hpp_
#define _maxim_output_win_driver_hpp_

#include "maxim.output.sec_driver.hpp"

#include <Windows.h>

namespace maxim {
	struct stream;

	namespace output::win {

		class sec_driver : public maxim::output::sec_driver {
        public:
			sec_driver();

            ~sec_driver() override;

            size get_size() const override;

            void send(maxim::stream & stream) const override;

		private:

			DWORD	_stdout_original_mode;
			HANDLE	_stdout_handle;

        };
    }    
}

#endif
#endif