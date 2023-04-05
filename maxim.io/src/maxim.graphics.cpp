#include "maxim.graphics.hpp"
#include "maxim.encoder.hpp"

#include <memory>

namespace maxim {

    // local text positioning tools
    namespace {
        inline std::string_view calc_next_line(std::string_view text, int line_width) {
            if (text.size() > line_width) {
                text = text.substr(0, line_width);
            }

            for (auto it = text.rbegin(); it != text.rend(); ++it) {
                if (std::isgraph(*it) == 0) {
                    return text.substr(0, std::distance(text.begin(), it.base()));
                }
            }

            return text;
        }

        inline int calc_line_count(std::string_view text, int line_width) {
            int line_count = 0;

            // while we still have text in this view
            while (!text.empty()) {
                // get next line
                auto line = calc_next_line(text, line_width);

                // consume line
                text.substr(line.size());

                ++line_count;
            }

            return line_count;
        }

        inline point calc_text_line_position (
                rect bounds,
                alignment h_align,
                alignment v_align,
                int line_width,
                int line_count,
                int line_index
        ){
            point position = bounds.position;

            switch (h_align) {
                case (alignment::center):
                    position.x += bounds.width() / 2 + line_width / 2;
                    break;
                case (alignment::far):
                    position.x = bounds.width() - line_width;
                    break;
                default:
                    break;
            }

            switch (v_align) {
                case (alignment::center):
                    position.y += bounds.height() / 2 - (line_count / 2);
                    break;
                case (alignment::far):
                    position.y += bounds.height() - line_count;
                    break;
                default:
                    break;
            }

            return position + point { 0, line_index };
        }

    }

	graphics::graphics(character_buffer& buffer, rect clip, point transform) :
		_buffer(buffer),
		_clip(clip),
		_transform(transform)
	{}

	graphics::graphics(character_buffer& buffer, point transform) :
		graphics(buffer, {}, transform)
	{}

	graphics::graphics(character_buffer& buffer, rect clip) :
		graphics(buffer, clip, {})
	{}

	graphics::graphics(character_buffer& buffer) :
		graphics(buffer, {}, {})
	{}

	void graphics::clear(maxim::color clear_color) {
		_buffer.reset(clear_color);
	}

    void graphics::draw_point (maxim::point position, maxim::color color) {
		auto tc_pos = transform_and_clip(position);

		if (!tc_pos) {
			return;
		}

		auto& c_ref = _buffer.at(tc_pos.value());
		
		c_ref = {}; // clear
		c_ref.backcolor = color;
    }

    void graphics::draw_char (maxim::point position, maxim::color color, char c, flags flags) {
		auto tc_pos = transform_and_clip(position);

		auto& c_ref = _buffer.at(tc_pos.value());

		c_ref.forecolor = color;
		c_ref.flags = flags;
		c_ref.content = c;
    }

	void graphics::draw_box(rect rct, maxim::color color,bool solid) {
		auto const TC_RECT = transform_and_clip(rct);

		if (TC_RECT.zero()) {
            return;
        }

		if (solid) {
			character c {};
			c.backcolor = color;

			for (int32_t y = TC_RECT.top (); y < TC_RECT.bottom(); ++y) {
				auto * ptr = _buffer.data_at ({TC_RECT.left (), y});
				std::fill_n(ptr, TC_RECT.size.width, c);
			}

		} else {
            point const TOP_LEFT = TC_RECT.position;
            point const TOP_RIGHT = {TC_RECT.right(), TC_RECT.top()};
            point const BOTTOM_LEFT = {TC_RECT.left(), TC_RECT.bottom()};
            point const BOTTOM_RIGHT = {TC_RECT.right(), TC_RECT.bottom()};

			// draw lines
			draw_h_line(TOP_LEFT, TC_RECT.size.width, color, false);
			draw_h_line(TOP_RIGHT, TC_RECT.size.width, color, false);
			draw_v_line(BOTTOM_LEFT, TC_RECT.size.height, color, false);
			draw_v_line(BOTTOM_RIGHT, TC_RECT.size.height, color, false);

			auto set_corner = [color] (character & c, char content) {
				c.content = content;
				c.flags.is_graph = true;
				c.forecolor = color;
			};

			set_corner (_buffer.at(TOP_LEFT), ansi::graph_chars::CORNER_DOWN_RIGHT);
			set_corner (_buffer.at(TOP_RIGHT), ansi::graph_chars::CORNER_DOWN_LEFT);
			set_corner (_buffer.at(BOTTOM_LEFT), ansi::graph_chars::CORNER_UP_RIGHT);
			set_corner (_buffer.at(BOTTOM_RIGHT), ansi::graph_chars::CORNER_UP_LEFT);
		}
	}

