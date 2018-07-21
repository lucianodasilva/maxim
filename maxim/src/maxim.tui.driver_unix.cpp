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

constexpr uint32_t key_delay_ms = 150;
constexpr uint32_t key_delay_ds = key_delay_ms / 100;

// hash commbination util
template < class ... hash_v_tv >
std::size_t hash_combine (hash_v_tv const & ... values)
{
	std::size_t h = 0;
	// sink
	using sink = int[];
	(void)sink {
		0, ((h ^= std::hash < hash_v_tv >()(values) + 0x9e3779b9 + (h << 6) + (h >> 2)), 0)...
	};

	return h;
}

// hasher and equality operator for key_event_args
namespace maxim
{
	namespace tui
	{
		inline bool operator == (key_event_args const & v1, key_event_args const & v2)
		{
			return
				v1.char_value == v2.char_value &&
				v1.key == v2.key &&
				v1.modifiers == v2.modifiers;
		}
	}
}

namespace std
{
	template <>
	struct hash <maxim::tui::key_event_args>
	{
		std::size_t operator ()(maxim::tui::key_event_args const & args) const noexcept{
			return hash_combine(
				args.char_value,
				static_cast < std::underlying_type_t <maxim::tui::virtual_key> > (args.key),
				static_cast < std::underlying_type_t <maxim::tui::modifier_key> > (args.modifiers)
			);
		}
	};
}

namespace maxim {
    namespace tui {

		struct key_map_item {
			virtual_key		key;
			modifier_key	alt;
		};

