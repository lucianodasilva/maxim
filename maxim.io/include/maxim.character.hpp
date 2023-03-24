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

        [[nodiscard]] uint8_t to_ansi_256 () const;
        [[nodiscard]] uint8_t to_ansi_8 () const;
    };

    [[nodiscard]] 
    inline bool operator == (color const v1, color const v2) {
        return v1.r == v2.r && v1.g == v2.g && v1.b == v2.b;
    }

    [[nodiscard]]
    inline bool operator != (color const v1, color const v2) {
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

    union flags {
        struct {
            bool bold       : 1;
            bool underlined : 1;
            bool blink      : 1;
            bool reversed   : 1;
            bool is_graph   : 1;
            int             : 0; // padding
        };

        uint8_t flags_as_int {0};
    };

    [[nodiscard]] 
    inline bool operator == (flags const lhv, flags const rhv) {
        return lhv.flags_as_int == rhv.flags_as_int;
    }

    struct character {
        color           backcolor = {};
        color           forecolor = {};
        maxim::flags    flags = {};
        char            content = ' ';
    };

    [[nodiscard]]
    inline bool has_same_colors (character const& lhv, character const& rhv) {
		return lhv.backcolor == rhv.backcolor && lhv.forecolor == rhv.forecolor;
    }

	[[nodiscard]]
    inline bool operator == (character const & lhv, character const & rhv) {
        return 
            has_same_colors(lhv, rhv) &&
            lhv.flags == rhv.flags &&
            lhv.content == rhv.content;
    }

}

#endif
