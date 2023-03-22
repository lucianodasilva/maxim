#include "maxim.input.unix.termio_parser.hpp"
#include "maxim.encoder.hpp"

namespace maxim::input::_unix {

#   define match_to(sequence, key) \
    if (source.match(sequence)) { \
        source += sequence_len (sequence); \
        return key; \
    }


    template < std::size_t ... _len_v >
    virtual_key match_special_keys_csi (span & source) {

        // larger sequences must come first

        // function keys
        match_to ("11", virtual_key::f1);
        match_to ("12", virtual_key::f2);
        match_to ("13", virtual_key::f3);
        match_to ("14", virtual_key::f4);
        match_to ("15", virtual_key::f5);
        match_to ("17", virtual_key::f6);
        match_to ("18", virtual_key::f7);
        match_to ("19", virtual_key::f8);
        match_to ("20", virtual_key::f9);
        match_to ("21", virtual_key::f10);
        match_to ("23", virtual_key::f11);
        match_to ("24", virtual_key::f12);

        // directional keys
        match_to ("A", virtual_key::up);
        match_to ("B", virtual_key::down);
        match_to ("C", virtual_key::right);
        match_to ("D", virtual_key::left);

        // six-key editing keypad
        match_to ("H", virtual_key::home);
        match_to ("1", virtual_key::home);

        match_to ("F", virtual_key::end);
        match_to ("4", virtual_key::end);

        match_to ("2", virtual_key::insert);
        match_to ("3", virtual_key::del);
        match_to ("5", virtual_key::page_up);
        match_to ("6", virtual_key::page_down);

        return virtual_key::none;
    }

    template < std::size_t ... _len_v >
    virtual_key match_special_keys_sse3 (span & source) {

        // larger sequences must come first

        // function keys
        match_to ("P", virtual_key::f1);
        match_to ("Q", virtual_key::f2);
        match_to ("R", virtual_key::f3);
        match_to ("S", virtual_key::f4);

        return virtual_key::none;
    }

    modifier_key map_modifiers(span & source) {

        match_to ("2", modifier_key::shift);
        match_to ("3", modifier_key::alt);
        match_to ("4", modifier_key::alt | modifier_key::shift);
        match_to ("5", modifier_key::control);
        match_to ("6", modifier_key::control | modifier_key::shift);
        match_to ("7", modifier_key::control | modifier_key::alt);
        match_to ("8", modifier_key::control | modifier_key::alt | modifier_key::shift);

        return modifier_key::none;
    }

#   undef match_to

    key_info termio_parse (span & source) {

        // check for special keys
        if (source.match (ansi::csi)) {
            auto next = source + sequence_len (ansi::csi);

            virtual_key key = match_special_keys_csi(next);
            modifier_key mod{modifier_key::none};

            // recognized key
            if (key != virtual_key::none) {

                // check for modifiers
                if (next.is(';')) {
                    next.step();
                    mod = map_modifiers(next);
                }

                next.ignore_until('~');
                source = next;

                return {
                        0,
                        key,
                        mod
                };
            }
        }

        if (source.match (ansi::sse3)) {
            auto next = source + sequence_len (ansi::csi);

            virtual_key key = match_special_keys_sse3 (next);
            modifier_key mod{modifier_key::none};

            // recognized key
            if (key != virtual_key::none) {
                source = next;

                return {
                    0,
                    key,
                    mod
                };
            }
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
            }

            return key;
        }
    }

}
