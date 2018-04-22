#pragma once
#ifndef _MAXIM_TUI_DEVICE_HPP_
#define _MAXIM_TUI_DEVICE_HPP_

#include <memory>

#include "maxim.tui.character.hpp"
#include "maxim.tui.driver.hpp"

namespace maxim {
    namespace tui {

        struct device {
        public:

            point const & get_size () const;

            character_buffer & back_buffer ();

            device (std::unique_ptr < driver_base > driver);

            void fullscreen(bool value);

            void enable_cursor (bool value);

            void clear();

            void draw_box (
                    point const & position,
                    point const & size,
                    color_t color,
                    bool solid = true,
                    color_t backcolor = colors::black
            );

            void draw_text (
                    std::string const & text,
                    point const & position,
                    color_t color = colors::white,
                    color_t back_color = colors::black,
                    character_flags const & flags = {}
            );

            void draw_v_line (
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

            void present ();

        private:

            std::unique_ptr <driver_base>
                        _driver;
            point       _size           { _driver->get_size() };

            character_buffer
                        _back_buffer    { _size },
                        _front_buffer   { _size };



        };

    }
}

#endif //_MAXIM_TUI_DEVICE_HPP_
