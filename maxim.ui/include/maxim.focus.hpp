#pragma once
#ifndef _maxim_ui_focus_hpp_
#define _maxim_ui_focus_hpp_

#include "maxim.property.hpp"

#include <optional>
#include <set>

namespace maxim {

	/// sortable index for focusable objects
	/// @details this is a vector of digits, where each digit represents the index of a control in a control hierarchy
	/// @details the index is sorted from the root to the leaf ( depth first )
	struct focus_index {
	public:

		/// index chain
		/// @return const reference to the index chain
		std::vector < uint8_t > const & digits () const {
			return _digits;
		}

		/// check if the index is empty
		/// @return true if the index is empty, false otherwise
		[[nodiscard]] bool empty () const {
			return digits().empty();
		}

		/// create a new index by appending a digit to the current index
		/// @param digit the digit to append ( must be less than 256 )
		/// @return a new index with the digit appended
		[[nodiscard]] focus_index append (std::size_t digit) const;
	private:
		std::vector < uint8_t > _digits;
	};

	/// equality operator for focus_index
	/// @param lhs left hand side
	/// @param rhs right hand side
	/// @return true if the indices are equal, false otherwise
	inline bool operator == (focus_index const & lhs, focus_index const & rhs) {
		return lhs.digits() == rhs.digits();
	}

	/// less than operator for focus_index
	/// @details this operator is used to sort focusable objects, depth first
	/// @param lhs left hand side
	/// @param rhs right hand side
	/// @return true if lhs is less than rhs, false otherwise
	bool operator < (focus_index const & lhs, focus_index const & rhs);

	/// base class for focus managers
	class focus_manager {
	public:
		/// focus the next object
		void focus_next ();
		/// focus the previous object
		void focus_prev ();

		/// focus the specified object
		/// @param object the object to focus
		void focus (class control * object);

		/// register a focusable object
		/// @param object the object to register
		void focus_register (class control * object);

		/// unregister a focusable object
		/// @param object the object to unregister
		void focus_unregister (class control * object);

		/// checks if focus is registered for the specified object
		/// @param object the object to check
		/// @return true if the object is registered, false otherwise
		bool focus_registered (class control const * object) const;

		/// clear focusable object registry
		void focus_reset ();

		/// unfocus the current object
		void unfocus ();

		/// get the current focused object
		/// @return a pointer to the current focused object, or nullptr if no object is focused
		[[nodiscard]] class control * current_focus () const {
			return _current_focus;
		}

	private:
		/// compare functor for focusable objects
		struct focusable_compare_less {
		public:
			bool operator () (class control const * lhs, class control const * rhs) const;
		};

		/// focusable object registry type
		using focus_set_t = std::set < class control *, focusable_compare_less >;

		focus_set_t         _focus_set;
		class control *     _current_focus { nullptr };
	};

}

#endif
