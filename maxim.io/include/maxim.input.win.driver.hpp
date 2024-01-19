#pragma once

#ifdef WIN32
#ifndef _maxim_io_input_win_driver_hpp_
#define _maxim_io_input_win_driver_hpp_

#include "maxim.input.driver.hpp"

#include <Windows.h>

namespace maxim::input::win {

    class driver : public maxim::input::driver {
    public:
                    
        driver();

        virtual ~driver();
        virtual void dispatch_events(input_event_handler& handler) const override;

    private:
		DWORD	_stdin_original_mode;
		HANDLE	_stdin_handle;

		static modifier_key cast_to_modifier(DWORD const & key_event);
    };
}

#endif
#endif