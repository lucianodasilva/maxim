#pragma once
#ifndef _maxim_io_details_hpp_
#define _maxim_io_details_hpp_

#include <algorithm>
#include <cinttypes>
#include <cmath>

namespace maxim {

    /// 2D vector helper structure
    /// \tparam type - Type of vector components
    template < typename type >
    struct vec2d {
    public:
        using value_type = type;

        /// Determines if vector length is zero
        /// \return True if vector length is zero
        [[nodiscard]]
        constexpr bool is_zero() const { return x == 0 && y == 0; }

        /// x component
        value_type x{};
        /// y component
        value_type y{};
    };

    template <typename lhs_type, typename rhs_type>
    constexpr vec2d<lhs_type> operator+(vec2d<lhs_type> const& lhs, vec2d<rhs_type> const& rhs) {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    template <typename lhs_type, typename rhs_type>
    constexpr vec2d<lhs_type> operator-(vec2d<lhs_type> const& lhs, vec2d<rhs_type> const& rhs) {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    template <typename lhs_type, typename rhs_type>
    constexpr vec2d<lhs_type> operator*(vec2d<lhs_type> const& lhs, vec2d<rhs_type> const& rhs) {
        return { lhs.x * rhs.x, lhs.y * rhs.y };
    }

    template <typename lhs_type, typename rhs_type>
    constexpr vec2d<lhs_type> operator*(vec2d<lhs_type> const& lhs, rhs_type const& rhs) {
        return { lhs.x * rhs, lhs.y * rhs };
    }

    template <typename lhs_type, typename rhs_type>
    constexpr vec2d<lhs_type> operator/(vec2d<lhs_type> const& lhs, vec2d<rhs_type> const& rhs) {
        return { lhs.x / rhs.x, lhs.y / rhs.y };
    }

    template <typename lhs_type, typename rhs_type>
    constexpr vec2d<lhs_type> operator/(vec2d<lhs_type> const& lhs, rhs_type const& rhs) {
        return { lhs.x / rhs, lhs.y / rhs };
    }

    template <typename lhs_type, typename rhs_type>
    constexpr bool operator==(vec2d<lhs_type> const& lhs, vec2d<rhs_type> const& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    template <typename lhs_type, typename rhs_type>
    constexpr bool operator!=(vec2d<lhs_type> const& lhs, vec2d<rhs_type> const& rhs) {
        return !(lhs == rhs);
    }

	/// Calculate linear interpolation between two values
	/// \tparam value_type - Values type
	/// \param a - First value
	/// \param b - Second value
	/// \param t - Interpolation factor
	/// \return Interpolated value
	template < typename value_type >
	constexpr value_type lerp (value_type const& a, value_type const& b, float t) noexcept {
		return a + (b - a) * t;
	}

	/// Calculate linear interpolation between two points
	/// \tparam value_type - Type of point components
	/// \param a - First point
	/// \param b - Second point
	/// \param t - Interpolation factor
	/// \return Interpolated point
	template < typename value_type >
	constexpr float length (vec2d < value_type > const& value) noexcept {
		return std::sqrt (value.x * value.x + value.y * value.y);
	}

	/// Calculate distance between two points
	/// \tparam value_type - Type of components
	/// \param a - First point
	/// \param b - Second point
	/// \return Distance between points
	template < typename value_type >
	constexpr float distance(vec2d < value_type > const& a, vec2d < value_type > const& b) noexcept {
		return length(a - b);
	}

	/// Integer 2D vector
    using point     = vec2d < int32_t >;
    /// Floating point 2D vector
    using pointf    = vec2d < float >;

    /// Size helper structure
    struct size {
        public:
			constexpr size() = default;
			constexpr size(int32_t width, int32_t height) : width(width), height(height) {}

			/// Constructs size from point
			/// \param p - Origin point variable 
			constexpr size(point const& p) : width(p.x), height(p.y) {}

			/// Determines if all components are zero
			[[nodiscard]]
			constexpr bool is_zero() const { return width == 0 && height == 0; }

			/// Converts size to point
			[[nodiscard]]
			constexpr point as_point() const { return { width, height }; }

            constexpr bool operator==(size const& rhs) const {
				return width == rhs.width && height == rhs.height;
			}

            constexpr bool operator!=(size const& rhs) const {
				return !operator==(rhs);
			}

			/// Width component
			int32_t width{};
            /// Height component
			int32_t height{};
    };

    /// Rectangular bounds helper structure
    struct rect {

        /// Default constructor
        constexpr rect() = default;

        /// Constructs rect from position and size
        /// \param x - X coordinate of top left corner
        /// \param y - Y coordinate of top left corner
        /// \param width - Width of rect
        /// \param height - Height of rect 
        constexpr rect (int x, int y, int width, int height) : 
            x(x), y(y), width(width), height(height) {}

        /// Constructs rect from position and size
        /// \param position - 2d coordinates of top left corner 
        /// \param size - Size of rect
        constexpr rect (point const& position, maxim::size const& size) :
			x(position.x), y(position.y), width(size.width), height(size.height) {}

        /// Determines if rect is empty
        [[nodiscard]] constexpr bool is_zero() const { return x == 0 && y == 0 && width == 0 && height == 0; }

        /// Coordinates of top left corner as point
        [[nodiscard]] constexpr point position() const { return { x, y }; }

        /// Size of rect as size
		[[nodiscard]] constexpr maxim::size size() const { return { width, height }; }

        /// Left most x axis value 
        [[nodiscard]] constexpr int32_t left() const { return x; }

        /// Right most x axis value
        [[nodiscard]] constexpr int32_t right() const { return x + std::max < int32_t > (0, width - 1); }

        /// Top most y axis value
        [[nodiscard]] constexpr int32_t top() const { return y; }
        /// Bottom most y axis value
        [[nodiscard]] constexpr int32_t bottom() const { return y + std::max < int32_t > (0, height - 1); }

        /// Determines if rect intersects with another rect
        /// \param r - Rect to check intersection with
        [[nodiscard]] constexpr bool intersects(rect const& r) const {
            if (this->is_zero() || r.is_zero()) {
                return false;
            }

            return
                left() <= r.right() &&
                top() <= r.bottom() &&
                right() >= r.left() &&
                bottom() >= r.top();
        }

        /// Calculates intersection rect with another rect
        /// \param r - Rect to calculate intersection with
        /// \return Intersection rect or empty rect if no intersection
        [[nodiscard]] constexpr rect intersection(rect const& r) const {
            if (!intersects(r)) {
                return {};
            }

            point near_point = {
				std::max(left(), r.left()),
				std::max(top(), r.top())
            };

			point far_point = {
				std::min (right (), r.right()),
				std::min (bottom(), r.bottom())
			};

            return { 
                near_point,
				(far_point - near_point) + point {1, 1}
            };
        }

        /// Determines if rect contains point
        /// \param p - Point to check
        [[nodiscard]] constexpr bool contains(point const& p) const {
            return
                p.x >= left() && p.x <= right() &&
                p.y >= top() && p.y <= bottom();
        }

        /// Transforms rect by offsetting its position by a point
        /// \param p - Offset point
        /// \return Transformed rect
        [[nodiscard]] constexpr rect offset(point const& p) const {
			return { x + p.x, y + p.y, width, height };
		}

        /// Transforms rect by expanding its size by a point
		/// \param p - Expansion point
		/// \return Transformed rect
		[[nodiscard]] constexpr rect expand(point const& p) const {
            return { x, y, width + p.x, height + p.y };
        }
        
        /// X component
        int32_t x{};
        /// Y component
        int32_t y{};
        /// Width component
        int32_t width{};
        /// Height component
        int32_t height{};
    };

    /// Check if enum flag value contains enum item
    /// \tparam _enum_t - Enum type
    /// \param origin - Flag value
    /// \param value - Enum item
    /// \return True if flag value contains enum item
    template < typename _enum_t, typename std::enable_if_t < std::is_enum_v < _enum_t >, int > = 0 >
    constexpr bool enum_has(_enum_t origin, _enum_t value) {
        using under_t = std::underlying_type_t < _enum_t >;

        auto cast_origin = static_cast <under_t> (origin);
        auto cast_value = static_cast <under_t> (value);

        return (cast_origin & cast_value) == cast_value;
    }

    /// Concatenate enum items into flag value
    /// \tparam enum_tv - Enum types
    /// \param args - Enum items
    /// \return Flag value
    template < typename ... enum_tv >
    constexpr auto enum_cat(enum_tv ... args) {
        static_assert ((std::is_enum_v < enum_tv > && ...), "enum_cat: all types must be enums");
        static_assert ((std::is_same_v < enum_tv, enum_tv > && ...), "enum_cat: all types must be the same enum");

        using enum_t = std::common_type_t < enum_tv... >;
        using underlying_t = std::underlying_type_t < enum_t >;

        return static_cast <enum_t> ((static_cast <underlying_t> (args) | ...));
    }
}

#endif
