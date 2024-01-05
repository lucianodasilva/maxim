#pragma once
#include <limits>
#ifndef MAXIM_CHARACTER_HPP
#define MAXIM_CHARACTER_HPP

#include <cinttypes>
#include <cmath>
#include <vector>
#include <tuple>

#include "maxim.details.hpp"

namespace maxim {

    struct color {
    public:
        uint8_t r, g, b;

        [[nodiscard]] uint8_t to_ansi_256 () const;
        [[nodiscard]] uint8_t to_ansi_8 () const;
    };

    /// Linearly interpolates between two values
    /// \param LHV The left hand value
    /// \param RHV The right hand value
    /// \param THETA The interpolation factor
    /// \return The interpolated value
    [[nodiscard]]
    inline float lerp (float const LHV, float const RHV, float const THETA) {
        return LHV + (RHV - LHV) * THETA;
    }

    /// Linearly interpolates between two colors
    /// \param LHV The left hand value
    /// \param RHV The right hand value
    /// \param THETA The interpolation factor
    /// \return The interpolated value
    [[nodiscard]]
    inline color lerp (color const LHV, color const RHV, float const THETA) {
        return {
            static_cast<uint8_t>(lerp (LHV.r, RHV.r, THETA)),
            static_cast<uint8_t>(lerp (LHV.g, RHV.g, THETA)),
            static_cast<uint8_t>(lerp (LHV.b, RHV.b, THETA))
        };
    }

    [[nodiscard]] 
    inline bool operator == (color const LHV, color const RHV) {
        return LHV.r == RHV.r && LHV.g == RHV.g && LHV.b == RHV.b;
    }

    [[nodiscard]]
    inline bool operator != (color const LHV, color const RHV) {
        return !operator==(LHV, RHV);
    }

    namespace colors {

        constexpr color const
            BLACK {},
            DARK_RED    {170, 0, 0 },
            DARK_GREEN  {0, 170, 0 },
            DARK_YELLOW {170, 85, 0 },
            DARK_BLUE   { 0, 0, 170 },
            PURPLE      { 170, 0, 170 },
            DARK_CYAN   { 0, 170, 170 },
            GRAY        { 170, 170, 170},
            DARK_GRAY   { 85, 85, 85 },
            RED         { 255, 85, 85 },
            GREEN       { 85, 255, 85 },
            YELLOW      { 255, 255, 85 },
            BLUE        { 85, 85, 255 },
            PINK        { 255, 85, 255 },
            CYAN        { 85, 255, 255 },
            WHITE       { 255, 255, 255 };
    }

    union flags {
        struct {
            bool bold       : 1;
            bool underlined : 1;
            bool blink      : 1;
            bool reversed   : 1;
            bool is_graph   : 1;
            int             : 0; // padding
        };

        uint8_t flags_as_int {0};
    };

    [[nodiscard]] 
    inline bool operator == (flags const lhv, flags const rhv) {
        return lhv.flags_as_int == rhv.flags_as_int;
    }

    [[nodiscard]]
    inline bool operator != (flags const lhv, flags const rhv) {
        return !operator==(lhv, rhv);
    }

    struct character {
        color           backcolor = {};
        color           forecolor = {};
        maxim::flags    flags = {};
        char            content = ' ';
    };

    [[nodiscard]]
    inline bool has_same_colors (character const& lhv, character const& rhv) {
		return lhv.backcolor == rhv.backcolor && lhv.forecolor == rhv.forecolor;
    }

	[[nodiscard]]
    inline bool operator == (character const & lhv, character const & rhv) {
        return 
            has_same_colors(lhv, rhv) &&
            lhv.flags == rhv.flags &&
            lhv.content == rhv.content;
    }

}

#endif
