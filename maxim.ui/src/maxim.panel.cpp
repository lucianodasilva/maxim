#include "maxim.panel.hpp"
#include "maxim.layout.hpp"

namespace maxim {

	std::string_view panel::default_style_class_name () const {
		return panel::DEFAULT_STYLE_CLASS_NAME;
	}

	void panel::on_paint (maxim::graphics & g, maxim::size size) {
		// paint itself
		basic_control_container::on_paint (g, size);

		// apply margins
		auto bounds = layout::apply_margins({{}, size}, margins);

		// paint children
		for (auto & object : controls ()) {
			auto item_bounds = item_layout (object.get(), size);

			auto tnc_guard = graphics_transform_guard(g, item_bounds);
			object->on_paint (g, item_bounds.size());
		}
	}

	maxim::rect panel::item_layout(panel_item * item, maxim::size size) {
		auto item_near_margin	= item->position.get();
		auto item_far_margin	= this->default_size.get() - (item->position.get() + item->size.get());

		auto item_bounds = rect{ item_near_margin, item->size.get() };

		if (item->docking.get() != docking_kind::none) {
			// calculate docking
			switch (item->docking.get()) {
			case docking_kind::top:

				if (enum_has(item->anchor.get(), anchor_kind::bottom)) {
					item_bounds.height = (size.height - item_far_margin.y);
				}

				item_bounds.x = 0;
				item_bounds.y = 0;
				item_bounds.width = size.width;
				break;
			case docking_kind::bottom:

				if (enum_has(item->anchor.get(), anchor_kind::top)) {
					item_bounds.height = (size.height - item_bounds.y);
				}
				else {
					item_bounds.y = (size.height - item->size.get().y);
				}

				item_bounds.x = 0;
				item_bounds.width = size.width;
				break;
			case docking_kind::left:

				if (enum_has(item->anchor.get(), anchor_kind::right)) {
					item_bounds.width = (size.width - item_far_margin.x);
				}

				item_bounds.x = 0;
				item_bounds.y = 0;
				item_bounds.height = size.height;
				break;
			case docking_kind::right:

				if (enum_has(item->anchor.get(), anchor_kind::left)) {
					item_bounds.width = (size.width - item_bounds.x);
				}
				else {
					item_bounds.x = (size.width - item->size.get().x);
				}

				item_bounds.y = 0;

				item_bounds.height = size.height;
				break;
			case docking_kind::fill:
				item_bounds = { 0, 0, size.width, size.height };
				break;
			case docking_kind::none:
				[[fallthrough]];
			default:
				break;
			}
		}
		else {
			// calculate anchoring
			if (enum_has(item->anchor.get(), anchor_kind::bottom)) {
				if (enum_has(item->anchor.get(), anchor_kind::top)) {
					item_bounds.height = (size.height - item_far_margin.y - item_near_margin.y);
				}
				else {
					item_bounds.y = (size.height - item_far_margin.y - item->size.get().y);
				}
			}

			if (enum_has(item->anchor.get(), anchor_kind::right)) {
				if (enum_has(item->anchor.get(), anchor_kind::left)) {
					item_bounds.width = (size.width - item_far_margin.x - item_near_margin.x);
				}
				else {
					item_bounds.x = (size.width - item_far_margin.x - item->size.get().x);
				}
			}
		}

		return item_bounds;
	}

}