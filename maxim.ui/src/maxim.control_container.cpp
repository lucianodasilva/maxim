#include "maxim.control_container.hpp"
#include "maxim.application.hpp"

namespace maxim {

	void control_container::on_control_added(maxim::control * object, std::size_t index) {
		// set focus index
		object->focus_index (this->focus_index().append(index));

		// if we are active already, activate the new control
		if (this->is_active()) {
			object->on_activate();
		}

		// attach to control hierarchy tree
		object->attach_to_tree(this->application(), this);
	}

	void basic_control_container::on_initialize_controls () {
		basic_control::on_initialize_controls();

		this->accept ([](auto * control) {
			control->on_initialize_controls ();
			return true;
		});
	}

	void basic_control_container::on_activate () {
		basic_control::on_activate ();

		this->accept ([](auto * control) {
			control->on_activate ();
			return true;
		});
	}

	void basic_control_container::on_deactivate () {
		basic_control::on_deactivate ();

		this->accept ([](auto * control) {
			control->on_deactivate ();
			return true;
		});
	}

	void basic_root_control::close () {
		if (this->application () && this->application()->active() == this) {
			this->application()->pop_active();
		}
	}

}