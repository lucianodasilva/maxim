#include "maxim.label.hpp"
#include "maxim.layout.hpp"

namespace maxim {

	std::string_view label::default_style_class_name () const {
		return label::DEFAULT_STYLE_CLASS_NAME;
	}

	void label::on_paint (maxim::graphics & g, maxim::rect bounds) {
		control::on_paint (g, bounds);

		maxim::color            text_color;
		maxim::text_attributes  text_attributes;
		maxim::margins		    margins;

		this->style_reader ()
			.get (properties::TEXT_COLOR, text_color)
			.get (properties::TEXT_ATTRIBUTES, text_attributes)
			.get (properties::MARGINS, margins);

		bounds = layout::apply_margins(bounds, margins);
		g.draw_text (text.get (), bounds, text_color, text_attributes);
	}



}