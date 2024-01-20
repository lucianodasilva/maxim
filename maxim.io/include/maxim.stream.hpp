#pragma once
#ifndef _maxim_io_stream_hpp_
#define _maxim_io_stream_hpp_

#include <iterator>
#include <limits>
#include <type_traits>
#include <vector>

#include "maxim.details.hpp"

namespace maxim {

    constexpr std::size_t STREAM_TRIM_THRESHOLD = 1024;

	/// Count the number of digits in an integer
	/// \tparam type - Type of the integer
	/// \tparam common_type - Inferred common type of the integer
	/// \param v - Integer
	/// \return Number of digits
    template < typename type, typename common_type = std::decay_t < type > >
    [[nodiscard]] inline std::enable_if_t < std::is_integral_v < common_type >, size_t > digit_count (type v)
    {
        if (v == 0) {
	        return 1;
        }

        size_t digits = 0;

        if (v < 0) {
	        digits = 1;
        }

        while (v) {
            v /= 10;
            ++digits;
        }

        return digits;
    }

	/// Stream class - a class for writing data to a buffer
    struct stream {
    public:

		/// Write char to the stream
        inline stream & write (char v) {
            _buffer.push_back(v);
            return *this;
        }

		/// Write an integer to the stream
		/// \tparam type - Type of the integer
		/// \tparam common_type - Inferred common type of the integer
		/// \param v - Integer
		/// \return Reference to the stream
        template < typename type, typename common_type = std::decay_t<type> >
        inline std::enable_if_t <
	        !std::is_same_v < common_type , char > &&
	        std::is_integral_v < common_type >,
            stream &
        > write (type v) {
            auto count = digit_count(v);

            if (count == 0) {
	            return *this;
            }

            size_t index = _buffer.size();
            // resize for new string
            _buffer.resize(_buffer.size() + count);

            if (v < type(0)) {
	            _buffer[index] = '-';
            }

            while(count) {
                _buffer[index + (count - 1)] = '0' + char (v % 10);
                v /= 10;
                --count;
            }

            return *this;
        }

		/// Write a const char array to the stream
        template < size_t SIZE >
        inline stream & write (char const (&str)[SIZE]) {
            if constexpr (SIZE == 0) {
                return *this;
            }

            _buffer.insert(
                    std::end(_buffer),
                    std::begin(str),
                    std::begin(str) + (SIZE - 1)
            );

            return *this;
        }

		/// Write a sequence of arguments to the stream
		/// \tparam args_tv - Types of the arguments
		/// \param args - Arguments
        template < typename ... args_tv >
        inline stream & write_sequence (args_tv && ... args) {
            (write (args), ...);
            return *this;
        }

		/// Optionally a sequence of arguments to the stream
		/// \tparam type - Value type
		/// \param enabled - Enabled
		/// \param v - value
        template < typename type >
        inline stream & maybe_write(bool enabled, type && v) {
            if (enabled) {
	            write(v);
            }

            return *this;
        }

		/// Reset the stream
        inline void reset() {
            _buffer.resize(0);

            if (_buffer.capacity () > STREAM_TRIM_THRESHOLD) {
	            _buffer.clear();
            }
        }

		/// Get a span for the stream buffer
        inline span < char > data() {
            return  span {_buffer};
        }

    private:
        std::vector < char > _buffer;
    };
}

#endif
