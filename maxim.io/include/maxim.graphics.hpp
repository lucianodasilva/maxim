#pragma once
#ifndef _maxim_graphics_hpp_
#define _maxim_graphics_hpp_

#include "maxim.character_buffer.hpp"
#include "maxim.details.hpp"

#include <tuple>
#include <string>

#ifdef _WIN32
#undef far
#undef near
#endif

namespace maxim {

    enum class text_alignment : uint8_t {
        center,
        near,
        far
    };

    enum class text_trimming : uint8_t {
        character,
        word,
        ellipsis_word,
        ellipsis_character
    };

    struct text_style_t {
    public:
        text_alignment  alignment { text_alignment::near };
        text_alignment  line_alignment { text_alignment::near };
        // TODO: implement text trimming
        //text_trimming   trimming;
        //char const      elipsis_char { '' };
        bool            wrap { false };
    };

    struct text_style_t_maker {
    public:

    	inline text_style_t_maker & alignment (text_alignment v) { _style.alignment = v; return *this; }
    	inline text_style_t_maker & line_alignment (text_alignment v) { _style.line_alignment = v; return *this; }
		inline text_style_t_maker & wrap (bool v) { _style.wrap = v; return *this; }

		inline operator text_style_t () const { return build (); }

		[[nodiscard]]
		inline text_style_t build () const { return _style; }

    private:
    	text_style_t _style;
    };

    inline text_style_t_maker make_text_style () { return {}; }

    struct graphics {
    private:
        character_buffer & _buffer;
    public:
        rect 	view = {};
        point 	transform = {};

        explicit graphics(character_buffer & buffer);
        graphics(character_buffer & buffer, rect const & view);

        void clear(color_t clear_color = colors::system);

        void draw_box(
            rect const & rct,
            color_t color,
            bool solid = true
        );

        void draw_text(
            std::string const & text,
            point const & position,
            style_t style
        );

        void draw_text(
			std::string const & text,
			point const & position,
			int32_t width,
			style_t style,
            text_style_t text_style
        );

        void draw_text (
        	std::string const & text,
        	rect const & bounds,
			style_t style,
        	text_style_t text_style);

        void draw_v_line(
            point const & position,
            int32_t length,
            color_t color,
            bool solid = true
        );

        void draw_h_line(
            point const & position,
            int32_t length,
            color_t color,
            bool solid = true
        );

    private:

    	[[nodiscard]]
    	inline rect transform_and_clip (rect const & src) const {
			// intersect with view
			auto dest = this->view.intersection (src);

			// transform
			dest.position = dest.position + transform;

			// intersect with buffer
			rect buffer_rect {
					{},
					_buffer.size ()
			};

			return buffer_rect.intersection (dest);
    	}

    };
}

#endif