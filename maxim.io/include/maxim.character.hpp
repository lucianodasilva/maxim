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

        inline color_t rgb_to_x256 (uint8_t r, uint8_t g, uint8_t b) {
			constexpr int cv_lookup [6] = {0, 0x5F, 0x87, 0xAF, 0xD7, 0xFF};

			// calculate nearest index
			auto channel_to_index = [](auto ... c) {
				return std::make_tuple (c > 115 ? ((c - 35) / 40) : (c > 48 ? 1 : 0)...);
			};

			auto [ir, ig, ib] = channel_to_index (r, g, b);

			auto avr = (ir + ig + ib / 3);
			auto gray_index = (avr > 238 ? 23 : (avr - 3) / 10);

			auto
				cr = cv_lookup[ir],
				cg = cv_lookup[ig],
				cb = cv_lookup[ib];

			auto gv = 8 + 10 * gray_index;

			auto sqr = [](auto v1, auto v2) { return (v1 - v2) * (v1 - v2); };

			auto color_dist = sqr (cr, r) + sqr (cg, g) + sqr (cb, b);
			auto gray_dist = sqr (gv, r) + sqr (gv, g) + sqr (gv, b);

			auto clr = color_dist <= gray_dist ?
					16 + (36 * ir + 6 * ig + ib) :
					232 + gray_index;

			if (clr == 255)
				clr = colors::white;

			return clr;
        }
    }

    struct style_t_maker;

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


		inline static style_t_maker make ();
	};

	struct style_t_maker {
	public:

		style_t_maker & backcolor (color_t c) { style.backcolor = c; return *this; }
		style_t_maker & forecolor (color_t c) { style.forecolor = c; return *this; }
		style_t_maker & bold (bool v) { style.bold = v; return *this; }
		style_t_maker & underlined (bool v) { style.underlined = v; return *this; }
		style_t_maker & blink (bool v) { style.blink = v; return *this; }
		style_t_maker & reversed (bool v) { style.reversed = v; return *this; }
		style_t_maker & is_graph (bool v) { style.is_graph = v; return *this; }

		inline operator style_t () const { return style; }

	private:
		style_t style {};
	};

	style_t_maker style_t::make() { return {}; }

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
