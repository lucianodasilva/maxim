#include "maxim.input.unix.termio_parser.hpp"
#include "maxim.encoder.hpp"

#include <map>
#include <string>

namespace maxim::input::_unix {

	modifier_key map_modifiers(char ch) {
		switch (ch) {
			case '2': return modifier_key::shift;
			case '3': return modifier_key::alt;
			case '4': return modifier_key::alt | modifier_key::shift;
			case '5': return modifier_key::control;
			case '6': return modifier_key::control | modifier_key::shift;
			case '7': return modifier_key::control | modifier_key::alt;
			case '8': return modifier_key::control | modifier_key::alt | modifier_key::shift;
			default:
				return modifier_key::none;
		}
	}

	key_info match_special_keys_csi (span source) {
		static std::map < std::string, key_info > const csi_key_map {
			// Function keys
			{ "P", { 0, virtual_key::f1, modifier_key::none } },
			{ "Q", { 0, virtual_key::f2, modifier_key::none } },
			{ "R", { 0, virtual_key::f3, modifier_key::none } },
			{ "S", { 0, virtual_key::f4, modifier_key::none } },
			{ "15", { 0, virtual_key::f5, modifier_key::none } },
			{ "17", { 0, virtual_key::f6, modifier_key::none } },
			{ "18", { 0, virtual_key::f7, modifier_key::none } },
			{ "19", { 0, virtual_key::f8, modifier_key::none } },
			{ "20", { 0, virtual_key::f9, modifier_key::none } },
			{ "21", { 0, virtual_key::f10, modifier_key::none } },
			{ "23", { 0, virtual_key::f11, modifier_key::none } },
			{ "24", { 0, virtual_key::f12, modifier_key::none } },
			// cursor keys
			{ "A", { 0, virtual_key::up, modifier_key::none } },
			{ "B", { 0, virtual_key::down, modifier_key::none } },
			{ "C", { 0, virtual_key::right, modifier_key::none } },
			{ "D", { 0, virtual_key::left, modifier_key::none } },
			// 6 key editing keyboard
			{ "H", { 0, virtual_key::home, modifier_key::none } },
			{ "1", { 0, virtual_key::home, modifier_key::none } },

			{ "F", { 0, virtual_key::end, modifier_key::none } },
			{ "4", { 0, virtual_key::end, modifier_key::none } },

			{ "2", { 0, virtual_key::insert, modifier_key::none } },
			{ "3", { 0, virtual_key::del, modifier_key::none } },
			{ "5", { 0, virtual_key::page_up, modifier_key::none } },
			{ "6", { 0, virtual_key::page_down, modifier_key::none } },

			// others
			{ "Z", { 0, virtual_key::tab, modifier_key::shift } },
		};

		key_info        info {};
		modifier_key    mod {};

		// ignore control characters
		source = source.advance(sizeof(ansi::csi) - 1);

		// check for tilde
		bool is_extended = source.find ('~') != source.end;

		// check for separator
		auto sep_it = source.find (';');
		std::string search_str;

		// search for key
		if (is_extended) {
			if (sep_it != source.end) {
				search_str = std::string { source.begin, sep_it };
				mod = map_modifiers(*(sep_it + 1));
			} else {
				search_str = std::string { source.begin, source.end - 1 };
			}
		} else {
			if (sep_it != source.end) {
				search_str = std::string { sep_it + 2, source.end };
				mod = map_modifiers(*(sep_it + 1));
			} else {
				search_str = std::string { source.begin, source.end };
			}
		}

		auto it = csi_key_map.find(search_str);

		if (it != csi_key_map.end()) {
			info = it->second;
		}

		if (mod != modifier_key::none) {
			info.modifiers = mod;
		}

		return info;
	}

	key_info match_special_keys_ss3 (span source) {
		static std::map < std::string, key_info > const ss3_key_map {
			// Function keys
			{ "\033OP", { 0, virtual_key::f1, modifier_key::none } },
			{ "\033OQ", { 0, virtual_key::f2, modifier_key::none } },
			{ "\033OR", { 0, virtual_key::f3, modifier_key::none } },
			{ "\033OS", { 0, virtual_key::f4, modifier_key::none } },
		};

		key_info info {};

		auto it = ss3_key_map.find({source.begin, source.end});

		if (it != ss3_key_map.end()) {
			info = it->second;
		}

		return info;
	}

    key_info termio_parse (span source) {

        // check for special keys
        if (source.match (ansi::csi)) {
            return match_special_keys_csi (source);
        }

        if (source.match (ansi::ss3)) {
			return match_special_keys_ss3(source);
        }

        { // remaining
            auto key_char = *source.begin;
            bool prefixed = key_char == '\033';
            key_info key {};

            source.step();

            if (prefixed && source) {
                key = key_map[*source.begin];
                key.modifiers = key.modifiers | modifier_key::alt;

                source.step();
            } else {
                key = key_map[key_char];
	            key.char_value = key_char;
            }

            return key;
        }
    }

}
