#include "maxim.button.hpp"

namespace maxim {

	std::string_view button::default_style_class_name() const {
		return button::DEFAULT_STYLE_CLASS_NAME;
	}

	std::string_view button::style_state() const {
		if (is_pressed()) {
			return states::PRESSED;
		}

		return control::style_state();
	}

	void button::on_pressed() {
		pressed.invoke(*this);
	}

	void button::on_key_down(maxim::input::key_info const &key) {
		if (key.key == maxim::input::virtual_key::enter || key.key == maxim::input::virtual_key::space) {
			_pressed = true;
			on_pressed();
		}
	}

	void button::on_key_up(maxim::input::key_info const &key) {
		if (key.key == maxim::input::virtual_key::enter || key.key == maxim::input::virtual_key::space) {
			_pressed = false;
		}
	}

}