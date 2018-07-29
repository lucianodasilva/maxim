#include "maxim.input.unix.termio_parser.hpp"

namespace maxim {
    namespace input {
        namespace _unix {

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

            key_event_args parse_termio_input(uint64_t input, int len) {
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
        }
    }
}
