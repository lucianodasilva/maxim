#pragma once
#ifndef _maxim_io_span_hpp_
#define _maxim_io_span_hpp_

#include <vector>

namespace maxim {

    template < typename type >
    struct span {
    public:

        using value_type = type;
        using const_reference = value_type const&;
        using const_pointer = value_type const*;
        using size_type = std::size_t;
        using const_iterator = const_pointer;

        constexpr span() noexcept = default;
        constexpr span(span const& other) noexcept = default;
        constexpr span& operator = (span const&) = default;

        constexpr span(const_pointer ptr, size_type size) noexcept :
            _ptr(ptr),
            _size(size)
        {}

        /// Construct span from vector
        template < typename ... others_tv >
        constexpr explicit span(std::vector < value_type, others_tv... > const& vec) :
            _ptr(vec.data()),
            _size(vec.size())
        {}

        template < size_type SIZE >
        constexpr explicit span(value_type const (&array)[SIZE]) :
            _ptr(array),
            _size(SIZE)
        {}

        constexpr const_iterator begin() const noexcept {
            return this->_ptr;
        }

        constexpr const_iterator end() const noexcept {
            return this->_ptr + _size;
        }

        constexpr const_iterator cbegin() const noexcept {
            return this->_ptr;
        }

        constexpr const_iterator cend() const noexcept {
            return this->_ptr + _size;
        }

        constexpr const_reference operator [](std::size_t const POS) const noexcept {
            return *(this->_ptr + POS);
        }

        constexpr const_reference at(std::size_t const POS) const {
            if (POS >= this->_size) {
                throw std::out_of_range("view::at: pos value out of range");
            }

            return *(this->_ptr + POS);
        }

        constexpr const_reference front() const noexcept {
            return *(this->_ptr);
        }

        constexpr const_reference back() const noexcept {
            return *(this->_ptr + (this->_size - 1));
        }

        constexpr const_pointer data() const noexcept {
            return _ptr;
        }

        [[nodiscard]]
        constexpr size_type size() const noexcept {
            return _size;
        }

        [[nodiscard]]
        constexpr bool empty() const noexcept {
            return this->_size == 0;
        }

        constexpr void swap(span& other) noexcept {
            std::swap(_ptr, other._ptr);
            std::swap(_size, other._size);
        }

    private:
        const_pointer   _ptr = nullptr;
        size_type       _size = 0;
    };

}

#endif