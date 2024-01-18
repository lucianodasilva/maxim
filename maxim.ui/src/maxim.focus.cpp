#include "maxim.focus.hpp"
#include "maxim.control.hpp"

#include <stdexcept>

namespace maxim {

	[[nodiscard]] focus_index focus_index::append (std::size_t digit) const {
		if (digit > std::numeric_limits < uint8_t >::max ()) {
			throw std::runtime_error ("focus_index::append: digit is too large");
		}

		auto copy = *this;
		copy._digits.push_back (digit);

		return copy;
	}

	bool operator < (focus_index const & lhs, focus_index const & rhs){
		// cover empty cases
		if (lhs.empty()) {
			return !rhs.empty();
		}

		if (rhs.empty()) {
			return false;
		}

		// iterate over both digit vectors
		auto lhs_it = lhs.digits().begin();
		auto rhs_it = rhs.digits().begin();

		while (lhs_it != lhs.digits().end() && rhs_it != rhs.digits().end()) {
			if (*lhs_it < *rhs_it) {
				return true;
			}

			if (*lhs_it > *rhs_it) {
				return false;
			}

			++lhs_it;
			++rhs_it;
		}

		// if we reach this point, the vectors are equal up to the length of the shorter one
		return lhs_it == lhs.digits().end() && rhs_it != rhs.digits().end();
	}

	void focus_manager::focus_next() {
		if (_focus_set.empty ()) {
			// we have no objects, so we can't focus anything
			return;
		}

		if (_current_focus) {
			// find the current focused object
			auto it = _focus_set.find (_current_focus);

			// check if we have data integrity
			if (it == _focus_set.end ()) {
				throw std::runtime_error ("current focus not found in focus set");
			}

			// find the next object with tab_stop set to true
			// start out by looking at the next object
			auto next = std::find_if (std::next (it), _focus_set.end (), [] (auto * obj) {
				return obj->tab_stop.get ();
			});

			// if we didn't find anything, wrap around
			if (next == _focus_set.end ()) {
				next = std::find_if (_focus_set.begin (), it, [] (auto * obj) {
					return obj->tab_stop.get ();
				});
			}

			// if we found something, focus it
			if (next != _focus_set.end ()) {
				this->focus (*next);
			} else {
				// otherwise, unfocus
				this->unfocus ();
			}
		} else {
			// if we have no current focus, focus the first object with tab_stop set to true
			auto it = std::find_if (_focus_set.begin (), _focus_set.end (), [] (auto * obj) {
				return obj->tab_stop.get ();
			});

			// if we found something, focus it
			if (it != _focus_set.end ()) {
				this->focus (*it);
			}
		}
	}

	void focus_manager::focus_prev() {
		if (_focus_set.empty ()) {
			// we have no objects, so we can't focus anything
			return;
		}

		if (_current_focus) {
			// find the current focused object
			auto it = _focus_set.find (_current_focus);

			// check if we have data integrity
			if (it == _focus_set.end ()) {
				throw std::runtime_error ("current focus not found in focus set");
			}

			// find the next object with tab_stop set to true
			// start out by looking at the previous object
			auto prev = std::find_if (std::make_reverse_iterator (it), _focus_set.rend (), [] (auto * obj) {
				return obj->tab_stop.get ();
			});

			// if we didn't find anything, wrap around
			if (prev == _focus_set.rend ()) {
				prev = std::find_if (_focus_set.rbegin (), std::make_reverse_iterator (it), [] (auto * obj) {
					return obj->tab_stop.get ();
				});
			}

			// if we found something, focus it
			if (prev != _focus_set.rend ()) {
				this->focus (*prev);
			} else {
				// otherwise, unfocus
				this->unfocus ();
			}
		} else {
			// if we have no current focus, focus the first object with tab_stop set to true
			auto it = std::find_if (_focus_set.rbegin (), _focus_set.rend (), [] (auto * obj) {
				return obj->tab_stop.get ();
			});

			// if we found something, focus it
			if (it != _focus_set.rend ()) {
				this->focus (*it);
			}
		}
	}

	void focus_manager::focus (control * object) {
		// if no object is passed, unfocus
		if (object == nullptr) {
			this->unfocus ();
			return;
		}

		// if nothing changes, do nothing
		if (_current_focus == object) {
			return;
		}

		// double check it belongs to us
		if (_focus_set.find (object) == _focus_set.end ()) {
			// TODO: rething if this should be an error or simply ignored
			throw std::runtime_error ("focus_manager_base::focus: focusable object instance does not belong to this manager");
		}

		// unfocus current
		this->unfocus();

		// focus new
		_current_focus = object;

		// notify
		if (_current_focus) {
			_current_focus->on_focus ();
		}
	}

	void focus_manager::focus_register (maxim::control * object) {
		if (object == nullptr) {
			return;
		}

		if (object->focus_index().empty()) {
			throw std::runtime_error ("focus_manager_base::focus_register: cannot register focusable object with empty focus index");
		}

		_focus_set.insert (object);
	}

	void focus_manager::focus_unregister (maxim::control * object) {
		if (object == nullptr) {
			return;
		}

		_focus_set.erase (object);
	}

	bool focus_manager::focus_registered (maxim::control const * object) const {
		return _focus_set.find (const_cast < maxim::control * > (object)) != _focus_set.end ();
	}

	void focus_manager::focus_reset () {
		// unset focus
		this->unfocus ();

		// clear set
		_focus_set.clear ();
	}

	void focus_manager::unfocus () {
		if (_current_focus) {
			_current_focus->on_focus_lost ();
			_current_focus = nullptr;
		}
	}

	bool focus_manager::focusable_compare_less::operator () (control const * lhs, control const * rhs) const {
		return lhs->focus_index() < rhs->focus_index();
	}

}