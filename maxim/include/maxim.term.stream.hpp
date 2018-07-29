#pragma once
#ifndef _maxim_term_stream_hpp_
#define _maxim_term_stream_hpp_

#include <iterator>
#include <limits>
#include <type_traits>
#include <vector>

namespace maxim {
    namespace term {

        struct stream {
        public:

            inline stream & write (char v) {
                _buffer.push_back(v);
                return *this;
            }

            template < class _t >
            inline std::enable_if_t <
                    !(std::is_same<
						std::remove_all_extents_t <_t>, 
						char 
					>::value) && std::numeric_limits<_t>::is_integer,
                    stream &
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
            inline stream & write (char const (&str)[n]) {
                _buffer.insert(
                        std::end(_buffer),
                        std::begin(str),
                        std::begin(str) + (n - 1)
                );

                return *this;
            }

            template < class _t >
            inline stream & optional_write(bool enabled, _t && v) {
                if (enabled)
                    write(v);

                return *this;
            }

			inline void reset() {
				_buffer.resize(0);
			}

			inline char * data() {
				return _buffer.data();
			}

			inline size_t size() const {
				return _buffer.size();
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

            std::vector < char > _buffer;
        };

    }
}

#endif
