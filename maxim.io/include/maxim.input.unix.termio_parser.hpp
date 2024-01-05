#pragma once
#ifndef _maxim_input_unix_termio_parser_hpp_
#define _maxim_input_unix_termio_parser_hpp_

#include "maxim.input.hpp"

namespace maxim::input::_unix {

    constexpr key_info key_map[] = {
            {}, // 0x00
            { 0, virtual_key::A,         modifier_key::control}, // 0x01
            { 0, virtual_key::B,         modifier_key::control}, // 0x02
            { 0, virtual_key::C,         modifier_key::control}, // 0x03
            { 0, virtual_key::D,         modifier_key::control}, // 0x04
            { 0, virtual_key::E,         modifier_key::control}, // 0x05
            { 0, virtual_key::F,         modifier_key::control}, // 0x06
            { 0, virtual_key::G,         modifier_key::control}, // 0x07
            { 0, virtual_key::backspace, modifier_key::control}, // 0x08
            { 0, virtual_key::tab,       modifier_key::none}, // 0x09
            { 0, virtual_key::J,         modifier_key::control}, // 0x0A
            { 0, virtual_key::K,         modifier_key::control}, // 0x0B
            { 0, virtual_key::L,         modifier_key::control}, // 0x0C
            { 0, virtual_key::enter,     modifier_key::none}, // 0x0D
            { 0, virtual_key::N,         modifier_key::control}, // 0x0E
            { 0, virtual_key::O,         modifier_key::control}, // 0x0F
            { 0, virtual_key::P,         modifier_key::control}, // 0x10
            { 0, virtual_key::Q,         modifier_key::control}, // 0x11
            { 0, virtual_key::R,         modifier_key::control}, // 0x12
            { 0, virtual_key::S,         modifier_key::control}, // 0x13
            { 0, virtual_key::T,         modifier_key::control}, // 0x14
            { 0, virtual_key::U,         modifier_key::control}, // 0x15
            { 0, virtual_key::V,         modifier_key::control}, // 0x16
            { 0, virtual_key::W,         modifier_key::control}, // 0x17
            { 0, virtual_key::X,         modifier_key::control}, // 0x18
            { 0, virtual_key::Y,         modifier_key::control}, // 0x19
            { 0, virtual_key::Z,         modifier_key::control}, // 0x1A
            { 0, virtual_key::escape,    modifier_key::none}, // 0x1B
            {}, // 0x1C
            {}, // 0x1D
            {}, // 0x1E
            {}, // 0x1F
            { 0, virtual_key::space,     modifier_key::none}, // 0x20
            { 0, virtual_key::none,      modifier_key::shift}, // 0x21
            { 0, virtual_key::none,      modifier_key::shift}, // 0x22
            { 0, virtual_key::none,      modifier_key::shift}, // 0x23
            { 0, virtual_key::none,      modifier_key::shift}, // 0x24
            { 0, virtual_key::none,      modifier_key::shift}, // 0x25
            { 0, virtual_key::none,      modifier_key::shift}, // 0x26
            { 0, virtual_key::none,      modifier_key::shift}, // 0x27
            { 0, virtual_key::none,      modifier_key::shift}, // 0x28
            { 0, virtual_key::none,      modifier_key::shift}, // 0x29
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
            { 0, virtual_key::none,      modifier_key::shift}, // 0x3D
            {}, // 0x3E
            {}, // 0x3F
            {}, // 0x40
            { 0, virtual_key::A,         modifier_key::shift}, // 0x41
            { 0, virtual_key::B,         modifier_key::shift}, // 0x42
            { 0, virtual_key::C,         modifier_key::shift}, // 0x43
            { 0, virtual_key::D,         modifier_key::shift}, // 0x44
            { 0, virtual_key::E,         modifier_key::shift}, // 0x45
            { 0, virtual_key::F,         modifier_key::shift}, // 0x46
            { 0, virtual_key::G,         modifier_key::shift}, // 0x47
            { 0, virtual_key::H,         modifier_key::shift}, // 0x48
            { 0, virtual_key::I,         modifier_key::shift}, // 0x49
            { 0, virtual_key::J,         modifier_key::shift}, // 0x4A
            { 0, virtual_key::K,         modifier_key::shift}, // 0x4B
            { 0, virtual_key::L,         modifier_key::shift}, // 0x4C
            { 0, virtual_key::M,         modifier_key::shift}, // 0x4D
            { 0, virtual_key::N,         modifier_key::shift}, // 0x4E
            { 0, virtual_key::O,         modifier_key::shift}, // 0x4F
            { 0, virtual_key::P,         modifier_key::shift}, // 0x50
            { 0, virtual_key::Q,         modifier_key::shift}, // 0x51
            { 0, virtual_key::R,         modifier_key::shift}, // 0x52
            { 0, virtual_key::S,         modifier_key::shift}, // 0x53
            { 0, virtual_key::T,         modifier_key::shift}, // 0x54
            { 0, virtual_key::U,         modifier_key::shift}, // 0x55
            { 0, virtual_key::V,         modifier_key::shift}, // 0x56
            { 0, virtual_key::W,         modifier_key::shift}, // 0x57
            { 0, virtual_key::X,         modifier_key::shift}, // 0x58
            { 0, virtual_key::Y,         modifier_key::shift}, // 0x59
            { 0, virtual_key::Z,         modifier_key::shift}, // 0x5A
            {}, // 0x5B
            {}, // 0x5C
            {}, // 0x5D
            {}, // 0x5E
            {}, // 0x5F
            {}, // 0x60
            { 0, virtual_key::A,         modifier_key::none}, // 0x61
            { 0, virtual_key::B,         modifier_key::none}, // 0x62
            { 0, virtual_key::C,         modifier_key::none}, // 0x63
            { 0, virtual_key::D,         modifier_key::none}, // 0x64
            { 0, virtual_key::E,         modifier_key::none}, // 0x65
            { 0, virtual_key::F,         modifier_key::none}, // 0x66
            { 0, virtual_key::G,         modifier_key::none}, // 0x67
            { 0, virtual_key::H,         modifier_key::none}, // 0x68
            { 0, virtual_key::I,         modifier_key::none}, // 0x69
            { 0, virtual_key::J,         modifier_key::none}, // 0x6A
            { 0, virtual_key::K,         modifier_key::none}, // 0x6B
            { 0, virtual_key::L,         modifier_key::none}, // 0x6C
            { 0, virtual_key::M,         modifier_key::none}, // 0x6D
            { 0, virtual_key::N,         modifier_key::none}, // 0x6E
            { 0, virtual_key::O,         modifier_key::none}, // 0x6F
            { 0, virtual_key::P,         modifier_key::none}, // 0x70
            { 0, virtual_key::Q,         modifier_key::none}, // 0x71
            { 0, virtual_key::R,         modifier_key::none}, // 0x72
            { 0, virtual_key::S,         modifier_key::none}, // 0x73
            { 0, virtual_key::T,         modifier_key::none}, // 0x74
            { 0, virtual_key::U,         modifier_key::none}, // 0x75
            { 0, virtual_key::V,         modifier_key::none}, // 0x76
            { 0, virtual_key::W,         modifier_key::none}, // 0x77
            { 0, virtual_key::X,         modifier_key::none}, // 0x78
            { 0, virtual_key::Y,         modifier_key::none}, // 0x79
            { 0, virtual_key::Z,         modifier_key::none}, // 0x7A
            {}, // 0x7B
            {}, // 0x7C
            {}, // 0x7D
            {}, // 0x7E
            { 0, virtual_key::backspace, modifier_key::none} // 0x7F
    };

