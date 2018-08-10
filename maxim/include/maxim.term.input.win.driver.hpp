#pragma once

#ifdef WIN32
#ifndef _maxim_term_input_win_driver_hpp_
#define _maxim_term_input_win_driver_hpp_

#include "maxim.term.input.driver.hpp"

#include <Windows.h>

namespace maxim {
    namespace term {
        namespace input {
            namespace win {

                class driver : public maxim::term::input::driver {
                public:
                    
					driver();

                    virtual ~driver();
                    virtual std::vector<key_event_args> get_input_events() const override;

                private:
					DWORD	_stdin_original_mode;
					HANDLE	_stdin_handle;

					static modifier_key cast_to_modifier(DWORD const & key_event);
                };

            }
        }
    }
}

#endif
#endif