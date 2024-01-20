#pragma once
#ifndef _maxim_io_span_hpp_
#define _maxim_io_span_hpp_

#include <stdexcept>
#include <vector>

namespace maxim {

	/// Span class - a view of a contiguous sequence of objects
	/// \tparam type - Type of the span
    template < typename type >
    struct span {
    public:

        using value_type = type;
        using const_reference = value_type const&;
        using const_pointer = value_type const*;
        using size_type = std::size_t;
        using const_iterator = const_pointer;

		/// Default constructor
        constexpr span() noexcept = default;

		/// Copy constructor
		/// \param other - Other span
        constexpr span(span const& other) noexcept = default;

		/// Move constructor
		/// \param other - Other span
        constexpr span& operator = (span const&) = default;

		/// Construct span from pointer and size
		/// \param ptr - Pointer to the first element
		/// \param size - Size of the span
        constexpr span(const_pointer ptr, size_type size) noexcept :
            _ptr(ptr),
            _size(size)
        {}

        /// Construct span from vector
        /// \tparam others_tv - Other template parameters
        /// \param vec - Vector
        template < typename ... others_tv >
        constexpr explicit span(std::vector < value_type, others_tv... > const& vec) :
            _ptr(vec.data()),
            _size(vec.size())
        {}

		/// Construct span from array
		/// \tparam SIZE - Size of the array
		/// \param array - Array
        template < size_type SIZE >
        constexpr explicit span(value_type const (&array)[SIZE]) :
            _ptr(array),
            _size(SIZE)
        {}

		/// Get the iterator to the first element
        constexpr const_iterator begin() const noexcept {
            return this->_ptr;
        }

		/// Get the iterator to the first non owned element
        constexpr const_iterator end() const noexcept {
            return this->_ptr + _size;
        }

		/// Get the iterator to the first element
        constexpr const_iterator cbegin() const noexcept {
            return this->_ptr;
        }

		/// Get the iterator to the first non owned element
        constexpr const_iterator cend() const noexcept {
            return this->_ptr + _size;
        }

		/// Get the reference to the element at the given position
		/// \param POS - Position
		/// \return Reference to the element at the given position
		/// \note No bounds checking is performed
        constexpr const_reference operator [](std::size_t const POS) const noexcept {
            return *(this->_ptr + POS);
        }

		/// Get the reference to the element at the given position
		/// \param POS - Position
		/// \return Reference to the element at the given position
		/// \throws std::out_of_range if the position is out of range
        constexpr const_reference at(std::size_t const POS) const {
            if (POS >= this->_size) {
                throw std::out_of_range("view::at: pos value out of range");
            }

            return *(this->_ptr + POS);
        }

		/// Get the reference to the first element
        constexpr const_reference front() const noexcept {
            return *(this->_ptr);
        }

		/// Get the reference to the last element
        constexpr const_reference back() const noexcept {
            return *(this->_ptr + (this->_size - 1));
        }

		/// Get the pointer to the first element
        constexpr const_pointer data() const noexcept {
            return _ptr;
        }

		/// Get the size of the span
        [[nodiscard]] constexpr size_type size() const noexcept {
            return _size;
        }

		/// Check if the span is empty
        [[nodiscard]] constexpr bool empty() const noexcept {
            return this->_size == 0;
        }

		/// Swap the span pointers with another span
		/// \param other - Other span
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