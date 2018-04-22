#pragma once
#ifndef _MAXIM_TUI_DRIVER_HPP_
#define _MAXIM_TUI_DRIVER_HPP_

#include <limits>
#include "maxim.tui.character.hpp"
#include "maxim.tui.character_change_set.hpp"

namespace maxim {
    namespace tui {

        class driver_base {
        public:

            virtual ~driver_base() = default;

            virtual void set_alternate_buffer (bool value) = 0;

            virtual void set_cursor_enabled (bool value) = 0;

            virtual graph_chars const & get_graph_chars () const = 0;

            virtual void encode_and_send (
                    character_buffer const & buffer,
                    character_change_set const & changes
            ) = 0;

            virtual point get_size () const = 0;
        };

    }
}

#endif //_MAXIM_TUI_DRIVER_HPP_
