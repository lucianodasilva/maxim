#include "maxim.checkbox.hpp"

namespace maxim {

	std::string_view checkbox::default_style_class_name() const {
		return checkbox::DEFAULT_STYLE_CLASS_NAME;
	}

	void checkbox::on_paint(maxim::graphics &g, maxim::rect bounds) {
		control::on_paint(g, bounds);

		maxim::color            text_color;
		maxim::text_attributes  text_attributes;

		this->style_reader()
			.get(properties::TEXT_COLOR, text_color)
			.get(properties::TEXT_ATTRIBUTES, text_attributes);

		// TODO: dude... this is so ugly
		constexpr auto UNSET_CHECK = "[ ]";
		constexpr auto SET_CHECK = "[X]";
		constexpr auto CHECK_WIDTH = 3;

		auto check_bounds = maxim::rect { bounds.left(), bounds.top(), CHECK_WIDTH, bounds.height() };
		check_bounds = bounds.intersection (check_bounds);

		g.draw_text (value.get() ? SET_CHECK : UNSET_CHECK, check_bounds, text_color, text_attributes);

		auto text_bounds = maxim::rect {
			check_bounds.left() + CHECK_WIDTH + 1,
			bounds.top(),
			bounds.width() - CHECK_WIDTH - 1,
			bounds.height() };

		text_bounds = bounds.intersection (text_bounds);
		g.draw_text(text.get(), bounds, text_color, text_attributes);
	}

	void checkbox::on_key_press (maxim::input::key_info const & key) {
		if (key.key == maxim::input::virtual_key::enter || key.key == maxim::input::virtual_key::space) {
			value = !value.get();
		}

		control::on_key_press (key);
	}

}