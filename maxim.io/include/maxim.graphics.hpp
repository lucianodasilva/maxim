#pragma once
#ifndef _maxim_io_graphics_hpp_
#define _maxim_io_graphics_hpp_

#include "maxim.character.hpp"
#include "maxim.character_buffer.hpp"
#include "maxim.details.hpp"

#include <tuple>
#include <string>
#include <stack>

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
    public:

        explicit graphics(character_buffer & buffer);
        graphics(character_buffer & buffer, rect const & view);

        void clear(color clear_color = colors::black);

        void draw_point (point const & position, struct color color);
        void draw_char (point  const & position, struct color color, char c, attribute_flags flags = {}, bool is_graph = false);

		character * get_char (point const & position);

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

		void    transform_push (point const & transform);
		void    transform_pop ();

		[[nodiscard]] point transform () const;

		void    clip_window_push (rect const & clip);
		void    clip_window_pop ();

		rect    clip_window () const;

    private:

		[[nodiscard]]
		inline rect transform_rect (rect const & src) const {
            auto pos = src.position () + transform();
			return rect { pos.x, pos.y, src.width, src.height };
		}

    	[[nodiscard]]
        inline rect clip_rect() const {
            // intersect with buffer
            rect buffer_rect({}, _buffer.size());

			auto clipper = clip_window ();

			if (clipper.is_zero ()) {
				return buffer_rect;

			} else {
				return buffer_rect.intersection (clipper.offset (transform()));
			}
    	}

		std::stack < point > _transform_stack;
		std::stack < rect > _clip_stack;

	    character_buffer & _buffer;
    };
}

#endif