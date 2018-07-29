#ifdef __linux__

#include <termio.h>
#include <termios.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>

#include "maxim.term.driver.hpp"
#include "maxim.term.stream.hpp"

#include <chrono>
#include <unordered_map>

#include "maxim.term.input.unix.driver.hpp"

namespace maxim {
    namespace tui {

        struct driver::internals
        {

			// == input ==
			// key press state
			std::unordered_map < key_event_args, std::chrono::high_resolution_clock::time_point >
				key_states;

			termios initial_term_settings;
        };

        driver::driver() :
			_internals{std::make_unique<internals>()}
        {
			// setup term io settings
			termios initial_settings{};
			tcgetattr(0, &_internals->initial_term_settings);
			termios new_settings = _internals->initial_term_settings;

			new_settings.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INPCK | ISTRIP | IXON);
			new_settings.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
			new_settings.c_cflag
				&= ~(OPOST);
			new_settings.c_cflag |= CS8;
			new_settings.c_cc[VMIN] = 0;
			new_settings.c_cc[VTIME] = key_delay_ds;

			tcsetattr(0, TCSANOW, &new_settings);
			fcntl(STDIN_FILENO, F_SETFL, FNDELAY);
        }

        driver::~driver()
        {
	        // restore term io settingss
			fcntl(STDIN_FILENO, 0, FNDELAY);
			tcsetattr(0, TCSANOW, &_internals->initial_term_settings);
        }

        size driver::get_size() const {
            winsize w = {};
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

            return {
                    w.ws_col,
                    w.ws_row
            };
        }

        void driver::send (maxim::tui::stream & stream) const {
            ::write(STDOUT_FILENO, stream.data(), stream.size());
        }

		std::vector < input_event_info > driver::get_input_events() const {
			uint64_t v = 0;

			auto len = read(STDIN_FILENO, &v, sizeof(uint64_t));
			key_event_args info = parse_input(v, len);

			auto time = std::chrono::high_resolution_clock::now();
			// set or reset counter for key combination
			if (info.char_value != 0) {
				auto key_it = _internals->key_states.find(info);
				bool is_new_key = key_it == _internals->key_states.end();

				if (is_new_key) {
					_internals->key_states[info] = time;

					events.push_back(input_event_info{
						info.char_value,
						info.key,
						info.modifiers,
						true
						});
				}
				else {
					key_it->second = time;
				}
			}

			// check for time timeouts
			for (auto k : _internals->key_states)
			{
				auto time_span = std::chrono::duration_cast <std::chrono::milliseconds> (time - k.second).count();
				if (time_span > key_delay_ms){
					// remove
					events.push_back(input_event_info{
						info.char_value,
						info.key,
						info.modifiers,
						false
					});
				}
			}

			// remove from key state cache
			for (auto & e : events) {
				if (!e.is_down)
					_internals->key_states.erase({
							e.char_value,
							e.key,
							e.modifiers
						});
			}

            return events;
        }

    }
}

#endif