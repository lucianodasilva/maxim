#pragma once
#ifndef _maxim_character_hpp_
#define _maxim_character_hpp_

#include <cinttypes>
#include <vector>
#include <tuple>

#include "maxim.character_change_set.hpp"
#include "maxim.details.hpp"

namespace maxim {

    using color_t = uint8_t;

    namespace colors {
        constexpr color_t const
                black       = 0x0,
                dark_red    = 0x1,
                dark_green  = 0x2,
                dark_yellow = 0x3,
                dark_blue   = 0x4,
                purple      = 0x5,
                dark_cyan   = 0x6,
                gray        = 0x7,
                dark_gray   = 0x8,
                red         = 0x9,
                green       = 0xA,
                yellow      = 0xB,
                blue        = 0xC,
                pink        = 0xD,
                cyan        = 0xE,
                white       = 0xF,
                system		= 0xFF;
    }

    struct style_t {

		color_t backcolor { colors::system };
		color_t forecolor { colors::system };

		union {
			struct {
				bool bold       : 1;
				bool underlined : 1;
				bool blink      : 1;
				bool reversed   : 1;
				bool is_graph   : 1;
				int             : 0; // fill to next byte
			};

			uint8_t _style_flags;
		};
	};

    inline bool operator == (style_t a, style_t b) {
    	return
    		a._style_flags == b._style_flags &&
    		a.backcolor == b.backcolor &&
    		a.forecolor == b.forecolor;
    }

    struct character {
		style_t style 	{};
        char content 	{' '};
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

    inline bool operator == (character const & a, character const & b) {
        return
            a.content == b.content &&
            a.style == b.style;
    }

}

#endif
