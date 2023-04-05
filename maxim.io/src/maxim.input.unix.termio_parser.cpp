#include "maxim.input.unix.termio_parser.hpp"
#include "maxim.details.hpp"
#include "maxim.encoder.hpp"

#define MATCH_TO(_sequence, _key) \
    if (string::begins_with (source, _sequence)) return (_key);

namespace maxim::input::unix {

    namespace {

        inline key key_code_lookup (char key_code) {
            constexpr key lookup[] = {
                    /* 0x00 */ key::none,
                    /* 0x01 */ key::a | key::control,
                    /* 0x02 */ key::b | key::control,
                    /* 0x03 */ key::c | key::control,
                    /* 0x04 */ key::d | key::control,
                    /* 0x05 */ key::e | key::control,
                    /* 0x06 */ key::f | key::control,
                    /* 0x07 */ key::g | key::control,
                    /* 0x08 */ key::backspace | key::control,
                    /* 0x09 */ key::tab,
                    /* 0x0A */ key::j | key::control,
                    /* 0x0B */ key::k | key::control,
                    /* 0x0C */ key::l | key::control,
                    /* 0x0D */ key::enter,
                    /* 0x0E */ key::n | key::control,
                    /* 0x0F */ key::o | key::control,
                    /* 0x10 */ key::p | key::control,
                    /* 0x11 */ key::q | key::control,
                    /* 0x12 */ key::r | key::control,
                    /* 0x13 */ key::s | key::control,
                    /* 0x14 */ key::t | key::control,
                    /* 0x15 */ key::u | key::control,
                    /* 0x16 */ key::v | key::control,
                    /* 0x17 */ key::w | key::control,
                    /* 0x18 */ key::x | key::control,
                    /* 0x19 */ key::y | key::control,
                    /* 0x1A */ key::z | key::control,
                    /* 0x1B */ key::escape,
                    /* 0x1C */ key::none,
                    /* 0x1D */ key::none,
                    /* 0x1E */ key::none,
                    /* 0x1F */ key::none,
                    /* 0x20 */ key::space,
                    /* 0x21 */ key::shift,
                    /* 0x22 */ key::shift,
                    /* 0x23 */ key::shift,
                    /* 0x24 */ key::shift,
                    /* 0x25 */ key::shift,
                    /* 0x26 */ key::shift,
                    /* 0x27 */ key::shift,
                    /* 0x28 */ key::shift,
                    /* 0x29 */ key::shift,
                    /* 0x2A */ key::none,
                    /* 0x2B */ key::none,
                    /* 0x2C */ key::none,
                    /* 0x2D */ key::none,
                    /* 0x2E */ key::none,
                    /* 0x2F */ key::none,
                    /* 0x30 */ key::n0,
                    /* 0x31 */ key::n1,
                    /* 0x32 */ key::n2,
                    /* 0x33 */ key::n3,
                    /* 0x34 */ key::n4,
                    /* 0x35 */ key::n5,
                    /* 0x36 */ key::n6,
                    /* 0x37 */ key::n7,
                    /* 0x38 */ key::n8,
                    /* 0x39 */ key::n9,
                    /* 0x3A */ key::none,
                    /* 0x3B */ key::none,
                    /* 0x3C */ key::none,
                    /* 0x3D */ key::shift,
                    /* 0x3E */ key::none,
                    /* 0x3F */ key::none,
                    /* 0x40 */ key::none,
                    /* 0x41 */ key::a | key::shift,
                    /* 0x42 */ key::b | key::shift,
                    /* 0x43 */ key::c | key::shift,
                    /* 0x44 */ key::d | key::shift,
                    /* 0x45 */ key::e | key::shift,
                    /* 0x46 */ key::f | key::shift,
                    /* 0x47 */ key::g | key::shift,
                    /* 0x48 */ key::h | key::shift,
                    /* 0x49 */ key::i | key::shift,
                    /* 0x4A */ key::j | key::shift,
                    /* 0x4B */ key::k | key::shift,
                    /* 0x4C */ key::l | key::shift,
                    /* 0x4D */ key::m | key::shift,
                    /* 0x4E */ key::n | key::shift,
                    /* 0x4F */ key::o | key::shift,
                    /* 0x50 */ key::p | key::shift,
                    /* 0x51 */ key::q | key::shift,
                    /* 0x52 */ key::r | key::shift,
                    /* 0x53 */ key::s | key::shift,
                    /* 0x54 */ key::t | key::shift,
                    /* 0x55 */ key::u | key::shift,
                    /* 0x56 */ key::v | key::shift,
                    /* 0x57 */ key::w | key::shift,
                    /* 0x58 */ key::x | key::shift,
                    /* 0x59 */ key::y | key::shift,
                    /* 0x5A */ key::z | key::shift,
                    /* 0x5B */ key::none,
                    /* 0x5C */ key::none,
                    /* 0x5D */ key::none,
                    /* 0x5E */ key::none,
                    /* 0x5F */ key::none,
                    /* 0x60 */ key::none,
                    /* 0x61 */ key::a,
                    /* 0x62 */ key::b,
                    /* 0x63 */ key::c,
                    /* 0x64 */ key::d,
                    /* 0x65 */ key::e,
                    /* 0x66 */ key::f,
                    /* 0x67 */ key::g,
                    /* 0x68 */ key::h,
                    /* 0x69 */ key::i,
                    /* 0x6A */ key::j,
                    /* 0x6B */ key::k,
                    /* 0x6C */ key::l,
                    /* 0x6D */ key::m,
                    /* 0x6E */ key::n,
                    /* 0x6F */ key::o,
                    /* 0x70 */ key::p,
                    /* 0x71 */ key::q,
                    /* 0x72 */ key::r,
                    /* 0x73 */ key::s,
                    /* 0x74 */ key::t,
                    /* 0x75 */ key::u,
                    /* 0x76 */ key::v,
                    /* 0x77 */ key::w,
                    /* 0x78 */ key::x,
                    /* 0x79 */ key::y,
                    /* 0x7A */ key::z,
                    /* 0x7B */ key::none,
                    /* 0x7C */ key::none,
                    /* 0x7D */ key::none,
                    /* 0x7E */ key::none,
                    /* 0x7F */ key::backspace,
            };

            if (key_code < 0) {
                return key::none;
            }

            return lookup[key_code];
        }

