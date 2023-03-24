#include "maxim.graphics.hpp"
#include "maxim.encoder.hpp"

#include <memory>

namespace maxim {

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
		rect tc_rect = transform_and_clip(rct);

		if (tc_rect.zero())
			return;

		if (solid) {
			character c {};
			c.backcolor = color;

			for (int32_t y = tc_rect.top (); y < tc_rect.bottom(); ++y) {
				auto * ptr = _buffer.data_at ({tc_rect.left (), y});
				std::fill_n(ptr, tc_rect.size.width, c);
			}

		} else {

			point
				top_left = tc_rect.position,
				top_right = {tc_rect.right(), tc_rect.top()},
				bottom_left = {tc_rect.left(), tc_rect.bottom()},
				bottom_right = {tc_rect.right(), tc_rect.bottom()};

			// draw lines
			draw_h_line(top_left, tc_rect.size.width, color, false);
			draw_h_line(bottom_left, tc_rect.size.width, color, false);
			draw_v_line(top_left, tc_rect.size.height, color, false);
			draw_v_line(top_right, tc_rect.size.height, color, false);

			auto set_corner = [color] (character & c, char content) {
				c.content = content;
				c.flags.is_graph = true;
				c.forecolor = color;
			};

			set_corner (_buffer.at(top_left), ansi::graph_chars::corner_down_right);
			set_corner (_buffer.at(top_right), ansi::graph_chars::corner_down_left);
			set_corner (_buffer.at(bottom_left), ansi::graph_chars::corner_up_right);
			set_corner (_buffer.at(bottom_right), ansi::graph_chars::corner_up_left);
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

	inline static std::string_view calc_next_line(std::string_view text, int line_width){
		
		if (text.size() > line_width) {
			text = text.substr(0, line_width);
		}

		for (auto it = text.rbegin(); it != text.rend(); ++it) {
			if (!std::isgraph(*it)) {
				return text.substr(0, std::distance(text.begin(), it.base()));
			}
		}

		return text;
	}

	inline static int calc_line_count (std::string_view text, int line_width) {
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

	inline point calc_text_position (point const & position, size const & text_size, alignment line_alignment, int line_count) {
		switch (line_alignment) {
			case (alignment::near):
				return position;
			case (alignment::far):
				return position + point{ 0, static_cast <int> (text_size.height - line_count) };
			default:
				return position + point{ 0, static_cast <int> (text_size.height / 2 - (line_count / 2)) };
		}
	}

	inline point calc_text_line_position (point text_position, size text_size, alignment alignment, int line_index, int line_width) {
		int x = 0;

		switch (alignment) {
			case (alignment::center):
				x = text_size.width / 2 - line_width / 2;
				break;
			case(alignment::far):
				x = text_size.width - line_width;
				break;
			default:
				x = 0;
				break;
		}

		return text_position + point { x, line_index };
	}

	void graphics::draw_text(std::string_view text, rect bounds, maxim::color color, text_options options){
		// transform
		auto position = _transform + bounds.position;
		bounds = transform_and_clip(bounds);

		auto text_size = text.size ();
		auto text_position = position;

		// clip
		rect clipped = transform_and_clip(bounds);

		if (clipped.is_zero())
			return;

		if (options.wrap) {
			// calculate wrap rect
			auto lines = get_line_count (text, size.width);

			text_position = get_text_position(
					position,
					size,
					flags.v_align,
					lines);

			int offset = 0;
			for (int line = 0; line < lines; ++line) {
				auto [ 
					line_begin,
					line_end
				] = get_next_line(text, offset, size.width);

				auto line_position = get_line_position (
						text_position,
						size,
						flags.h_align,
						line,
						line_end - line_begin);

				// write to buffer
				for (int i = line_begin; i < line_end; ++i) {
					if (clipped.contains (line_position)) {
                        auto & ch = _buffer.at (line_position);
                        ch.forecolor = color;
                        ch.flags = flags.flags;
                        ch.content = text [i];
                    }

					++line_position.x;
				}

				offset = line_end;
			}
		} else {
			text_position = get_text_position(
					position,
					size,
					flags.v_align,
					1);

			int left_pad = 0;

			if (flags.h_align == alignment::far) {
				left_pad = size.width - text_size;
			} else if (flags.h_align == alignment::center) {
				auto half_width = size.width / 2;
				auto half_len = text_size / 2;

				left_pad = half_width - half_len;
			}

			// text
			for (int i = 0; i < text_size; ++i) {

				auto p = text_position + point { left_pad + i, 0};

				if (clipped.contains (p)) {
                    auto & ch = _buffer.at(p);
                    ch.forecolor = color;
                    ch.flags = options.flags;
                    ch.content = text[i];
                }
			}
		}

	}

	void graphics::draw_v_line(
		point const &position,
		int32_t length,
		struct color color,
		bool solid
	) {
		rect clipped = transform_and_clip({position, { 1, length }});

		if (clipped.is_zero())
			return;

		character c = {};

		if (solid) {
			c.flags.backcolor = color;
		} else {
			c.content = ansi::graph_chars.line_vertical;
			c.flags.flags_as_int.is_graph = true;
			c.flags.forecolor = color;
		}

		for (int32_t y = clipped.top (); y < clipped.bottom(); ++y) {
			*_buffer.ptr_at ({clipped.left (), y}) = c;
		}
	}

	void graphics::draw_h_line(
		point const &position,
		int32_t length,
		struct color color,
		bool solid
	) {
		rect clipped = transform_and_clip({position, { length, 1 }});

		if (clipped.is_zero())
			return;

		character c = {};

		if (solid) {
			c.flags.backcolor = color;
		} else {
			c.content = ansi::graph_chars.line_horizontal;
			c.flags.flags_as_int.is_graph = true;
			c.flags.forecolor = color;
		}

		auto * ptr = _buffer.ptr_at (clipped.position);
		std::fill (ptr, ptr + clipped.size.width, c);
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