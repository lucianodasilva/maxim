#include "maxim.progress_bar.hpp"

namespace maxim {

	std::string_view progress_bar::default_style_class_name() const {
		return progress_bar::DEFAULT_STYLE_CLASS_NAME;
	}

	void progress_bar::on_initialize_controls() {
		basic_control::on_initialize_controls();

		this->bind_style_property(this->text_color, properties::TEXT_COLOR);
		this->bind_style_property(this->text_attributes, properties::TEXT_ATTRIBUTES);
	}

	void progress_bar::on_paint(maxim::graphics &g, maxim::size size) {
		auto bounds = rect{ {}, size};

		auto value_width = static_cast<int>(std::rint(bounds.width * (value / static_cast<float>(max - min))));

		maxim::color            highlight_color;
		maxim::color            highlight_text_color;
		maxim::text_attributes  highlight_text_attributes;

		this->style ()
			.get < maxim::color > ({ this->style_class_name, states::SELECTED, properties::BACKGROUND }, highlight_color)
			.get < maxim::color > ({ this->style_class_name, states::SELECTED, properties::TEXT_COLOR }, highlight_text_color)
			.get < maxim::text_attributes > ({ this->style_class_name, states::SELECTED, properties::TEXT_ATTRIBUTES }, highlight_text_attributes);

		g.draw_box(bounds, this->background, true);

		if (!text.get().empty()) {
			g.draw_text(text, bounds, highlight_text_color, highlight_text_attributes);
		}

		// change clip view
		{
			graphics_transform_guard tnc_guard(g, { bounds.position() + point {value_width, 0}, { bounds.width - value_width, bounds.height }});
			g.draw_text(text, {{}, {value_width, bounds.height}}, this->text_color, this->text_attributes);
		}
		
	}
}