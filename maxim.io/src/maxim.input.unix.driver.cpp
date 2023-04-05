#ifdef __linux__

#include <termio.h>
#include <termios.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>

#include <chrono>
#include <unordered_map>
#include <iostream>
#include <iomanip>

#include "maxim.input.unix.driver.hpp"
#include "maxim.input.unix.termio_parser.hpp"

namespace maxim::input::unix {

    driver::driver() {
        // setup term io settings
        _initial_term_settings = termios{};
        tcgetattr(STDIN_FILENO, &_initial_term_settings);
        termios new_settings = _initial_term_settings;

        new_settings.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INPCK | ISTRIP | IXON);
        new_settings.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
        new_settings.c_cflag &= ~(OPOST);
        new_settings.c_cflag |= CS8;
        new_settings.c_cc[VMIN] = 0;
        new_settings.c_cc[VTIME] = key_state_emulator::KEY_DELAY_DS;

        tcsetattr(0, TCSANOW, &new_settings);
        fcntl(STDIN_FILENO, F_SETFL, FNDELAY);
    }

    driver::~driver() {
        // restore term io settings
        fcntl(STDIN_FILENO, 0, FNDELAY);
        tcsetattr(STDIN_FILENO, TCSANOW, &_initial_term_settings);
    }

    void driver::dispatch_events(maxim::input_event_handler & handler) const {

        _state_emulator.timeout_and_dispatch(handler);

        constexpr std::size_t buffer_size { 16 };
        char buffer [buffer_size];

        auto len = ::read(STDIN_FILENO, +buffer, buffer_size);

        if (len < 0) {
            return;
        }

        auto source = std::string_view (buffer, len);

        while (!source.empty ()) {
            auto parsed_key = termio_parse(source);

            if (parsed_key) {
                _state_emulator.evaluate_and_dispatch(parsed_key, handler);
            }
        }
    }
}

#endif