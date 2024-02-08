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

	void numupdown::on_paint (maxim::graphics & g, maxim::size size) {
		auto bounds = maxim::rect { { 0, 0 }, size };

		// clear extended background
		g.draw_rect(bounds, background);

		auto text_bounds = maxim::rect {
			bounds.position() + maxim::point{1, 0},
			{
				bounds.width - 1,
				bounds.height
			}
		};

		maxim::label::on_paint (g, size);

		// draw +/- label
		auto const LEFT_LABEL_BOUNDS = maxim::rect {
			bounds.position(),
			{ 1, bounds.height }
		};

		g.draw_char (LEFT_LABEL_BOUNDS.position(), text_color, 0x67, text_attributes.get().flags, true);
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