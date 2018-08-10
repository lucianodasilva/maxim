#pragma once

#ifdef __linux__
#ifndef _maxim_term_input_unix_driver_hpp_
#define _maxim_term_input_unix_driver_hpp_

#include <cinttypes>
#include <termio.h>

#include "maxim.term.input.driver.hpp"
#include "maxim.term.input.unix.key_state_emulator.hpp"

namespace maxim {
    namespace term {
        namespace input {
            namespace _unix {

                class driver : public maxim::term::input::driver {
                public:
                    driver();

                    virtual ~driver();

                    virtual std::vector<key_event_args> get_input_events() const override;

                private:
                    termios
                        _initial_term_settings;

                    mutable key_state_emulator
                        _state_emulator;
                };

            }
        }
    }
}

#endif
#endif