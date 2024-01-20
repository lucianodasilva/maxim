#pragma once
#ifndef _maxim_io_character_hpp_
#define _maxim_io_character_hpp_

#include "maxim.details.hpp"

namespace maxim {

    /// 24-bit color
    struct color {
    public:

        /// Default constructor
		constexpr color () = default;

        /// Constructor from 24-bit color
        /// \param rgb - 24-bit color
		constexpr explicit color (uint32_t rgb) : r (rgb >> 16), g (rgb >> 8), b (rgb) {}

		/// Constructor from 8-bit color components
        /// \param r - Red component
        /// \param g - Green component
        /// \param b - Blue component
        constexpr color (uint8_t r, uint8_t g, uint8_t b) : r (r), g (g), b (b) {}

        /// Calculate aproximate 8-bit color from 24-bit color
        [[nodiscard]] uint8_t to_256 () const;

        /// Calculate aproximate 4-bit color from 24-bit color
        [[nodiscard]] uint8_t to_8 () const;

        /// Convert to 24-bit color integer
        [[nodiscard]] constexpr uint32_t to_rgb() const {
			return (r << 16) | (g << 8) | b;
        }

        /// Calculate relative luminance
	    [[nodiscard]] constexpr double relative_luminance() const {
		    return 0.2126 * r / 255.0 + 0.7152 * g / 255.0 + 0.0722 * b / 255.0;
	    }

        /// Calculate contrast color
        [[nodiscard]] constexpr color contrast_color(color const& lower_bounds = { 0, 0, 0 }, color const& upper_bounds = { 255, 255, 255 }) const {
		    return relative_luminance() > 0.4 ? color {0, 0, 0} : color {255, 255, 255};
	    }

        /// Red component
        uint8_t r{};
        /// Green component
        uint8_t g{};
        /// Blue component
        uint8_t b{};
    };

    /// Calculate linear interpolation between two colors
    /// \param a - First color
    /// \param b - Second color
    /// \param t - Interpolation factor
    /// \return Interpolated color
    [[nodiscard]] constexpr color lerp(color const& a, color const& b, float t) {
        return {
            static_cast<uint8_t>(lerp(a.r, b.r, t)),
            static_cast<uint8_t>(lerp(a.g, b.g, t)),
            static_cast<uint8_t>(lerp(a.b, b.b, t)),
        };
    }

    [[nodiscard]] constexpr bool operator == (color const v1, color const v2) {
        return v1.r == v2.r && v1.g == v2.g && v1.b == v2.b;
    }

    [[nodiscard]] constexpr bool operator != (color const v1, color const v2) {
        return !operator==(v1, v2);
    }

    /// Helper color constants
    namespace colors {
        constexpr color const
            black {},
            dark_red    { 170, 0, 0 },
            dark_green  { 0, 170, 0 },
            dark_yellow { 170, 85, 0 },
            dark_blue   { 0, 0, 170 },
            purple      { 170, 0, 170 },
            dark_cyan   { 0, 170, 170 },
            gray        { 170, 170, 170},
            dark_gray   { 85, 85, 85 },
            red         { 255, 85, 85 },
            green       { 85, 255, 85 },
            yellow      { 255, 255, 85 },
            blue        { 85, 85, 255 },
            pink        { 255, 85, 255 },
            cyan        { 85, 255, 255 },
            white       { 255, 255, 255 };
    }

    /// Character attribute flags
    union attribute_flags {
        struct {
            bool bold       : 1; /// Bold if true
            bool underlined : 1; /// Underlined if true
            bool blink      : 1; /// Blinking if true
            bool reversed   : 1; /// Reversed colors if true
            int             : 0; // padding
        };

        uint8_t _bitflag {0};
    };

    [[nodiscard]] constexpr bool operator == (attribute_flags const v1, attribute_flags const v2) {
        return v1._bitflag == v2._bitflag;
    }

	[[nodiscard]] constexpr bool operator != (attribute_flags const v1, attribute_flags const v2) {
		return !operator==(v1, v2);
	}

    /// Character attributes
    struct attributes {
        color           backcolor;  /// Background color
        color           forecolor;  /// Foreground color
        attribute_flags flags;	    /// Attribute flags
		bool            is_graph;   /// Is graph character (DEC Special Character and Line Drawing Set)
    };

    [[nodiscard]] constexpr bool operator == (attributes const & v1, attributes const & v2) {
        return
            v1.backcolor == v2.backcolor &&
            v1.forecolor == v2.forecolor &&
            v1.flags     == v2.flags &&
			v1.is_graph  == v2.is_graph;
    }

    /// Character
    struct character {
		maxim::attributes   attributes; /// Character attributes
        char                content;    /// Character content
    };

    constexpr bool operator == (character const & v1, character const & v2) {
        return
            v1.attributes   == v1.attributes &&
            v1.content      == v2.content;
    }

}

#endif
