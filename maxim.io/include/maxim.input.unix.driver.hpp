#pragma once

#ifdef __linux__
#ifndef MAXIM_INPUT_UNIX_DRIVER_HPP
#define MAXIM_INPUT_UNIX_DRIVER_HPP

#include <cinttypes>
#include <termio.h>

#include "maxim.input.driver.hpp"
#include "maxim.input.unix.key_state_emulator.hpp"

namespace maxim::input::unix {

    class driver : public maxim::input::driver {
    public:
        driver();

        ~driver() override;

        virtual void dispatch_events (input_event_handler & handler) const override;

    private:

        termios
            _initial_term_settings;

        mutable key_state_emulator
            _state_emulator;
    };


}

#endif
#endif