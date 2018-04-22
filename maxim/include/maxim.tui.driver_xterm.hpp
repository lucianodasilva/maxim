#pragma once
#ifndef _MAXIM_TUI_DRIVER_XTERM_HPP_
#define _MAXIM_TUI_DRIVER_XTERM_HPP_

#include "maxim.tui.character.hpp"
#include "maxim.tui.driver.hpp"
#include "maxim.tui.term_stream.hpp"

#include <limits>
#include <vector>
#include <type_traits>

namespace maxim {
    namespace tui {

        struct driver_xterm : public driver_base {
        public:

            void set_alternate_buffer (bool value) override;

            void set_cursor_enabled (bool value) override;

            graph_chars const & get_graph_chars () const override;

            void encode_and_send(
                    character_buffer const & buffer,
                    character_change_set const & changes
            ) override;

            point get_size () const override;

        private:

            struct xterm_flush_policy {
                static void flush(uint8_t const * buffer, size_t length);
            };

            term_stream < xterm_flush_policy >
                    _stream;

            void encode_move(point const & p);

            void encode_attribute_reset ();

            void encode_attribute_diff (character_flags const & current, character_flags const & past);

            void encode_color_diff (character const & current, character const & past);

        };

    }
}

#endif //_MAXIM_TUI_DRIVER_XTERM_HPP_
