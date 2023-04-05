#pragma once
#ifndef MAXIM_GRAPHICS_HPP
#define MAXIM_GRAPHICS_HPP

#include "maxim.character.hpp"
#include "maxim.character_buffer.hpp"
#include "maxim.details.hpp"

#include <tuple>
#include <string>

#ifdef _WIN32
#undef far
#undef near
#endif

namespace maxim {

    /// Alignment options for text
    enum class alignment : uint8_t {
        near,   /// Align to the near edge
        center, /// Align to the center
        far     /// Align to the far edge
    };

    /// Aligns a dimension to a range
    /// \param align The alignment to use
    /// \param dim The dimension to align
    /// \param range_near The near edge of the range
    /// \param range_far The far edge of the range
    inline int32_t align_to (alignment align, int dim, int range_near, int range_far) {
        switch (align){
            case alignment::center:
                return ((range_far - range_near) / 2) - (dim / 2);
            case alignment::far:
                return range_far - dim;
            default:
                return range_near;
        }
    }

    /// Text rendering options
    struct text_options {
        alignment       h_align; /// Horizontal alignment
        alignment       v_align; /// Vertical alignment
        maxim::flags    flags;   /// Text rendering flags
        bool            wrap;    /// Whether to wrap text
    };

    /// Character buffer graphics context
    struct graphics {
    public:

        graphics(character_buffer& buffer, rect clip, point transform);
        graphics(character_buffer& buffer, rect clip);
        graphics(character_buffer& buffer, point transform);

        explicit graphics(character_buffer& buffer);

        void clear(maxim::color clear_color = colors::BLACK);

        void draw_point (point position, maxim::color color);
        void draw_char (point position, maxim::color color, char c, maxim::flags flags = {});

        /// Draws a rectangle
        /// \param rct The rectangle to draw
        /// \param color The color to draw the rectangle
        /// \param solid If true, the rectangle will be filled with the color, otherwise it will be drawn as an outline
        void draw_box(rect rct, maxim::color color, bool solid = true);

        void draw_text(std::string_view text, point position, maxim::color color);

        void draw_text(std::string_view text, point position, int32_t width, maxim::color color, text_options options);

        void draw_text (std::string_view text, rect bounds, maxim::color color, text_options options);

        void draw_v_line(point position, int32_t length, maxim::color color, bool solid = true);

        void draw_h_line(point position, int32_t length, maxim::color color, bool solid = true);

        [[nodiscard]] point transform() const;
        void transform(point transform);

        [[nodiscard]] rect clip() const;
        void clip(rect transform);

    private:

		/// Transforms and clips a rectangle
		/// \param src The rectangle to transform and clip
		/// \return The transformed and clipped rectangle
    	[[nodiscard]]
    	inline rect transform_and_clip (rect client) const {

            // transform
            client.position += _transform;

			// intersect with clip
            if (!this->_clip.zero()) {
                client = this->_clip.intersection(client);
            }

			// intersect with buffer
            auto buffer_rect = maxim::rect{ point(), _buffer.size()};

			return buffer_rect.intersection (client);
    	}

        [[nodiscard]]
        inline std::optional < point > transform_and_clip(point client) const {
			client += _transform;

            if (!this->clip().zero() && !this->clip().contains(client)) {
                return std::nullopt;
			}

            auto buffer_rect = maxim::rect{ point(), _buffer.size() };

            if (!buffer_rect.contains(client)) {
                return std::nullopt;
            }

            return client;
		}

        character_buffer& _buffer;

        rect    _clip = {};
        point 	_transform = {};
    };
}

#endif