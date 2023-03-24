#pragma once
#ifndef MAXIM_STREAM_HPP
#define MAXIM_STREAM_HPP

#include <iterator>
#include <limits>
#include <type_traits>
#include <vector>

#include "maxim.details.hpp"

namespace maxim {

    constexpr std::size_t STREAM_TRIM_THRESHOLD = 1024;

    /// Function to count the number of digits in a number
    /// \param value The number to count the digits of
    /// \ return The number of digits in the number
    template<
        typename value_t, 
        std::enable_if_t < std::is_integral_v < std::decay_t < value_t >>, int > = 0 >
    [[nodiscard]] 
    inline std::size_t digit_count(value_t value) {
        if (value == 0) {
            return 1;
        }

        std::size_t digits = 0;

        if (value < 0) {
            digits = 1;
        }

        while (value) {
            value /= 10;
            ++digits;
        }

        return digits;
    }

    struct stream {
    public:

        inline stream & write(char value) {
            _buffer.push_back(value);
            return *this;
        }

        template<
            typename value_t,
            std::enable_if_t < 
                std::is_integral_v < std::decay_t < value_t > > &&
                !std::is_same_v < std::decay_t < value_t >, char >,
                int > = 0 >
        inline stream & write(value_t value) {
            auto count = digit_count(value);

            if (count == 0) {
                return *this;
            }

            std::size_t const INDEX = _buffer.size();
            // resize for new string
            _buffer.resize(_buffer.size() + count);

            if (value < value_t(0)) {
                _buffer[INDEX] = '-';
            }

            while (count) {
                _buffer[INDEX + (count - 1)] = '0' + char(value % 10);
                value /= 10;
                --count;
            }

            return *this;
        }

        template<std::size_t COUNT>
        inline stream & write(char const (&str)[COUNT]) { // NOLINT: signature is not "array" usage
            if constexpr (COUNT == 0) {
                return *this;
            }

            _buffer.insert(
                    std::end(_buffer),
                    std::begin(str),
                    std::begin(str) + (COUNT - 1)
            );

            return *this;
        }

        template<typename ... args_t>
        inline stream &write_sequence(args_t &&... args) {
            (write(args), ...);
            return *this;
        }


        template<typename value_t>
        inline stream &maybe_write(bool enabled, value_t && value) {
            if (enabled) {
                write(value);
            }

            return *this;
        }

        inline void reset() {
            _buffer.resize(0);

            if (_buffer.capacity() > STREAM_TRIM_THRESHOLD) {
                _buffer.clear();
            }
        }

        inline maxim::view <char> data() {
            return view(_buffer);
        }

    private:

        std::vector<char> _buffer;
    };
}

#endif
