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

            virtual void set_alternate_buffer (bool value) = 0;

            virtual void set_cursor_enabled (bool value) = 0;

            virtual void encode_and_send (
                    character_buffer const & buffer,
                    character_change_set const & changes
            ) = 0;

            virtual point get_size () const = 0;
        };

        template < class _flush_policy_t >
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
                _flush_policy_t::flush(reinterpret_cast <const uint8_t * >(+str), n);
                return *this;
            }

            inline void flush () {
                _flush_policy_t::flush(
                        reinterpret_cast<uint8_t *>(_buffer.data()),
                        _buffer.size()
                );
            }

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

            std::vector < uint8_t > _buffer;
        };

    }
}

#endif //_MAXIM_TUI_DRIVER_HPP_
