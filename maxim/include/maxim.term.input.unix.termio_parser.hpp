#pragma once
#ifndef _maxim_term_input_unix_termio_parser_hpp_
#define _maxim_term_input_unix_termio_parser_hpp_

#include "maxim.term.input.hpp"

namespace maxim {
    namespace term {
        namespace input {
            namespace _unix {

                struct key_map_item {
                    virtual_key key;
                    modifier_key mod;
                };

                constexpr key_map_item key_map[] = {
                    {}, // 0x00
                    {virtual_key::A,         modifier_key::control}, // 0x01
                    {virtual_key::B,         modifier_key::control}, // 0x02
                    {virtual_key::C,         modifier_key::control}, // 0x03
                    {virtual_key::D,         modifier_key::control}, // 0x04
                    {virtual_key::E,         modifier_key::control}, // 0x05
                    {virtual_key::F,         modifier_key::control}, // 0x06
                    {virtual_key::G,         modifier_key::control}, // 0x07
                    {virtual_key::backspace, modifier_key::control}, // 0x08
                    {virtual_key::tab,       modifier_key::none}, // 0x09
                    {virtual_key::J,         modifier_key::control}, // 0x0A
                    {virtual_key::K,         modifier_key::control}, // 0x0B
                    {virtual_key::L,         modifier_key::control}, // 0x0C
                    {virtual_key::enter,     modifier_key::control}, // 0x0D
                    {virtual_key::N,         modifier_key::control}, // 0x0E
                    {virtual_key::O,         modifier_key::control}, // 0x0F
                    {virtual_key::P,         modifier_key::control}, // 0x10
                    {virtual_key::Q,         modifier_key::control}, // 0x11
                    {virtual_key::R,         modifier_key::control}, // 0x12
                    {virtual_key::S,         modifier_key::control}, // 0x13
                    {virtual_key::T,         modifier_key::control}, // 0x14
                    {virtual_key::U,         modifier_key::control}, // 0x15
                    {virtual_key::V,         modifier_key::control}, // 0x16
                    {virtual_key::W,         modifier_key::control}, // 0x17
                    {virtual_key::X,         modifier_key::control}, // 0x18
                    {virtual_key::Y,         modifier_key::control}, // 0x19
                    {virtual_key::Z,         modifier_key::control}, // 0x1A
                    {virtual_key::escape,    modifier_key::none}, // 0x1B
                    {}, // 0x1C
                    {}, // 0x1D
                    {}, // 0x1E
                    {}, // 0x1F
                    {virtual_key::space,     modifier_key::none}, // 0x20
                    {virtual_key::none,      modifier_key::shift}, // 0x21
                    {virtual_key::none,      modifier_key::shift}, // 0x22
                    {virtual_key::none,      modifier_key::shift}, // 0x23
                    {virtual_key::none,      modifier_key::shift}, // 0x24
                    {virtual_key::none,      modifier_key::shift}, // 0x25
                    {virtual_key::none,      modifier_key::shift}, // 0x26
                    {virtual_key::none,      modifier_key::shift}, // 0x27
                    {virtual_key::none,      modifier_key::shift}, // 0x28
                    {virtual_key::none,      modifier_key::shift}, // 0x29
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
                    {virtual_key::none,      modifier_key::shift}, // 0x3D
                    {}, // 0x3E
                    {}, // 0x3F
                    {}, // 0x40
                    {virtual_key::A,         modifier_key::shift}, // 0x41
                    {virtual_key::B,         modifier_key::shift}, // 0x42
                    {virtual_key::C,         modifier_key::shift}, // 0x43
                    {virtual_key::D,         modifier_key::shift}, // 0x44
                    {virtual_key::E,         modifier_key::shift}, // 0x45
                    {virtual_key::F,         modifier_key::shift}, // 0x46
                    {virtual_key::G,         modifier_key::shift}, // 0x47
                    {virtual_key::H,         modifier_key::shift}, // 0x48
                    {virtual_key::I,         modifier_key::shift}, // 0x49
                    {virtual_key::J,         modifier_key::shift}, // 0x4A
                    {virtual_key::K,         modifier_key::shift}, // 0x4B
                    {virtual_key::L,         modifier_key::shift}, // 0x4C
                    {virtual_key::M,         modifier_key::shift}, // 0x4D
                    {virtual_key::N,         modifier_key::shift}, // 0x4E
                    {virtual_key::O,         modifier_key::shift}, // 0x4F
                    {virtual_key::P,         modifier_key::shift}, // 0x50
                    {virtual_key::Q,         modifier_key::shift}, // 0x51
                    {virtual_key::R,         modifier_key::shift}, // 0x52
                    {virtual_key::S,         modifier_key::shift}, // 0x53
                    {virtual_key::T,         modifier_key::shift}, // 0x54
                    {virtual_key::U,         modifier_key::shift}, // 0x55
                    {virtual_key::V,         modifier_key::shift}, // 0x56
                    {virtual_key::W,         modifier_key::shift}, // 0x57
                    {virtual_key::X,         modifier_key::shift}, // 0x58
                    {virtual_key::Y,         modifier_key::shift}, // 0x59
                    {virtual_key::Z,         modifier_key::shift}, // 0x5A
                    {}, // 0x5B
                    {}, // 0x5C
                    {}, // 0x5D
                    {}, // 0x5E
                    {}, // 0x5F
                    {}, // 0x60
                    {virtual_key::A,         modifier_key::none}, // 0x61
                    {virtual_key::B,         modifier_key::none}, // 0x62
                    {virtual_key::C,         modifier_key::none}, // 0x63
                    {virtual_key::D,         modifier_key::none}, // 0x64
                    {virtual_key::E,         modifier_key::none}, // 0x65
                    {virtual_key::F,         modifier_key::none}, // 0x66
                    {virtual_key::G,         modifier_key::none}, // 0x67
                    {virtual_key::H,         modifier_key::none}, // 0x68
                    {virtual_key::I,         modifier_key::none}, // 0x69
                    {virtual_key::J,         modifier_key::none}, // 0x6A
                    {virtual_key::K,         modifier_key::none}, // 0x6B
                    {virtual_key::L,         modifier_key::none}, // 0x6C
                    {virtual_key::M,         modifier_key::none}, // 0x6D
                    {virtual_key::N,         modifier_key::none}, // 0x6E
                    {virtual_key::O,         modifier_key::none}, // 0x6F
                    {virtual_key::P,         modifier_key::none}, // 0x70
                    {virtual_key::Q,         modifier_key::none}, // 0x71
                    {virtual_key::R,         modifier_key::none}, // 0x72
                    {virtual_key::S,         modifier_key::none}, // 0x73
                    {virtual_key::T,         modifier_key::none}, // 0x74
                    {virtual_key::U,         modifier_key::none}, // 0x75
                    {virtual_key::V,         modifier_key::none}, // 0x76
                    {virtual_key::W,         modifier_key::none}, // 0x77
                    {virtual_key::X,         modifier_key::none}, // 0x78
                    {virtual_key::Y,         modifier_key::none}, // 0x79
                    {virtual_key::Z,         modifier_key::none}, // 0x7A
                    {}, // 0x7B
                    {}, // 0x7C
                    {}, // 0x7D
                    {}, // 0x7E
                    {virtual_key::backspace, modifier_key::none} // 0x7F
                };

                key_event_args parse_termio_input(uint64_t input, int len);

            }
        }
    }
}


#endif