	void graphics::draw_text(std::string_view text, point position, maxim::color color) {
		this->draw_text (
				text,
				{
					position,
					{ static_cast < int > (text.size()), 1 }
				},
                color,
				text_options {}
				);
	}

	void graphics::draw_text(std::string_view text,point position,int32_t width,maxim::color color,text_options flags){
		this->draw_text (
				text,
				{ position, width, 1 },
                color,
				flags);
	}

	void graphics::draw_text(std::string_view text, rect bounds, maxim::color color, text_options options){
        // get clipped bounds
        auto tc_bounds = transform_and_clip(bounds);

        if (tc_bounds.zero()) {
            return;
        }

        // transform bounds
		bounds = rect { bounds.position + _transform, bounds.size };

        auto line_count = 1;

        if (options.wrap) {
            line_count = calc_line_count (text, bounds.width ());
        }

        for (int idx = 0; idx < line_count; ++idx) {
            auto line_view = text;

            if (options.wrap) {
                line_view = calc_next_line(text, bounds.width());
            }

            auto line_position = calc_text_line_position (
                    bounds,
                    options.h_align,
                    options.v_align,
                    static_cast < int > (line_view.size()),
                    line_count,
                    idx);

            for (int i = 0; i < line_view.size(); ++i) {
                line_position.x += i;

                // clip
                if (!tc_bounds.contains(line_position)) {
                    continue;
                }

                auto & char_ref = _buffer.at (line_position);

                char_ref.forecolor = color;
                char_ref.flags = options.flags;
                char_ref.content = text [i];
            }

            // advance text view
            text = text.substr (line_view.size());
        }
	}

	void graphics::draw_v_line(
		point position,
		int32_t length,
		maxim::color color,
		bool solid
	) {
		auto tc_rect = transform_and_clip({position, { 1, length }});

		if (tc_rect.zero()) {
            return;
        }

		character c = {};

		if (solid) {
			c.backcolor = color;
		} else {
			c.content = ansi::graph_chars::LINE_VERTICAL;
			c.flags.is_graph = true;
			c.forecolor = color;
		}

		for (int32_t y = tc_rect.top (); y < tc_rect.bottom(); ++y) {
			*_buffer.data_at ({tc_rect.left (), y}) = c;
		}
	}

	void graphics::draw_h_line(
		point position,
		int32_t length,
		maxim::color color,
		bool solid
	) {
		auto tc_rect = transform_and_clip({position, { length, 1 }});

		if (tc_rect.zero()) {
            return;
        }

		character c = {};

		if (solid) {
			c.backcolor = color;
		} else {
			c.content = ansi::graph_chars::LINE_HORIZONTAL;
			c.flags.is_graph = true;
			c.forecolor = color;
		}

		auto * ptr = _buffer.data_at (tc_rect.position);
		std::fill_n (ptr, tc_rect.size.width, c);
	}

	point graphics::transform() const {
		return _transform;
	}

	void graphics::transform(point value) {
		_transform = value;
	}

	rect graphics::clip() const {
		return _clip;
	}

	void graphics::clip(rect value) {
		_clip = value;
	}

}