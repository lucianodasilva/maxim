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
		rect clipped = transform_and_clip({
			position,
			{
				static_cast < int32_t > (text.size ()),
				1
			}});

		if (clipped.is_zero())
			return;

		// find text offset
		auto xoffset = clipped.position.x - (view.position + position).x;

		// trim to zero
		if (xoffset < 0)
			xoffset = 0;

		character c = {
			style,
			' '
		};

		character * ptr = _buffer.ptr_at ({clipped.position});

		for (int32_t i = 0; i < clipped.size.width; ++i) {
			c.content = text[i + xoffset];
			*ptr = c;
			++ptr;
		}
	}

	void graphics::draw_text(
		std::string const &text,
		point const &position,
		int32_t width,
		alignment align,
		style_t style
	) {
		rect clipped = transform_and_clip({
			  position,
			  {
				  width,
				  1
			  }});

		if (clipped.is_zero())
			return;

		// find text offset
		auto tpos = position + transform;

		character c = {
			style,
			' '
		};

		int32_t
			len = std::min(static_cast <int32_t> (text.size()), width),
			left_pad{0};

		if (align == alignment::right) {
			left_pad = width - len;
		} else if (align == alignment::centered) {
			auto half_width = width / 2;
			auto half_len = len / 2;

			left_pad = half_width - half_len;
		}

		// pad left
		/*
		c.content = ' ';
		for (int32_t i = 0; i < left_pad; ++i) {
			auto p = tpos + point { i, 0};
			if (clipped.contains (p))
				*_buffer.ptr_at (p) = c;
		}*/

		// text
		for (int32_t i = 0; i < len; ++i) {
			c.content = text[i];

			auto p = tpos + point { left_pad + i, 0};
			if (clipped.contains (p))
				*_buffer.ptr_at (p) = c;
		}

		/*
		c.content = ' ';

		// pad right
		for (int32_t i = left_pad + len; i < width; ++i) {
			auto p = tpos + point { i, 0};
			if (clipped.contains (p))
				*_buffer.ptr_at (p) = c;
		}*/
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