#include "maxim.progress_bar.hpp"

namespace maxim {

	std::string_view progress_bar::default_style_class_name() const {
		return progress_bar::DEFAULT_STYLE_CLASS_NAME;
	}

	void progress_bar::on_paint(maxim::graphics &g, maxim::rect bounds) {
		auto value_width = static_cast<int>(std::rint(bounds.width * (value / static_cast<float>(max - min))));

		maxim::color            back_color;
		maxim::color            text_color;
		maxim::text_attributes  text_attributes;

		maxim::color            highlight_color;
		maxim::color            highlight_text_color;
		maxim::text_attributes  highlight_text_attributes;

		this->style ()
			.get < maxim::color > ({ this->style_class_name, states::SELECTED, properties::BACKGROUND }, highlight_color)
			.get < maxim::color > ({ this->style_class_name, states::SELECTED, properties::TEXT_COLOR }, highlight_text_color)
			.get < maxim::text_attributes > ({ this->style_class_name, states::SELECTED, properties::TEXT_ATTRIBUTES }, highlight_text_attributes);

		g.draw_box(bounds, back_color, true);

		if (!text.get().empty()) {
			g.draw_text(text, bounds, highlight_text_color, highlight_text_attributes);
		}

		// change clip view
		g.clip_window_push(
			{bounds.position() + maxim::point{value_width, 0}, {bounds.width - value_width, bounds.height}});
		g.draw_text(text, bounds, text_color, text_attributes);
		g.clip_window_pop();
	}
}