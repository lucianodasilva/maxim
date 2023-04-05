#pragma once
#ifndef MAXIM_INPUT_HPP
#define MAXIM_INPUT_HPP

#include <cinttypes>
#include <type_traits>
#include <unordered_map>

#include "maxim.details.hpp"

namespace maxim::input {

    // virtual key codes
    enum struct key : uint16_t {
        none    = 0x00,
        backspace
                = 0x08,
        tab     = 0x09,

        clear   = 0x0C,
        enter   = 0x0D,

        escape  = 0x1B,
        space   = 0x20,

        page_up = 0x21,
        page_down
                = 0x22,

        end     = 0x23,
        home    = 0x24,

        left    = 0x25,
        up      = 0x26,
        right   = 0x27,
        down    = 0x28,

        insert  = 0x2D,
        del     = 0x2E,

        n0      = 0x30,
        n1      = 0x31,
        n2      = 0x32,
        n3      = 0x33,
        n4      = 0x34,
        n5      = 0x35,
        n6      = 0x36,
        n7      = 0x37,
        n8      = 0x38,
        n9      = 0x39,

        a       = 0x41,
        b       = 0x42,
        c       = 0x43,
        d       = 0x44,
        e       = 0x45,
        f       = 0x46,
        g       = 0x47,
        h       = 0x48,
        i       = 0x49,
        j       = 0x4A,
        k       = 0x4B,
        l       = 0x4C,
        m       = 0x4D,
        n       = 0x4E,
        o       = 0x4F,
        p       = 0x50,
        q       = 0x51,
        r       = 0x52,
        s       = 0x53,
        t       = 0x54,
        u       = 0x55,
        v       = 0x56,
        w       = 0x57,
        x       = 0x58,
        y       = 0x59,
        z       = 0x5A,

        f1      = 0x70,
        f2      = 0x71,
        f3      = 0x72,
        f4      = 0x73,
        f5      = 0x74,
        f6      = 0x75,
        f7      = 0x76,
        f8      = 0x77,
        f9      = 0x78,
        f10     = 0x79,
        f11     = 0x80,
        f12     = 0x81,

        shift   = 0x100,
        control = 0x200,
        alt     = 0x400,
    };

    constexpr inline key operator|(key lhv, key rhv) noexcept {
        using under_type = std::underlying_type_t<key>;

        return static_cast <key> (
                static_cast <under_type> (lhv) |
                static_cast <under_type> (rhv)
        );
    }

    constexpr inline key operator&(key lhv, key rhv) noexcept {
        using under_type = std::underlying_type_t<key>;

        return static_cast <key> (
                static_cast <under_type> (lhv) &
                static_cast <under_type> (rhv)
        );
    }

    struct key_info {
        char        char_value;
        input::key  key;

        inline explicit operator bool() const noexcept {
            return (char_value != 0U) || key != key::none;
        }
    };

    inline bool operator==(key_info const &lhv, key_info const &rhv) noexcept {
        return
                lhv.char_value == rhv.char_value &&
                lhv.key == rhv.key;
    }
}


namespace std
{
    template <>
    struct hash <maxim::input::key_info>
    {
        std::size_t operator ()(maxim::input::key_info const & info) const noexcept{
            return std::hash < maxim::input::key > () (info.key);
        }
    };
}

#endif