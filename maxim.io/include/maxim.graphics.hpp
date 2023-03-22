#pragma once
#ifndef _maxim_graphics_hpp_
#define _maxim_graphics_hpp_

#include "maxim.character.hpp"
#include "maxim.character_buffer.hpp"
#include "maxim.details.hpp"

#include <tuple>
#include <string>

#ifdef _WIN32
#undef far
#undef near
#endif

namespace maxim {

    enum class alignment : uint8_t {
        near,
        center,
        far
    };

    inline int32_t align_to (alignment align, int dim, int range_near, int range_far) {
        switch (align){
            case alignment::center:
                return ((range_far - range_near) / 2) - (dim / 2);
            case alignment::far:
                return range_far - dim;
            default:
                return range_near;
        }
    }

    struct text_attributes {
        alignment       h_align;
        alignment       v_align;
        attribute_flags flags;
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

        void clear(color clear_color = colors::black);

        void draw_point (point const & position, struct color color);
        void draw_char (point  const & position, struct color color, char c, attribute_flags flags = {});

        void draw_box(
            rect const & rct,
            maxim::color color,
            bool solid = true
        );

        void draw_text(
            std::string const & text,
            point const & position,
            struct color color
        );

        void draw_text(
			std::string const & text,
			point const & position,
			int32_t width,
            struct color color,
            text_attributes attributes
        );

        void draw_text (
        	std::string const & text,
        	rect const & bounds,
            struct color color,
            text_attributes attributes);

        void draw_v_line(
            point const & position,
            int32_t length,
            maxim::color color,
            bool solid = true
        );

        void draw_h_line(
            point const & position,
            int32_t length,
            maxim::color color,
            bool solid = true
        );

    private:

    	[[nodiscard]]
    	inline rect transform_and_clip (rect const & src) const {
            // transform
            auto dest = rect { src.position + transform, src.size };

			// intersect with view
			dest = this->view.intersection (dest);

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