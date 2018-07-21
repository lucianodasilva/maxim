#ifndef _MAXIM_TUI_INPUT_HPP_
#define _MAXIM_TUI_INPUT_HPP_

#include "maxim.tui.event.hpp"

namespace maxim {
	namespace tui {

		// virtual key codes
		enum class virtual_key : uint8_t {
			none = 0x00,
			backspace = 0x08,
			tab = 0x09,

			clear = 0x0C,
			enter = 0x0D,

			escape = 0x1B,
			space = 0x20,

			page_up = 0x21,
			page_down = 0x22,

			end = 0x23,
			home = 0x24,

			left = 0x25,
			up = 0x26,
			right = 0x27,
			down = 0x28,

			insert = 0x2D,
			del = 0x2E,

			A = 0x41,
			B = 0x42,
			C = 0x43,
			D = 0x44,
			E = 0x45,
			F = 0x46,
			G = 0x47,
			H = 0x48,
			I = 0x49,
			J = 0x4A,
			K = 0x4B,
			L = 0x4C,
			M = 0x4D,
			N = 0x4E,
			O = 0x4F,
			P = 0x50,
			Q = 0x51,
			R = 0x52,
			S = 0x53,
			T = 0x54,
			U = 0x55,
			V = 0x56,
			W = 0x57,
			X = 0x58,
			Y = 0x59,
			Z = 0x5A,

			f1 = 0x70,
			f2 = 0x71,
			f3 = 0x72,
			f4 = 0x73,
			f5 = 0x74,
			f6 = 0x75,
			f7 = 0x76,
			f8 = 0x77,
			f9 = 0x78,
			f10 = 0x79,
			f11 = 0x80,
			f12 = 0x81
		};

		enum class modifier_key : uint8_t {
			none = 0x00,
			shift = 0x01,
			control = 0x02,
			alt = 0x04
		};

		inline modifier_key operator | (modifier_key v1, modifier_key v2) {
			using under_type = std::underlying_type_t < modifier_key >;

			return static_cast <modifier_key> (
				static_cast <under_type> (v1) |
				static_cast <under_type> (v2)
				);
		}

		inline modifier_key operator & (modifier_key v1, modifier_key v2) {
			using under_type = std::underlying_type_t < modifier_key >;

			return static_cast <modifier_key> (
				static_cast <under_type> (v1) |
				static_cast <under_type> (v2)
				);
		}

		struct key_event_args {
			char32_t		char_value;
			virtual_key		key;
			modifier_key	modifiers;
		};

		struct input_event_info {
			char32_t		char_value;
			virtual_key		key;
			modifier_key	modifiers;
			bool			is_down;
		};

		template < class _sender_t >
		using key_down_event = event < _sender_t, key_event_args >;

		template < class _sender_t >
		using key_up_event = event < _sender_t, key_event_args >;
	}
}

#endif