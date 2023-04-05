#pragma once
#ifndef MAXIM_DETAILS_HPP
#define MAXIM_DETAILS_HPP

#include <cinttypes>
#include <las.h>

#undef unix

namespace maxim {
    // yeah i know... i just wanna have the same namespace all over
    using namespace las;

    struct point {
    public:
        inline point operator + (point const & value) const { return { x + value.x, y + value.y }; }
        inline point operator += (point const & value) { x += value.x; y += value.y; return *this; }

        inline point operator - (point const & value) const { return { x - value.x, y - value.y }; }
        inline point operator -= (point const & value) { x -= value.x; y -= value.y; return *this; }

        inline point operator * (point const & value) const { return { x * value.x, y * value.y }; }
        inline point operator *= (point const & value) { x *= value.x; y *= value.y; return *this; }

        inline point operator * (int32_t scalar) const { return { x * scalar, y * scalar }; }
        inline point operator *= (int32_t scalar) { x *= scalar; y *= scalar; return *this; }

        inline point operator / (point const & value) const { return { x / value.x, y / value.y }; }
        inline point operator /= (point const & value) { x /= value.x; y /= value.y; return *this; }

        inline point operator / (int32_t scalar) const { return { x / scalar, y / scalar }; }
        inline point operator /= (int32_t scalar) { x /= scalar; y /= scalar; return *this; }

        inline bool operator == (maxim::point const & other) const { return x == other.x && y == other.y; }
        inline bool operator != (maxim::point const & other) const { return !operator==(other); }

		[[nodiscard]]
        inline bool zero () const { return x == 0 && y == 0; }

        int32_t x;
        int32_t y;
    };



    struct size {
        int32_t width, height;

		[[nodiscard]]
        inline bool zero () const { return width == 0 && height == 0; }

        inline bool operator == (maxim::size const & size) const { return width == size.width && height == size.height; }
        inline bool operator != (maxim::size const & size) const { return !operator==(size); }

    };

    struct rect {

		[[nodiscard]]
        inline bool zero () const { return position.zero () && size.zero(); }

		[[nodiscard]]
        inline int32_t left () const { return position.x; }
		[[nodiscard]]
        inline int32_t right () const { return position.x + size.width; }
		[[nodiscard]]
        inline int32_t top () const { return position.y; }
		[[nodiscard]]
        inline int32_t bottom () const { return position.y + size.height; }
        [[nodiscard]]
        inline int32_t width () const { return size.width; }
        [[nodiscard]]
        inline int32_t height () const { return size.height; }

		[[nodiscard]]
        inline bool intersects (maxim::rect const & other) const {
            return
                    left () < other.right () &&
                    top () < other.bottom () &&
                    right () > other.left () &&
                    bottom () > other.top ();
        }

		[[nodiscard]]
        inline rect intersection (rect const & other) const {
            if (!intersects(other)) {
                return {};
            }

            point const POS = {
                    std::max(left(), other.left()),
                    std::max(top(), other.top())
            };

            maxim::size const SIZE_V = {
                    std::min (right(), other.right()) - POS.x,
                    std::min (bottom(), other.bottom()) - POS.y
            };

            return { POS, SIZE_V };
        }

        [[nodiscard]]
        inline bool contains (point pnt) const {
            return
                    pnt.x >= left() && pnt.x < right() &&
                    pnt.y >= top() && pnt.y < bottom();

        }

        maxim::point 	position;
        maxim::size		size;
    };
}

#endif
