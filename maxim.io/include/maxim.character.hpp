#pragma once
#ifndef _maxim_character_hpp_
#define _maxim_character_hpp_

#include <cinttypes>
#include <cmath>
#include <vector>
#include <tuple>

#include "maxim.details.hpp"

namespace maxim {

    struct color {
    public:
        uint8_t r, g, b;

        [[nodiscard]] uint8_t to_256 () const;
        [[nodiscard]] uint8_t to_8 () const;
    };

    [[nodiscard]] inline bool operator == (color const v1, color const v2) {
        return v1.r == v2.r && v1.g == v2.g && v1.b == v2.b;
    }

    [[nodiscard]] inline bool operator != (color const v1, color const v2) {
        return !operator==(v1, v2);
    }

    namespace colors {

        constexpr color const
            black {},
            dark_red    { 170, 0, 0 },
            dark_green  { 0, 170, 0 },
            dark_yellow { 170, 85, 0 },
            dark_blue   { 0, 0, 170 },
            purple      { 170, 0, 170 },
            dark_cyan   { 0, 170, 170 },
            gray        { 170, 170, 170},
            dark_gray   { 85, 85, 85 },
            red         { 255, 85, 85 },
            green       { 85, 255, 85 },
            yellow      { 255, 255, 85 },
            blue        { 85, 85, 255 },
            pink        { 255, 85, 255 },
            cyan        { 85, 255, 255 },
            white       { 255, 255, 255 };
    }

    union attribute_flags {
        struct {
            bool bold       : 1;
            bool underlined : 1;
            bool blink      : 1;
            bool reversed   : 1;
            bool is_graph   : 1;
            int             : 0; // padding
        };

        uint8_t _bitflag {0};
    };

    [[nodiscard]] inline bool operator == (attribute_flags const v1, attribute_flags const v2) {
        return v1._bitflag == v2._bitflag;
    }

    struct attributes {
        color           backcolor;
        color           forecolor;
        attribute_flags flags;
    };

    [[nodiscard]] inline bool operator == (attributes const & v1, attributes const & v2) {
        return
            v1.backcolor == v2.backcolor &&
            v1.forecolor == v2.forecolor &&
            v1.flags     == v2.flags;
    }

    struct character {
		maxim::attributes   attributes;
        char                content;
    };

    struct graph_chars {
        char const
                line_vertical,
                line_horizontal,
                cross_lines,
                corner_up_left,
                corner_up_right,
                corner_down_left,
                corner_down_right,
                vertical_left,
                vertical_right,
                horizontal_up,
                horizontal_down;
    };

    inline bool operator == (character const & v1, character const & v2) {
        return
            v1.attributes   == v2.attributes &&
            v1.content      == v2.content;
    }

}

#endif
