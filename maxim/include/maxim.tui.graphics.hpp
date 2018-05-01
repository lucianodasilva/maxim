#ifndef _MAXIM_TUI_GRAPHICS_HPP_
#define _MAXIM_TUI_GRAPHICS_HPP_

#include "maxim.tui.character.hpp"
#include "maxim.tui.structures.hpp"

#include <tuple>

namespace maxim {
	namespace tui {

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
				bool solid = true,
				color_t backcolor = colors::black
			);

			void draw_text(
				std::string const & text,
				point const & position,
				color_t color = colors::white,
				color_t back_color = colors::black,
				character_flags const & flags = {}
			);

			void draw_v_line(
				point const & position,
				int32_t length,
				color_t color,
				bool solid = true,
				color_t backcolor = colors::black
			);

			void draw_h_line(
				point const & position,
				int32_t length,
				color_t color,
				bool solid = true,
				color_t backcolor = colors::black
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

		};

	}
}

#endif // _MAXIM_TUI_GRAPHICS_HPP_