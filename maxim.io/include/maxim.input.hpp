#pragma once
#ifndef _maxim_input_hpp_
#define _maxim_input_hpp_

#include <cinttypes>
#include <type_traits>
#include <unordered_map>

namespace maxim::input {
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
        none    = 0b0000,
        shift   = 0b0001,
        control = 0b0010,
        alt     = 0b0100
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
            static_cast <under_type> (v1) &
            static_cast <under_type> (v2)
        );
    }

	enum class key_match_policy {
		match_all,
		ignore_modifier,
	};

    struct key_info {
        char32_t		char_value;
        virtual_key		key;
        modifier_key	modifiers;

        inline explicit operator bool () noexcept {
            return char_value || key != virtual_key::none;
        }

	    inline bool matches (virtual_key const KEY, modifier_key const MODIFIERS) const noexcept {
		    return this->key == KEY && this->modifiers == MODIFIERS;
	    }

		inline bool matches (virtual_key KEY, key_match_policy const POLICY = key_match_policy::match_all) const noexcept {
			if (POLICY == key_match_policy::ignore_modifier) {
				return this->key == KEY;
			}

			return matches (KEY, modifier_key::none);
		}
    };

    inline bool operator==(key_info const &v1, key_info const &v2) noexcept {
        return
            v1.char_value == v2.char_value &&
            v1.key == v2.key &&
            v1.modifiers == v2.modifiers;
    }

    // hasher and equality operator for key_event_args
    namespace details {

        // hash combination util
        template<class ... hash_v_tv>
        std::size_t hash_combine(hash_v_tv const &... values) {
            std::size_t h = 0;
            // sink
            using sink = int[];
            (void) sink{
                0, ((h ^= std::hash<hash_v_tv>()(values) + 0x9e3779b9 + (h << 6) + (h >> 2)), 0)...
            };

            return h;
        }

    }
}

namespace std
{
	template <>
	struct hash <maxim::input::key_info>
	{
		std::size_t operator ()(maxim::input::key_info const & info) const noexcept{
			return maxim::input::details::hash_combine(
				info.char_value,
				static_cast < std::underlying_type_t <maxim::input::virtual_key> > (info.key),
				static_cast < std::underlying_type_t <maxim::input::modifier_key> > (info.modifiers)
			);
		}
	};
}

#endif