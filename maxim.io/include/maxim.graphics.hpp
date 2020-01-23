#pragma once
#ifndef _maxim_graphics_hpp_
#define _maxim_graphics_hpp_

#include "maxim.character_buffer.hpp"
#include "maxim.details.hpp"

#include <tuple>
#include <string>

namespace maxim {

    enum class alignment {
        left,
        centered,
        right
    };

    struct graphics {
    private:
        character_buffer & _buffer;
    public:
        rect view = {};

        graphics(character_buffer & buffer);
        graphics(character_buffer & buffer, rect const & view);

        void clear();

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
            alignment align,
            style_t style
        );

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
    		// transform
			rect dest {
				src.position + view.position,
				src.size
			};

			// intersect with view
			dest = this->view.intersection (dest);

			// intersect with buffer
			rect buffer_rect {
					{},
					_buffer.size ()
			};

			return buffer_rect.intersection (dest);
    	}

        inline void translated_set( point const & pos, character const & c ) {
            auto * ptr = translated_get (pos);
            if (!ptr)
            	return;

            *ptr = c;
        }

        inline character * translated_get (point const & pos) {
        	auto tpos = view.position + pos;

			if (!view.contains(tpos))
				return nullptr;

			return _buffer.ptr_at (tpos);
        }

    };
}

#endif