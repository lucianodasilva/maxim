#pragma once

#ifdef WIN32
#ifndef MAXIM_INPUT_WIN_DRIVER_HPP
#define MAXIM_INPUT_WIN_DRIVER_HPP

#include "maxim.input.sec_driver.hpp"

#include <Windows.h>

namespace maxim::input::win {

    class sec_driver : public maxim::input::sec_driver {
    public:
                    
		sec_driver();

        virtual ~sec_driver();
        virtual void dispatch_events(input_event_handler& handler) const override;

    private:
		DWORD	_stdin_original_mode;
		HANDLE	_stdin_handle;

		static modifier_key cast_to_modifier(DWORD const & key_event);
    };
}

#endif
#endif