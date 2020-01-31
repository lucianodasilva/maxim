#pragma once
#ifndef _maxim_graphics_hpp_
#define _maxim_graphics_hpp_

#include "maxim.character_buffer.hpp"
#include "maxim.details.hpp"

#include <tuple>
#include <string>

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
        text_alignment  alignment;
        text_alignment  line_alignment;
        // - TO BE IMPLEMENTED -
        //text_trimming   trimming;
        //char const      elipsis_char { '' };
        bool            wrap;
    };

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

        static inline auto get_next_line(
            std::string const& text,
            int begin,
            int line_width)
		{
			if (begin + line_width < text.size ()) {

				int end = begin + line_width;

				while (std::isgraph(text[end - 1])) {
					--end;

					if (end == 0)
						return std::make_tuple(begin, line_width);
				}

				return std::make_tuple(begin, end);
			} else {
				return std::make_tuple (begin, static_cast < int > (text.size () - begin));
			}
        }

		static inline point get_line_position (point const & text_position, size const & text_size, text_alignment alignment, int line, int line_width) {
			int x = 0;

			switch (alignment) {
			case (text_alignment::center):
				x = text_size.width / 2 - line_width / 2;
			case(text_alignment::far):
				x = text_size.width - line_width;
			default:
				x = 0;
				break;
			}

			return text_position + point { x, line};
        }

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