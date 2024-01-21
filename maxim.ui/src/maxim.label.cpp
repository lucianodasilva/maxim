#include "maxim.label.hpp"

namespace maxim {

	std::string_view label::default_style_class_name () const {
		return label::DEFAULT_STYLE_CLASS_NAME;
	}

	void label::on_initialize_controls () {
		basic_control::on_initialize_controls ();

		this->bind_style_property(text_color, properties::TEXT_COLOR);
		this->bind_style_property(text_attributes, properties::TEXT_ATTRIBUTES);
		this->bind_style_property(margins, properties::MARGINS);
	}

	void label::on_paint (maxim::graphics & g, maxim::rect bounds) {
		basic_control::on_paint (g, bounds);

		bounds = layout::apply_margins(bounds, margins);
		g.draw_text (text.get (), bounds, text_color, text_attributes);
	}



}