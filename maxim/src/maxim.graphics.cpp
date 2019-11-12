#include "maxim.graphics.hpp"
#include "maxim.encoding.hpp"

namespace maxim {

    graphics::graphics(character_buffer & buffer) :
        _buffer{ buffer },
        view{ {}, buffer.size() }
    {}

    graphics::graphics(character_buffer & buffer, rect const & view)
            : _buffer {buffer}, view {view}
    {}

    void graphics::clear() {
        _buffer = {_buffer.size()};
    }

    void graphics::draw_box(
        rect const & rct,
        color_t color,
        bool solid,
        color_t backcolor
    ){
        if (solid) {
            character c = {};
            c.backcolor = color;
            c.forecolor = color;
            c.content = ' ';

            for (
                    int32_t y = rct.top();
                    y < rct.bottom();
                    ++y
            ) {
                for (int32_t x = rct.left(); x < rct.right(); ++x) {
                    set({ x, y }, c);
                }
            }
        }
        else {

            point
                    top_left = rct.position,
                    top_right = {rct.right(), rct.top()},
                    bottom_left = {rct.left(), rct.bottom ()},
                    bottom_right = {rct.right(), rct.bottom ()};

            // draw lines
            draw_h_line(top_left, rct.size.width, color, false, backcolor);
            draw_h_line(bottom_left, rct.size.width, color, false, backcolor);
            draw_v_line(top_left, rct.size.height, color, false, backcolor);
            draw_v_line(top_right, rct.size.height, color, false, backcolor);

            // draw corners
            character c = {
                ' ', // content
            {
                false, // bold
                false, // undelined
                false, // blink
                false, // reversed
                true,  // is graph
            },
                backcolor,
                color
            };

            auto const & graphs = encoding::ansi::graph_chars;

            c.content = graphs.corner_up_right;
            set (top_left, c);

            c.content = graphs.corner_up_left;
            set (top_right, c);

            c.content = graphs.corner_down_right;
            set (bottom_left, c);

            c.content = graphs.corner_down_left;
            set(bottom_right, c);
        }
    }

    void graphics::draw_text(
        std::string const & text,
        point const & position,
        color_t color,
        color_t back_color,
        character_flags const & flags
    ) {

        character c = {
            ' ',
            flags,
            back_color,
            color
        };

        int32_t len = static_cast <int32_t> (text.size());

        for (int32_t i = 0; i < len; ++i) {
            c.content = text[i];
            set(position + point {i, 0}, c);
        }
    }

    void graphics::draw_v_line(
        point const & position,
        int32_t length,
        color_t color,
        bool solid,
        color_t backcolor
    ) {
        character c = {};

        if (solid) {
            c.content = ' ';
            c.backcolor = c.forecolor = color;
        }
        else {
            c.content = encoding::ansi::graph_chars.line_vertical;
            c.flags.is_graph = true;
            c.backcolor = backcolor;
            c.forecolor = color;
        }

        for (int32_t i = 0; i < length; ++i)
            set(position + point {0, i}, c);
    }

    void graphics::draw_h_line(
        point const & position,
        int32_t length,
        color_t color,
        bool solid,
        color_t backcolor
    ) {
        character c = {};

        if (solid) {
            c.content = ' ';
            c.backcolor = c.forecolor = color;
        }
        else {
            c.content = encoding::ansi::graph_chars.line_horizontal;
            c.flags.is_graph = true;
            c.backcolor = backcolor;
            c.forecolor = color;
        }

        for (int32_t i = 0; i < length; ++i)
            set(position + point{ i, 0 }, c);
    }

    void graphics::draw_buffer(
        character_buffer const & buffer,
        point const & position,
        rect source,
        bool null_mask
    ) {
        if (source.is_zero())
            source = { {}, buffer.size() };

        for (int32_t y = source.top(); y < source.bottom(); ++y) {
            for (int32_t x = source.left(); x < source.right(); ++x) {
                auto offset = point{ x, y };
                auto & c = buffer.at(offset);

                if (c.content != 0)
                    set(position + offset, c);
            }
        }
    }
}