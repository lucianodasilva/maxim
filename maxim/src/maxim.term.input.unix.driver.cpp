#ifdef __linux__

#include <termio.h>
#include <termios.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>

#include <chrono>
#include <unordered_map>

#include "maxim.term.input.unix.driver.hpp"
#include "maxim.term.input.unix.termio_parser.hpp"

namespace maxim {
    namespace term {
        namespace input {
            namespace _unix {

                driver::driver() {
                    // setup term io settings
                    _initial_term_settings = termios{};
                    tcgetattr(0, &_initial_term_settings);
                    termios new_settings = _initial_term_settings;

                    new_settings.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INPCK | ISTRIP | IXON);
                    new_settings.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
                    new_settings.c_cflag
                        &= ~(OPOST);
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
                    uint64_t v = 0;

                    auto len = read(STDIN_FILENO, &v, sizeof(uint64_t));
                    key_event_args key = parse_termio_input(v, len);

                    return _state_emulator.update_key_state(key);
                }
            }
        }
    }
}

#endif