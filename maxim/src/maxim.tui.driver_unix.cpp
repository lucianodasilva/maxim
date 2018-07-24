#ifdef __linux__

#include <termio.h>
#include <termios.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>

#include "maxim.tui.driver.hpp"
#include "maxim.tui.stream.hpp"

#include <chrono>
#include <unordered_map>

#include "maxim.tui.driver_unix.hpp"

namespace maxim {
    namespace tui {

		virtual_key map_special_key_code(uint16_t key_code) {
			switch (key_code) {
				case 0x32:
					return virtual_key::insert;
				case 0x33:
					return virtual_key::del;
				case 0x35:
					return virtual_key::page_up;
				case 0x36:
					return virtual_key::page_down;
				case 0x41:
					return virtual_key::up;
				case 0x42:
					return virtual_key::down;
				case 0x43:
					return virtual_key::right;
				case 0x44:
					return virtual_key::left;
				case 0x46:
					return virtual_key::end;
				case 0x48:
					return virtual_key::home;
				case 0x50:
					return virtual_key::f1;
				case 0x51:
					return virtual_key::f2;
				case 0x52:
					return virtual_key::f3;
				case 0x53:
					return virtual_key::f4;
				case 0x3531:
					return virtual_key::f5;
				case 0x3731:
					return virtual_key::f6;
				case 0x3831:
					return virtual_key::f7;
				case 0x3931:
					return virtual_key::f8;
				case 0x3032:
					return virtual_key::f9;
				case 0x3132:
					return virtual_key::f10;
				case 0x3332:
					return virtual_key::f11;
				case 0x3432:
					return virtual_key::f12;
				default:
					return virtual_key::none;
			}
		}

		modifier_key map_modifiers(uint8_t m) {
			switch (m) {
				case 0x32:
					return modifier_key::shift;
				case 0x33:
					return modifier_key::alt;
				case 0x34:
					return modifier_key::shift | modifier_key::alt;
				case 0x35:
					return modifier_key::control;
				case 0x36:
					return modifier_key::shift | modifier_key::control;
				case 0x37:
					return modifier_key::control | modifier_key::alt;
				case 0x38:
					return modifier_key::control | modifier_key::alt | modifier_key::shift;
				default:
					return modifier_key::none;
			}
		}

		key_event_args parse_input(uint64_t input, int len) {
			auto * buffer = reinterpret_cast < uint8_t * > (&input);
			if (len <= 0)
				return {};

			// regular keys
			if (len == 1 && buffer[0] < 0x80) {
				auto map_item = key_map[buffer[0]];

				return {
					*reinterpret_cast <char32_t *> (+buffer),
					map_item.key,
					map_item.mod
				};
			}
			else {
				// regular keys + mod
				if (len == 2 && buffer[0] == '\x1B') {
					return {
						*reinterpret_cast <char32_t *> (1 + buffer),
						key_map[buffer[1]].key,
						key_map[buffer[1]].mod | modifier_key::alt
					};
				}
				else if (buffer[0] == '\x1B') {
					uint16_t key_code = 0;
					modifier_key alt = modifier_key::none;

					// simple special keys
					if (len < 5) {
						key_code = buffer[2];
					}
					else if (len == 5) {
						key_code = *reinterpret_cast <uint16_t *> (buffer + 2);
					}
					// special key with modifier combinations
					else if (len == 6) {
						if (buffer[5] == '\x7e') {
							key_code = buffer[2];
						}
						else {
							key_code = buffer[5];
						}
						alt = map_modifiers(buffer[4]);
					}
					else if (len == 7) {
						key_code = *reinterpret_cast <uint16_t *> (buffer + 2);
						alt = map_modifiers(buffer[5]);
					}

					return { 0x00, map_special_key_code(key_code), alt };
				}
			}
		}
		
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
			std::vector < input_event_info > events;
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