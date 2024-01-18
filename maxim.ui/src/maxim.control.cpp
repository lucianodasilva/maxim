#include "maxim.control_container.hpp"
#include "maxim.io.hpp"
#include "maxim.application.hpp"

namespace maxim {

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

	control_container * control::parent (control_container * parent) {
		_parent = parent;
		return _parent;
	}

	void control::application (class application * application) {
		_application = application;
	}

	style & basic_control::style() {
		return const_cast < maxim::style & > (const_cast < basic_control const * > (this)->style ());
	}

	style const & basic_control::style () const {
		if (application() == nullptr) {
			return style::default_style();
		}

		return application()->style.get();
	}

	std::string_view basic_control::style_state () const {
		return (this->has_focus () ? states::FOCUSED : states::DEFAULT);
	}

	void basic_control::on_key_press (maxim::input::key_info const & key) {
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

		control::on_key_press (key);
	}

	bool basic_control::has_focus () const {
		if (this->application ()) {
			return this->application ()->current_focus() == this;
		}

		return false;
	}

	void basic_control::focus() {
		if (this->application()) {
			this->application ()->focus (this);
		}
	}

	void basic_control::on_focus() {
		focused.invoke (*this);
	}

	void basic_control::on_focus_lost () {
		focus_lost.invoke (*this);
	}

	maxim::focus_index const & control::focus_index() const {
		return _focus_index;
	}

	void control::focus_index (maxim::focus_index const & index) {
		_focus_index = index;
	}

	void basic_control::on_paint (maxim::graphics & g, maxim::rect bounds) {
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

	void basic_control::on_initialize_controls() {}

	void basic_control::on_activate () {
		// register control with focus manager
		if (this->application ()) {
			this->application ()->focus_register (this);
		}

		activated.invoke (this);
	}

	void basic_control::on_deactivate() {
		if (this->application ()) {
			this->application ()->focus_unregister (this);
		}

		deactivated.invoke (this);
	}

	bool basic_control::is_active () const {
		return this->application () && this->application ()->focus_registered (this);
	}

}