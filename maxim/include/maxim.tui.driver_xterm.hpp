#pragma once
#ifndef _MAXIM_TUI_DRIVER_XTERM_HPP_
#define _MAXIM_TUI_DRIVER_XTERM_HPP_

#include "maxim.tui.character.hpp"

#include <limits>
#include <vector>
#include <type_traits>

namespace maxim {
    namespace tui {

        struct out_stream {
        public:

            out_stream & write (char v) {
                _buffer.push_back(v);
                return *this;
            }

            template < class _t >
            inline std::enable_if_t <
                    !(std::is_same<std::remove_all_extents_t <_t>, char >::value) && std::numeric_limits<_t>::is_integer,
                    out_stream &
            > write (_t v) {
                auto count = digit_count(v);

                size_t index = _buffer.size();
                // resize for new string
                _buffer.resize(_buffer.size() + count);

                if (v < _t(0))
                    _buffer[index] = '-';

                while(v) {
                    _buffer[index + (count - 1)] = '0' + char (v % 10);
                    v /= 10;
                    --count;
                }

                return *this;
            }

            template < size_t n >
            out_stream & write (char const (&str)[n]) {
                _buffer.insert(
                        std::end(_buffer),
                        std::begin(str),
                        std::begin(str) + (n - 1)
                );

                return *this;
            }

            template < class _t >
            out_stream & optional_write(bool enabled, _t && v) {
                if (enabled)
                    write(v);

                return *this;
            }

            template < size_t n >
            out_stream & immediate_write(char const (&str)[n]) {
                flush(reinterpret_cast <const uint8_t * >(+str), n);
                return *this;
            }

            void flush ();

        private:

            template < class _t >
            static inline std::enable_if_t <std::numeric_limits<_t>::is_integer, size_t >
                digit_count (_t v)
            {
                size_t digits = 0;

                if (v < 0)
                    digits = 0;

                while (v) {
                    v /= 10;
                    ++digits;
                }

                return digits;
            }

            void flush(uint8_t const * buffer, size_t size);

            std::vector < uint8_t > _buffer;
        };

        struct driver_xterm {
        public:

            bool alternate_buffer (bool value);

            bool alternate_buffer () const;

            bool enable_cursor (bool value);

            bool enable_cursor () const;

            void encode_changes (
                    character_buffer const & buffer,
                    std::vector < character_span > const & changes
            );

            point get_size () const;

        private:

            out_stream
                    _stream;

            bool
                    _alternate_buffer = false,
                    _enable_cursor = true;

            void encode_move(point const & p);

            void encode_attributes(character_flags const & current);
            void encode_attribute_reset ();

            void encode_attribute_diff (character_flags const & current, character_flags const & past);

            void encode_color (character const & current);

            void encode_color_diff (character const & current, character const & past);

            void write_out(char const * buffer) const;
            void write_out(char const * buffer, uint32_t length) const;

        };

    }
}

#endif //_MAXIM_TUI_DRIVER_XTERM_HPP_