        inline bool consume (std::string_view & view, std::string_view sequence) {
            if (view.empty () != true &&
                string::begins_with(view, sequence)
            ){
                view = view.substr (sequence.size ());
                return true;
            }

            return false;
        }

        key parse_csi_special_keys (std::string_view source) {
            
            MATCH_TO ("11", key::f1);
            MATCH_TO ("12", key::f2);
            MATCH_TO ("13", key::f3);
            MATCH_TO ("14", key::f4);
            MATCH_TO ("15", key::f5);
            MATCH_TO ("17", key::f6);
            MATCH_TO ("18", key::f7);
            MATCH_TO ("19", key::f8);
            MATCH_TO ("20", key::f9);
            MATCH_TO ("21", key::f10);
            MATCH_TO ("23", key::f11);
            MATCH_TO ("24", key::f12);

            MATCH_TO ("1P", key::f1);
            MATCH_TO ("1Q", key::f2);
            MATCH_TO ("1R", key::f3);
            MATCH_TO ("1S", key::f4);

            // directional keys
            MATCH_TO ("A", key::up);
            MATCH_TO ("B", key::down);
            MATCH_TO ("C", key::right);
            MATCH_TO ("D", key::left);

            // six-key editing keypad
            MATCH_TO ("H", key::home);
            MATCH_TO ("1", key::home);

            MATCH_TO ("F", key::end);
            MATCH_TO ("4", key::end);

            MATCH_TO ("2", key::insert);
            MATCH_TO ("3", key::del);
            MATCH_TO ("5", key::page_up);
            MATCH_TO ("6", key::page_down);

            return key::none;
        }

        key parse_modifiers (char mod_code) {
            switch (mod_code) {
                case '1': return key::shift;
                case '2': return key::alt;
                case '3': return key::alt | key::shift;
                case '4': return key::control;
                case '5': return key::control | key::shift;
                case '6': return key::control | key::alt;
                case '7': return key::control | key::alt | key::shift;
                default:
                    return key::none;
            }
        }
    }

    key_info termio_parse(std::string_view source) {

        // single char key
        if (source.size () == 1) {
            return { source.front(), key_code_lookup(source.front ()) };
        }

        // char preceded by escape
        if (source.size () == 2) {
            if (source == "\33\33") {
                return { '\33', key::escape };
            }

            return { '[', key_code_lookup (source.back ()) | key::alt };
        }

        // csi prefixed sequence
        if (consume (source, ansi::CSI)) {

            // no further key info
            if (source.empty()) {
                return { '[', key::alt };
            }

            // modified text
            if (source.size () == 2 && source.back () != '~') {
                return {
                    source.back (),
                    key_code_lookup (source.back ()) | parse_modifiers (source.front ()) };
            }

            // special keys
            if (source.back () == '~') {
                char mod_code = '0';
                std::string_view key_code;

                auto mod_sep_index = source.find (ansi::SEPARATOR);

                // if no separator found, assume no modifiers
                if (mod_sep_index == std::string_view::npos) {
                    // remove terminator
                    key_code = source.substr (0, source.size () - 1);
                } else {
                    key_code = source.substr (0, mod_sep_index);
                    mod_code = source[mod_sep_index];
                }

                return { '\0', parse_csi_special_keys (key_code) | parse_modifiers (mod_code) };
            }
        }

        // unrecognized key sequence
        return { '\0', key::none };
    }

}

#   undef MATCH_TO
