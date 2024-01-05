#include "maxim.control.hpp"
#include "maxim.io.hpp"
#include "maxim.application.hpp"

namespace maxim {

	style & control::style() {
		return const_cast < maxim::style & > (const_cast < control const * > (this)->style ());
	}

	style const & control::style () const {
		if (_application == nullptr) {
			return style::default_style();
		}

		return _application->style.get();
	}

	std::string_view control::style_state () const {
		return (this->has_focus () ? states::FOCUSED : states::DEFAULT);
	}

	void control::on_key_press (maxim::input::key_info const & key) {
		if (key.matches (input::virtual_key::tab, input::modifier_key::none)) {
			if (application()) {
				application()->focus_next();
			}

			return;
		}

		if (key.matches (input::virtual_key::tab, input::modifier_key::shift)) {
			if (application()) {
				application()->focus_prev();
			}

			return;
		}

		focusable < control >::on_key_press (key);
	}

	bool control::has_focus () const {
		if (this->application ()) {
			return this->application ()->current_focus() == this;
		}

		return false;
	}

	void control::focus() {
		if (this->application()) {
			this->application ()->focus (this);
		}
	}

	void control::on_paint (maxim::graphics & g, maxim::rect bounds) {
		maxim::color back_color;
		maxim::color border_color;
		bool border;

		this->style_reader ()
			.get (properties::BACKGROUND, back_color)
			.get (properties::BORDER_COLOR, border_color)
			.get (properties::BORDER, border);

		g.draw_box (bounds, back_color);

		if (border) {
			g.draw_box (bounds, border_color, false);
		}
	}

	void control::on_initialize_controls() {}

	void control::on_activate () {
		// register control with focus manager
		if (this->application ()) {
			this->application ()->focus_register (this);
		}

		activated.invoke (this);
	}

	void control::on_deactivate() {
		deactivated.invoke (this);
	}

	control_container * control::parent () const {
		return _parent;
	}

	application * control::application () const {
		return _application;
	}

	control * control::root () {
		return _parent ? _parent->root () : this;
	}

	control const * control::root () const {
		return _parent ? _parent->root () : this;
	}

	bool control::is_active () const {
		return this->application () && this->application ()->active () == this->root ();
	}

	void control::attach_to_tree (class application * app, class control_container * parent) {
		this->application (app);
		this->parent (parent);

		this->on_initialize_controls();
	}

	void control_container::on_initialize_controls () {
		control::on_initialize_controls();

		for (auto & child : _controls) {
			child->on_initialize_controls ();
		}
	}

	void control_container::on_activate () {
		control::on_activate ();

		for (auto & child : _controls) {
			child->on_activate ();
		}
	}

	void control_container::on_deactivate () {
		control::on_deactivate ();

		for (auto & child : _controls) {
			child->on_deactivate ();
		}
	}

	void control_container::on_paint (maxim::graphics & g, maxim::rect bounds) {
		control::on_paint (g, bounds);

		for (auto & child : _controls) {
			child->on_paint (g, bounds);
		}
	}

	void control_container::push_back_control (std::unique_ptr < control > && control) {
		// move control ownership to this container
		auto & uptr = *_controls.emplace_back (std::move (control));

		// set focus index
		uptr.focus_index (this->focus_index().append(_controls.size() - 1));

		// attach to control hierarchy tree
		uptr.attach_to_tree(this->application(), this);

		// if we are active already, activate the new control
		if (this->is_active()) {
			uptr.on_activate();
		}
	}

	void basic_root_control::close () {
		if (this->application () && this->application()->active() == this) {
			this->application()->pop_active();
		}
	}

}