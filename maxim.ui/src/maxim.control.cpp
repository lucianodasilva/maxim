#include "maxim.control_container.hpp"
#include "maxim.io.hpp"
#include "maxim.application.hpp"
#include "maxim.control.hpp"


namespace maxim {

	void control::focus_index (maxim::focus_index const & index) {
		_focus_index = index;
	}

	maxim::focus_index const & control::focus_index() const {
		return _focus_index;
	}

	control_container * control::parent () const {
		return _parent;
	}

	application * control::application () const {
		return _application;
	}

	control * control::root () {
		return _parent != nullptr ? _parent->root () : this;
	}

	control const * control::root () const {
		return _parent != nullptr ? _parent->root () : this;
	}

	bool control::is_active () const {
		return
			this->application () != nullptr &&
			this->application ()->active () == this->root ();
	}

	void control::attach_to_tree (class application * app, class control_container * parent) {
		this->_application = app;
		this->_parent = parent;

		this->on_initialize_controls();
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
			if (application() != nullptr) {
				application()->focus_next();
			}

			return;
		}

		if (key.matches (input::virtual_key::tab, input::modifier_key::shift)) {
			if (application() != nullptr) {
				application()->focus_prev();
			}

			return;
		}

		control::on_key_press (key);
	}

	bool basic_control::has_focus () const {
		if (this->application () != nullptr) {
			return this->application ()->current_focus() == this;
		}

		return false;
	}

	void basic_control::focus() {
		if (this->application() != nullptr) {
			this->application ()->focus (this);
		}
	}

	void basic_control::on_focus() {
		focused.invoke (*this);
	}

	void basic_control::on_focus_lost () {
		focus_lost.invoke (*this);
	}

	void basic_control::on_paint (maxim::graphics & g, maxim::size size) {
		auto bounds	= maxim::rect { {}, size };

		g.draw_rect(bounds, background.get());

		if (border.get()) {
			g.draw_rect (bounds, border_color.get(), false);
		}
	}

	void basic_control::on_initialize_controls() {
		this->style_class_name_reset();

		this->bind_style_property (background, properties::BACKGROUND);
		this->bind_style_property (border_color, properties::BORDER_COLOR);
		this->bind_style_property (border, properties::BORDER, false);
	}

	void basic_control::on_activate () {
		// register control with focus manager
		if (this->application () != nullptr) {
			this->application ()->focus_register (this);
		}

		activated.invoke (this);
	}

	void basic_control::on_deactivate() {
		if (this->application () != nullptr) {
			this->application ()->focus_unregister (this);
		}

		deactivated.invoke (this);
	}

	bool basic_control::is_active () const {
		return
			this->application () != nullptr &&
			this->application ()->focus_registered (this);
	}
}