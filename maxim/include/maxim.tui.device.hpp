#pragma once
#ifndef _MAXIM_TUI_DEVICE_HPP_
#define _MAXIM_TUI_DEVICE_HPP_

#include <memory>

#include "maxim.tui.character.hpp"
#include "maxim.tui.driver.hpp"
#include "maxim.tui.graphics.hpp"
#include "maxim.tui.stream.hpp"

namespace maxim {
    namespace tui {

        struct device {
        public:

            point const & get_size () const;

			maxim::tui::graphics graphics();

            void fullscreen(bool value);

            void enable_cursor (bool value);

			void clear();
            void present ();

        private:

            driver		_driver;
            point       _size           { _driver.get_size() };

            character_buffer
                        _back_buffer    { _size },
                        _front_buffer   { _size };

			stream		_stream;

        };

    }
}

#endif //_MAXIM_TUI_DEVICE_HPP_
