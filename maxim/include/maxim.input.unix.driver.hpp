#pragma once

#ifdef __linux__
#ifndef _maxim_input_unix_driver_hpp_
#define _maxim_input_unix_driver_hpp_

#include <cinttypes>
#include <termio.h>

#include "maxim.input.driver.hpp"
#include "maxim.input.unix.key_state_emulator.hpp"

namespace maxim::input::_unix {

    class driver : public maxim::input::driver {
    public:
        driver();

        virtual ~driver();

        virtual std::vector<key_event_args> get_input_events() const override;

    private:
        termios
            _initial_term_settings;

        mutable key_state_emulator
            _state_emulator;

        mutable std::vector < key_info >
            _key_info_state;
    };


}

#endif
#endif