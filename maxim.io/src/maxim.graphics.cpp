#include "maxim.graphics.hpp"
#include "maxim.encoding.hpp"

#include <memory>

namespace maxim {

	graphics::graphics(character_buffer &buffer) :
		_buffer{buffer},
		view{{}, buffer.size()} {}

	graphics::graphics(character_buffer &buffer, rect const &view)
		: _buffer{buffer}, view{view} {}

	void graphics::clear(color_t clear_color) {
		style_t s;
		s.backcolor = clear_color;

		_buffer.reset(s);
	}

	void graphics::draw_box(
		rect const &rct,
		color_t color,
		bool solid
	) {
		rect clipped = transform_and_clip(rct);

		if (clipped.is_zero())
			return;

		if (solid) {
			character c;
			c.style.backcolor = color;

			for (int32_t y = clipped.top (); y < clipped.bottom(); ++y) {
				auto * ptr = _buffer.ptr_at ({clipped.left (), y});
				std::fill(ptr, ptr + clipped.size.width, c);
			}

		} else {

			point
				top_left = clipped.position,
				top_right = {clipped.right(), clipped.top()},
				bottom_left = {clipped.left(), clipped.bottom()},
				bottom_right = {clipped.right(), clipped.bottom()};

			// draw lines
			draw_h_line(top_left, clipped.size.width, color, false);
			draw_h_line(bottom_left, clipped.size.width, color, false);
			draw_v_line(top_left, clipped.size.height, color, false);
			draw_v_line(top_right, clipped.size.height, color, false);

			auto const &graphs = encoding::ansi::graph_chars;

			auto set_corner = [color] (character * c, char content) {
				if (!c)
					return;

				c->content = content;
				c->style.is_graph = true;
				c->style.forecolor = color;
			};

			set_corner (_buffer.ptr_at(top_left), graphs.corner_down_right);
			set_corner (_buffer.ptr_at(top_right), graphs.corner_down_left);
			set_corner (_buffer.ptr_at(bottom_left), graphs.corner_up_right);
			set_corner (_buffer.ptr_at(bottom_right), graphs.corner_up_left);
		}
	}

	void graphics::draw_text(
		std::string const &text,
		point const &position,
		style_t style
	) {
		this->draw_text (
				text,
				{
					position,
					{ static_cast < int > (text.size()), 1 }},
				style,
				text_style_t_maker ().build ()
				);
	}

	void graphics::draw_text(
			std::string const &text,
			point const &position,
			int32_t width,
			style_t style,
			text_style_t text_style
	) {
		this->draw_text (
				text,
				{ position, width, 1 },
				style,
				text_style);
	}

	inline auto get_next_line(
			std::string const& text,
			int begin,
			int line_width)
	{
		if (begin + line_width < text.size ()) {

			int end = begin + line_width;

			while (end > 0 && std::isgraph(text[end - 1]))
				--end;

			if (end - begin <= 0)
				return std::make_tuple(begin, begin + line_width);

			return std::make_tuple(begin, end);
		} else {
			return std::make_tuple (begin, static_cast < int > (text.size ()));
		}
	}

	inline int get_line_count (std::string const & text, int line_width) {
		int offset {0};
		int line_count {0};

		int begin {0};
		int end {0};

		while (end < text.size ()) {
			std::tie (begin, end) = get_next_line(text, offset, line_width);
			offset = end;
			++line_count;
		}

		return line_count;
	}

	inline point get_text_position (point const & position, size const & text_size, text_alignment line_alignment, int line_count) {
		switch (line_alignment) {
			case (text_alignment::near):
				return position;
			case (text_alignment::far):
				return position + point{ 0, static_cast <int> (text_size.height - line_count) };
			default:
				return position + point{ 0, static_cast <int> (text_size.height / 2 - (line_count / 2)) };
		}

	}

	inline point get_line_position (point const & text_position, size const & text_size, text_alignment alignment, int line, int line_width) {
		int x = 0;

		switch (alignment) {
			case (text_alignment::center):
				x = text_size.width / 2 - line_width / 2;
				break;
			case(text_alignment::far):
				x = text_size.width - line_width;
				break;
			default:
				x = 0;
				break;
		}

		return text_position + point { x, line };
	}

	void graphics::draw_text(
			std::string const &text,
			rect const & bounds,
			style_t style,
			text_style_t text_style
	) {

		this->draw_box (bounds, style.backcolor);

		// transform
		auto position = bounds.position + transform;
		auto size = bounds.size;

		auto text_size = text.size ();
		auto text_position = position;

		// clip
		rect clipped = transform_and_clip(bounds);

		if (clipped.is_zero())
			return;

		character c { style };

		if (text_style.wrap) {
			// calculate wrap rect
			auto lines = get_line_count (text, size.width);

			text_position = get_text_position(
					position,
					size,
					text_style.line_alignment,
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
						text_style.alignment,
						line,
						line_end - line_begin);

				// write to buffer
				for (int i = line_begin; i < line_end; ++i) {
					c.content = text[i];

					if (clipped.contains (line_position))
						*_buffer.ptr_at (line_position) = c;

					++line_position.x;
				}

				offset = line_end;
			}
		} else {
			text_position = get_text_position(
					position,
					size,
					text_style.line_alignment,
					1);

			int left_pad = 0;

			if (text_style.alignment == text_alignment::far) {
				left_pad = size.width - text_size;
			} else if (text_style.alignment == text_alignment::center) {
				auto half_width = size.width / 2;
				auto half_len = text_size / 2;

				left_pad = half_width - half_len;
			}

			// text
			for (int i = 0; i < text_size; ++i) {
				c.content = text[i];

				auto p = text_position + point { left_pad + i, 0};
				if (clipped.contains (p))
					*_buffer.ptr_at (p) = c;
			}
		}

	}

	void graphics::draw_v_line(
		point const &position,
		int32_t length,
		color_t color,
		bool solid
	) {
		rect clipped = transform_and_clip({position, { 1, length }});

		if (clipped.is_zero())
			return;

		character c = {};

		if (solid) {
			c.style.backcolor = color;
		} else {
			c.content = encoding::ansi::graph_chars.line_vertical;
			c.style.is_graph = true;
			c.style.forecolor = color;
		}

		for (int32_t y = clipped.top (); y < clipped.bottom(); ++y) {
			*_buffer.ptr_at ({clipped.left (), y}) = c;
		}
	}

	void graphics::draw_h_line(
		point const &position,
		int32_t length,
		color_t color,
		bool solid
	) {
		rect clipped = transform_and_clip({position, { length, 1 }});

		if (clipped.is_zero())
			return;

		character c = {};

		if (solid) {
			c.style.backcolor = color;
		} else {
			c.content = encoding::ansi::graph_chars.line_vertical;
			c.style.is_graph = true;
			c.style.forecolor = color;
		}

		auto * ptr = _buffer.ptr_at (clipped.position);
		std::fill (ptr, ptr + clipped.size.width, c);
	}

}