		key_map_item key_map[] = {
			{}, // 0x00
			{ virtual_key::A, modifier_key::control }, // 0x01
			{ virtual_key::B, modifier_key::control }, // 0x02
			{ virtual_key::C, modifier_key::control }, // 0x03
			{ virtual_key::D, modifier_key::control }, // 0x04
			{ virtual_key::E, modifier_key::control }, // 0x05
			{ virtual_key::F, modifier_key::control }, // 0x06
			{ virtual_key::G, modifier_key::control }, // 0x07
			{ virtual_key::backspace, modifier_key::control }, // 0x08
			{ virtual_key::tab, modifier_key::none }, // 0x09
			{ virtual_key::J, modifier_key::control }, // 0x0A
			{ virtual_key::K, modifier_key::control }, // 0x0B
			{ virtual_key::L, modifier_key::control }, // 0x0C
			{ virtual_key::enter, modifier_key::control }, // 0x0D
			{ virtual_key::N, modifier_key::control }, // 0x0E
			{ virtual_key::O, modifier_key::control }, // 0x0F
			{ virtual_key::P, modifier_key::control }, // 0x10
			{ virtual_key::Q, modifier_key::control }, // 0x11
			{ virtual_key::R, modifier_key::control }, // 0x12
			{ virtual_key::S, modifier_key::control }, // 0x13
			{ virtual_key::T, modifier_key::control }, // 0x14
			{ virtual_key::U, modifier_key::control }, // 0x15
			{ virtual_key::V, modifier_key::control }, // 0x16
			{ virtual_key::W, modifier_key::control }, // 0x17
			{ virtual_key::X, modifier_key::control }, // 0x18
			{ virtual_key::Y, modifier_key::control }, // 0x19
			{ virtual_key::Z, modifier_key::control }, // 0x1A
			{ virtual_key::escape, modifier_key::none }, // 0x1B
			{}, // 0x1C
			{}, // 0x1D
			{}, // 0x1E
			{}, // 0x1F
			{ virtual_key::space, modifier_key::none }, // 0x20
			{ virtual_key::none, modifier_key::shift }, // 0x21
			{ virtual_key::none, modifier_key::shift }, // 0x22
			{ virtual_key::none, modifier_key::shift }, // 0x23
			{ virtual_key::none, modifier_key::shift }, // 0x24
			{ virtual_key::none, modifier_key::shift }, // 0x25
			{ virtual_key::none, modifier_key::shift }, // 0x26
			{ virtual_key::none, modifier_key::shift }, // 0x27
			{ virtual_key::none, modifier_key::shift }, // 0x28
			{ virtual_key::none, modifier_key::shift }, // 0x29
			{}, // 0x2A
			{}, // 0x2B
			{}, // 0x2C
			{}, // 0x2D
			{}, // 0x2E
			{}, // 0x2F
			{}, // 0x30
			{}, // 0x31
			{}, // 0x32
			{}, // 0x33
			{}, // 0x34
			{}, // 0x35
			{}, // 0x36
			{}, // 0x37
			{}, // 0x38
			{}, // 0x39
			{}, // 0x3A
			{}, // 0x3B
			{}, // 0x3C
			{ virtual_key::none, modifier_key::shift }, // 0x3D
			{}, // 0x3E
			{}, // 0x3F
			{}, // 0x40
			{ virtual_key::A, modifier_key::shift }, // 0x41
			{ virtual_key::B, modifier_key::shift }, // 0x42
			{ virtual_key::C, modifier_key::shift }, // 0x43
			{ virtual_key::D, modifier_key::shift }, // 0x44
			{ virtual_key::E, modifier_key::shift }, // 0x45
			{ virtual_key::F, modifier_key::shift }, // 0x46
			{ virtual_key::G, modifier_key::shift }, // 0x47
			{ virtual_key::H, modifier_key::shift }, // 0x48
			{ virtual_key::I, modifier_key::shift }, // 0x49
			{ virtual_key::J, modifier_key::shift }, // 0x4A
			{ virtual_key::K, modifier_key::shift }, // 0x4B
			{ virtual_key::L, modifier_key::shift }, // 0x4C
			{ virtual_key::M, modifier_key::shift }, // 0x4D
			{ virtual_key::N, modifier_key::shift }, // 0x4E
			{ virtual_key::O, modifier_key::shift }, // 0x4F
			{ virtual_key::P, modifier_key::shift }, // 0x50
			{ virtual_key::Q, modifier_key::shift }, // 0x51
			{ virtual_key::R, modifier_key::shift }, // 0x52
			{ virtual_key::S, modifier_key::shift }, // 0x53
			{ virtual_key::T, modifier_key::shift }, // 0x54
			{ virtual_key::U, modifier_key::shift }, // 0x55
			{ virtual_key::V, modifier_key::shift }, // 0x56
			{ virtual_key::W, modifier_key::shift }, // 0x57
			{ virtual_key::X, modifier_key::shift }, // 0x58
			{ virtual_key::Y, modifier_key::shift }, // 0x59
			{ virtual_key::Z, modifier_key::shift }, // 0x5A
			{}, // 0x5B
			{}, // 0x5C
			{}, // 0x5D
			{}, // 0x5E
			{}, // 0x5F
			{}, // 0x60
			{ virtual_key::A, modifier_key::none }, // 0x61
			{ virtual_key::B, modifier_key::none }, // 0x62
			{ virtual_key::C, modifier_key::none }, // 0x63
			{ virtual_key::D, modifier_key::none }, // 0x64
			{ virtual_key::E, modifier_key::none }, // 0x65
			{ virtual_key::F, modifier_key::none }, // 0x66
			{ virtual_key::G, modifier_key::none }, // 0x67
			{ virtual_key::H, modifier_key::none }, // 0x68
			{ virtual_key::I, modifier_key::none }, // 0x69
			{ virtual_key::J, modifier_key::none }, // 0x6A
			{ virtual_key::K, modifier_key::none }, // 0x6B
			{ virtual_key::L, modifier_key::none }, // 0x6C
			{ virtual_key::M, modifier_key::none }, // 0x6D
			{ virtual_key::N, modifier_key::none }, // 0x6E
			{ virtual_key::O, modifier_key::none }, // 0x6F
			{ virtual_key::P, modifier_key::none }, // 0x70
			{ virtual_key::Q, modifier_key::none }, // 0x71
			{ virtual_key::R, modifier_key::none }, // 0x72
			{ virtual_key::S, modifier_key::none }, // 0x73
			{ virtual_key::T, modifier_key::none }, // 0x74
			{ virtual_key::U, modifier_key::none }, // 0x75
			{ virtual_key::V, modifier_key::none }, // 0x76
			{ virtual_key::W, modifier_key::none }, // 0x77
			{ virtual_key::X, modifier_key::none }, // 0x78
			{ virtual_key::Y, modifier_key::none }, // 0x79
			{ virtual_key::Z, modifier_key::none }, // 0x7A
			{}, // 0x7B
			{}, // 0x7C
			{}, // 0x7D
			{}, // 0x7E
			{ virtual_key::backspace, modifier_key::none } // 0x7F
		};

