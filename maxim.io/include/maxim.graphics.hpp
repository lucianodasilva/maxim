#pragma once
#ifndef _maxim_graphics_hpp_
#define _maxim_graphics_hpp_

#include "maxim.character.hpp"
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

        void draw_buffer(
            character_buffer const & buffer,
            point const & position,
            rect source = {},
            bool null_mask = false
        );

    private:

        inline void set( point const & pos, character const & c ) {
            // transform
            auto tpos = view.position + pos;

            // clip
            if (!view.contains(tpos))
                return;

            _buffer.at(tpos) = c;
        }

        inline character * get (point const & pos) {
        	auto tpos = view.position + pos;

			if (!view.contains(tpos))
				return nullptr;

			return &_buffer.at (tpos);
        }

    };
}

#endif