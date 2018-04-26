#ifndef _MAXIM_TUI_POINT_HPP_
#define _MAXIM_TUI_POINT_HPP_

#include <cinttypes>

namespace maxim {
	namespace tui {

		struct point {
			int32_t x, y;

			inline point operator + (point const & v) const { return { x + v.x, y + v.y }; }
			inline point operator - (point const & v) const { return { x - v.x, y - v.y }; }
			inline point operator * (point const & v) const { return { x * v.x, y * v.y }; }
			inline point operator * (int32_t s) const { return { x * s, y * s }; }
			inline point operator / (point const & v) const { return { x / v.x, y / v.y }; }
			inline point operator / (int32_t s) const { return { x / s, y / s }; }
			inline bool operator == (point const & p) const { return x == p.x && y == p.y; }
			inline bool operator != (point const & p) const { return !operator==(p); }
		};

	}
}

#endif