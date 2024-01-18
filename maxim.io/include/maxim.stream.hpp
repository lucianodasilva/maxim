#pragma once
#ifndef _maxim_io_stream_hpp_
#define _maxim_io_stream_hpp_

#include <iterator>
#include <limits>
#include <type_traits>
#include <vector>

#include "maxim.details.hpp"

namespace maxim {

    constexpr std::size_t stream_trim_threshold = 1024;

    template < typename _t, typename _common_t = std::decay_t < _t > >
    [[nodiscard]] inline std::enable_if_t <
        std::is_integral_v < _common_t >,
        size_t
    > digit_count (_t v)
    {
        if (v == 0)
            return 1;

        size_t digits = 0;

        if (v < 0)
            digits = 1;

        while (v) {
            v /= 10;
            ++digits;
        }

        return digits;
    }

    struct stream {
    public:

        inline stream & write (char v) {
            _buffer.push_back(v);
            return *this;
        }

        template < typename _t, typename _common_t = std::decay_t<_t> >
        inline std::enable_if_t <
            !std::is_same_v < _common_t , char > &&
            std::is_integral_v < _common_t >,
            stream &
        > write (_t v) {
            auto count = digit_count(v);

            if (count == 0)
                return *this;

            size_t index = _buffer.size();
            // resize for new string
            _buffer.resize(_buffer.size() + count);

            if (v < _t(0))
                _buffer[index] = '-';

            while(count) {
                _buffer[index + (count - 1)] = '0' + char (v % 10);
                v /= 10;
                --count;
            }

            return *this;
        }

        template < size_t _n >
        inline stream & write (char const (&str)[_n]) {
            if constexpr (_n == 0) {
                return *this;
            }

            _buffer.insert(
                    std::end(_buffer),
                    std::begin(str),
                    std::begin(str) + (_n - 1)
            );

            return *this;
        }

        template < typename ... _tv >
        inline stream & write_sequence (_tv && ... args) {
            (write (args), ...);
            return *this;
        }


        template < typename _t >
        inline stream & maybe_write(bool enabled, _t && v) {
            if (enabled)
                write(v);

            return *this;
        }

        inline void reset() {
            _buffer.resize(0);

            if (_buffer.capacity () > stream_trim_threshold)
                _buffer.clear ();
        }

        inline span < char > data() {
            return  span {_buffer};
        }

    private:

        std::vector < char > _buffer;
    };
}

#endif