    namespace details {

        constexpr std::size_t static_strnlen (
                char const * buffer,
                char const * end,
                std::size_t index = 0
        ) noexcept {
            return buffer != end && *buffer ? static_strnlen(buffer + 1, end, index + 1) : index;
        }

    }

    template < std::size_t _len_v >
    constexpr std::size_t sequence_len (char const (&buffer) [_len_v]) noexcept {
        return details::static_strnlen(+buffer, +buffer + _len_v);
    }

    struct span {
    public:

        inline span operator + (std::ptrdiff_t len) {
            return advance(len);
        }

        inline span & operator += (std::ptrdiff_t len) {
            begin += len;

            if (begin > end)
                begin = end;

            return *this;
        }

        inline explicit operator bool () const {
            return begin != end;
        }

        template < std::size_t _len_v >
        inline bool match (char const (&sequence) [_len_v]) {
            auto * seq_ptr = +sequence;
            auto * seq_end = seq_ptr + _len_v;
            auto * cursor = begin;

            while (seq_ptr != seq_end && *seq_ptr != '\0') {
                if (cursor == end || *cursor != *seq_ptr)
                    return false;

                ++cursor;
                ++seq_ptr;
            }

            return true;
        }

        span advance (std::ptrdiff_t len) {
            span ret = *this;
            ret += len;

            return ret;
        }

        inline void ignore_until (char c) {
            while (begin != end && *begin != c)
                ++begin;

            if (begin != end)
                ++begin;
        }

        inline void step () {
            if (begin != end)
                ++begin;
        }

        inline bool is (char c) {
            if (!*this)
                return false;

            return *begin == c;
        }

		inline const char * find (char c) const {
			auto * cursor = begin;

			while (cursor != end && *cursor != c) {
				++cursor;
			}

			return cursor;
		}

        const char * begin, * end;
    };

    key_info termio_parse (span source);

}

#endif
