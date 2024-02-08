#pragma once
#ifndef _maxim_io_graphics_hpp_
#define _maxim_io_graphics_hpp_

#include "maxim.character.hpp"
#include "maxim.character_buffer.hpp"
#include "maxim.details.hpp"

#include <string>

#ifdef _WIN32
#undef far
#undef near
#endif

namespace maxim {

	/// Alignment flags
	enum class alignment : uint8_t {
		near,   /// left or top
		center, /// center
		far     /// right or bottom
	};

	/// Align dimension to range
	/// \param align - Alignment
	/// \param dim - Dimension
	/// \param range_near - Near range
	/// \param range_far - Far range
	/// \return Starting position
	[[nodiscard]]
	inline int32_t align_to(alignment align, int dim, int range_near, int range_far) {
		switch (align) {
			case alignment::center:
				return ((range_far - range_near) / 2) - (dim / 2);
			case alignment::far:
				return range_far - dim;
			default:
				return range_near;
		}
	}

	/// Text attribute structure
	struct text_attributes {
		/// Horizontal Alignment
		alignment h_align{};
		/// Vertical Alignment
		alignment v_align{};
		/// Text Attribute Flags
		attribute_flags flags{};
		/// Wrap Text
		bool wrap{};
	};

	int text_height (std::string const &text, int width);

	/// Graphics structure
	struct graphics {
	public:

		/// Default constructor
		/// \param buffer - Character buffer
		explicit graphics(character_buffer & buffer);

		/// Constructor
		/// \param buffer - Character buffer
		/// \param clip - Clip window
		graphics(character_buffer & buffer, rect clip);

		/// Clear buffer with color
		/// \param clear_color - Color to clear buffer with
		void clear(color clear_color = colors::black);

		/// Draw point
		/// \param position - Position of point
		/// \param color - Color of point
		void draw_point(point const &position, struct color color);

		/// Draw  character
		/// \param position - Position of character
		/// \param color - Color of character
		/// \param c - Character
		/// \param flags - Character attribute flags
		/// \param is_graph - Is graph character
		void
		draw_char(point const &position, struct color color, char c, attribute_flags flags = {}, bool is_graph = false);

		/// Get character at position from character_buffer
		/// \param position - Position of character
		/// \return Pointer to character
		/// \note Returns nullptr if position is out of bounds
		/// \note Pointer may be invalidated if character_buffer is resized
		character *get_char(point const &position);

		/// Draw rectangle
		/// \param rct - Rectangle
		/// \param color - Color
		/// \param solid - Solid or outline
		void draw_rect(rect const &rct, maxim::color color, bool solid = true);

		/// Draw text
		/// \param text - Text
		/// \param position - Position
		/// \param color - Color
		void draw_text(std::string const &text, point const &position, struct color color);

		/// Draw text
		/// \param text - Text
		/// \param position - Position
		/// \param width - Width
		/// \param color - Color
		/// \param attributes - Text attributes
		void draw_text(std::string const &text, point const &position, int32_t width, struct color color,
		               text_attributes attributes);

		/// Draw text
		/// \param text - Text
		/// \param bounds - Bounds
		/// \param color - Color
		/// \param attributes - Text attributes
		void draw_text(std::string const &text, rect const &bounds, struct color color, text_attributes attributes);

		/// Draw vertical line
		/// \param position - Starting Position
		/// \param length - Length
		/// \param color - Color
		/// \param solid - Solid or line
		void draw_v_line(point const &position, int32_t length, maxim::color color, bool solid = true);

		/// Draw horizontal line
		/// \param position - Starting Position
		/// \param length - Length
		/// \param color - Color
		/// \param solid - Solid or line
		void draw_h_line(point const &position, int32_t length, maxim::color color, bool solid = true);

		/// set transform offset
		void transform(point const & transform) { _transform = transform; }

		/// Get current transform offset
		[[nodiscard]] point transform() const { return _transform; }

		/// Set clip window
		void clip_window(rect const& clip) { _clip_window = maxim::rect({}, _buffer.size2d()).intersection (clip); }

		/// Get current clip window
		[[nodiscard]] rect clip_window() const { return _clip_window; }

	private:

		[[nodiscard]]
		rect transform_rect (rect val) const {
			return { val.position () + transform(), val.size () };
		}

		character_buffer &	_buffer;
		point				_transform{};
		rect				_clip_window{};
	};

	struct graphics_transform_guard {
	public:

		graphics_transform_guard(graphics& g, rect clip_in_local) :
			_g(g)
		{
			_g.clip_window({ clip_in_local.position() + _g.transform(), clip_in_local.size() });
			_g.transform(_g.transform() + clip_in_local.position());
		}

		graphics_transform_guard(graphics& g, point transform, rect clip_in_global) :
			_g(g)
		{
			_g.clip_window(clip_in_global);
			_g.transform(_g.transform() + transform);
		}

		~graphics_transform_guard() {
			_g.transform(_stacked_transform);
			_g.clip_window(_stacked_clip);
		}
	private:
		graphics&	_g;
		point		_stacked_transform = _g.transform();
		rect		_stacked_clip = _g.clip_window();
	};

}

#endif