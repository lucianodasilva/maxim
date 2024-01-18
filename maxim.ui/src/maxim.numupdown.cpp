#include "maxim.numupdown.hpp"

namespace maxim {

	numupdown::numupdown () {
		this->text.bind ([this] () {
			return std::to_string (value.get());
		});

		this->tab_stop = true;
	}

	std::string_view numupdown::default_style_class_name() const {
		return DEFAULT_STYLE_CLASS_NAME;
	}

	void numupdown::on_paint (maxim::graphics & g, maxim::rect bounds) {

		maxim::color        back_color;
		maxim::color        text_color;
		maxim::attributes   text_attributes;

		this->style_reader ()
			.get (properties::BACKGROUND, back_color)
			.get (properties::TEXT_COLOR, text_color)
			.get (properties::TEXT_ATTRIBUTES, text_attributes);


		// clear extended background
		g.draw_box(bounds, back_color);

		auto text_bounds = maxim::rect {
			bounds.position + maxim::point{ 1, 0 },
			{
				bounds.size.width - 1,
				bounds.size.height
			}
		};

		maxim::label::on_paint (g, text_bounds);

		// draw +/- label
		auto const LEFT_LABEL_BOUNDS = maxim::rect {
			bounds.position,
			{ 1, bounds.size.height }
		};

		g.draw_char (LEFT_LABEL_BOUNDS.position, text_color, 0x67,  text_attributes.flags, true);
	}

	void numupdown::on_key_press(const maxim::input::key_info &e) {
		std::optional < int > nval;

		if (e.key == maxim::input::virtual_key::up) {
			nval = this->value.get () + this->step.get();
		} else if (e.key == maxim::input::virtual_key::down) {
			nval = this->value.get () - this->step.get();
		} else {
			maxim::label::on_key_press(e);
		}

		if (nval.has_value()) {
			this->value.set (std::clamp (nval.value (), this->min.get (), this->max.get ()));
		}
	}

}