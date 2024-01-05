#pragma once
#ifndef MAXIM_FOCUS_HPP
#define MAXIM_FOCUS_HPP

#include "maxim.key_handler.hpp"
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
		inline std::vector < uint8_t > const & digits () const {
			return _digits;
		}

		/// check if the index is empty
		/// @return true if the index is empty, false otherwise
		[[nodiscard]] inline bool empty () const {
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

	/// base class for focusable objects
	class focusable_base : public key_handler_base {
	public:

		/// focus lost notification
		virtual void on_focus_lost () = 0;
		/// focus gained notification
		virtual void on_focus () = 0;

		/// get the focus index of this object
		/// @return a const reference to the focus index
		[[nodiscard]]
		inline maxim::focus_index const & focus_index () const {
			return _focus_index;
		}

		/// set the focus index of this object
		/// @param index the new focus index
		inline void focus_index (maxim::focus_index const & index) {
			_focus_index = index;
		}

		property < bool > tab_stop { false };
	private:
		maxim::focus_index _focus_index;
	};

	template < typename sender_type >
	using focus_event = event < sender_type & /* sender */ >;

	template < typename sender_type, typename base_type = focusable_base /* work around virtual inheritance */ >
	class focusable : public key_handler < sender_type, base_type > {
	public:

		/// focus lost event
		focus_event < sender_type > focus_lost;
		/// focus gained event
		focus_event < sender_type > focus;

		void on_focus_lost () override {
			focus_lost.invoke (*static_cast < sender_type * > (this));
		}

		void on_focus () override {
			focus.invoke (*static_cast < sender_type * > (this));
		}
	};

	/// base class for focus managers
	class focus_manager {
	public:
		/// focus the next object
		void focus_next ();
		/// focus the previous object
		void focus_prev ();

		/// focus the specified object
		void focus (focusable_base * object);

		/// register a focusable object
		void focus_register (focusable_base * object);
		/// clear focusable object registry
		void focus_reset ();
		/// unfocus the current object
		void unfocus ();

		/// get the current focused object
		inline focusable_base * current_focus () const {
			return _current_focus;
		}

	private:
		/// compare functor for focusable objects
		struct focusable_compare_less {
		public:
			inline bool operator () (focusable_base const * lhs, focusable_base const * rhs) const {
				return lhs->focus_index() < rhs->focus_index();
			}
		};

		/// focusable object registry type
		using focus_set_t = std::set < focusable_base *, focusable_compare_less >;

		focus_set_t         _focus_set;
		focusable_base *    _current_focus { nullptr };
	};

}

#endif
