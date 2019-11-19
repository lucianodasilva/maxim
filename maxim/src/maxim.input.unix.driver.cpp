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

namespace maxim::input::_unix {

    driver::driver() {
        // setup term io settings
        _initial_term_settings = termios{};
        tcgetattr(0, &_initial_term_settings);
        termios new_settings = _initial_term_settings;

        new_settings.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INPCK | ISTRIP | IXON);
        new_settings.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
        new_settings.c_cflag &= ~(OPOST);
        new_settings.c_cflag |= CS8;
        new_settings.c_cc[VMIN] = 0;
        new_settings.c_cc[VTIME] = key_state_emulator::key_delay_ds;

        tcsetattr(0, TCSANOW, &new_settings);
        fcntl(STDIN_FILENO, F_SETFL, FNDELAY);
    }

    driver::~driver() {
        // restore term io settingss
        fcntl(STDIN_FILENO, 0, FNDELAY);
        tcsetattr(0, TCSANOW, &_initial_term_settings);
    }

    std::vector<key_event_args> driver::get_input_events() const {
        constexpr std::size_t buffer_size { 16 };
        char buffer [buffer_size];

        auto len = read(STDIN_FILENO, +buffer, buffer_size);

        if (len < 0)
            return {};

        // debug printing
         if (len > 0) {
            for (int i = 0; i < len; ++i) {
                std::cout << "0x" << std::hex << std::setw(2) << std::setfill ('0') << (int)buffer [i] << " ";
            }

            std::cout << "'";

            for (int i = 0; i < len; ++i) {
                std::cout << (char)(std::isgraph(buffer [i]) ? buffer[i] : ' ');
            }

            std::cout << "'";

            std::cout << std::endl;
        }
        // --------------
        _key_info_state.resize (0);
        parse_termio_input(buffer, len, _key_info_state);

        return _state_emulator.update_key_states(_key_info_state);
    }
}

#endif