		virtual_key map_special_key_code(uint16_t key_code) {
			switch (key_code) {
			case 32:
				return virtual_key::insert;
			case 33:
				return virtual_key::del;
			case 35:
				return virtual_key::page_up;
			case 36:
				return virtual_key::page_down;
			case 41:
				return virtual_key::up;
			case 42:
				return virtual_key::down;
			case 43:
				return virtual_key::right;
			case 44:
				return virtual_key::left;
			case 46:
				return virtual_key::end;
			case 48:
				return virtual_key::home;
			case 50:
				return virtual_key::f1;
			case 51:
				return virtual_key::f2;
			case 52:
				return virtual_key::f3;
			case 53:
				return virtual_key::f4;
			case 3135:
				return virtual_key::f5;
			case 3137:
				return virtual_key::f6;
			case 3138:
				return virtual_key::f7;
			case 3139:
				return virtual_key::f8;
			case 3230:
				return virtual_key::f9;
			case 3231:
				return virtual_key::f10;
			case 3233:
				return virtual_key::f11;
			case 3234:
				return virtual_key::f12;
			default:
				return virtual_key::none;
			}
		}

		modifier_key map_modifiers(uint8_t m) {
			switch (m) {
			case 32:
				return modifier_key::shift;
			case 33:
				return modifier_key::alt;
			case 34:
				return modifier_key::shift | modifier_key::alt;
			case 35:
				return modifier_key::control;
			case 36:
				return modifier_key::shift | modifier_key::control;
			case 37:
				return modifier_key::control | modifier_key::alt;
			case 38:
				return modifier_key::control | modifier_key::alt | modifier_key::shift;
			default:
				return modifier_key::none;
			}
		}

		key_event_args parse_input(uint64_t input, int len) {
			auto * buffer = reinterpret_cast < uint8_t * > (&input);
			if (len == 0)
				return {};

			// regular keys
			if (len == 1 && buffer[0] < 0x80) {
				return {
					*reinterpret_cast <char32_t *> (+buffer),
					key_map[buffer[0]].key,
					key_map[buffer[0]].alt
				};
			}
			else {
				// regular keys + alt
				if (len == 2 && buffer[0] == '\x1B') {
					return {
						*reinterpret_cast <char32_t *> (1 + buffer),
						key_map[buffer[1]].key,
						key_map[buffer[1]].alt | modifier_key::alt
					};
				}
				else if (buffer[0] == 0x1B) {
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
						if (buffer[5] == 0x7e) {
							key_code = buffer[2];
						}
						else {
							key_code = buffer[5];
						}
						alt = map_modifiers(buffer[4]);
					}
					else if (len == 7) {
						key_code = *reinterpret_cast <uint16_t *> (buffer + 2);
						alt = map_modifiers(buffer[6]);
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

        std::vector < input_event_info > driver::get_input_events () const {
			std::vector < input_event_info > events;
			uint64_t v = 0;

			auto len = read(STDIN_FILENO, &v, sizeof(uint64_t));
			key_event_args info = parse_input(v, len);

			// set or reset counter for key combination
			auto time = std::chrono::high_resolution_clock::now();
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
			} else {
				key_it->second = time;
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