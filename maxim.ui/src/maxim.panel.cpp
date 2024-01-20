#include "maxim.panel.hpp"

namespace maxim {

	maxim::rect panel_item::layout (panel * parent, panel_item * item, maxim::rect bounds) {

		auto item_near_margin = item->location.get ();
		auto item_far_margin = parent->default_size.get() - (item->location.get () + item->size.get());

		auto item_bounds = rect { item_near_margin, item->size.get () };


		if (item->docking.get () == docking_kind::none) {
			// calculate docking
			switch (item->docking.get()) {
				case docking_kind::top:

					if (enum_has(item->anchor.get(), anchor_kind::bottom)) {
						item_bounds.height = (bounds.height - item_far_margin.y);
					}

					item_bounds.x = 0;
					item_bounds.y = 0;
					item_bounds.width = bounds.width;
					break;
				case docking_kind::bottom:

					if (enum_has(item->anchor.get(), anchor_kind::top)) {
						item_bounds.height = (bounds.height - item_bounds.y);
					} else {
						item_bounds.y = (bounds.height - item->size.get().y);
					}

					item_bounds.x = 0;
					item_bounds.width = bounds.width;
					break;
				case docking_kind::left:

					if (enum_has(item->anchor.get(), anchor_kind::right)) {
						item_bounds.width = (bounds.width - item_far_margin.x);
					}

					item_bounds.x = 0;
					item_bounds.y = 0;
					item_bounds.height = bounds.height;
					break;
				case docking_kind::right:

					if (enum_has(item->anchor.get(), anchor_kind::left)) {
						item_bounds.width = (bounds.width - item_bounds.x);
					} else {
						item_bounds.x = (bounds.width - item->size.get().x);
					}

					item_bounds.y = 0;

					item_bounds.height = bounds.height;
					break;
				case docking_kind::fill:
					item_bounds = { 0, 0, bounds.width, bounds.height};
					break;
				case docking_kind::none:
					[[fallthrough]];
				default:
					break;
			}
		} else {
			// calculate anchoring
			if (enum_has (item->anchor.get(), anchor_kind::bottom)) {
				if (enum_has (item->anchor.get(), anchor_kind::top)) {
					item_bounds.height = (bounds.height - item_far_margin.y - item_near_margin.y);
				}
				else {
					item_bounds.y = (bounds.height  - item_far_margin.y - item->size.get ().y);
				}
			}

			if (enum_has (item->anchor.get(), anchor_kind::right)) {
				if (enum_has (item->anchor.get(), anchor_kind::left)) {
					item_bounds.width = (bounds.width - item_far_margin.x - item_near_margin.x);
				}
				else {
					item_bounds.x = (bounds.width - item_far_margin.x - item->size.get ().x);
				}
			}
		}

		return item_bounds;
	}

	std::string_view panel::default_style_class_name () const {
		return panel::DEFAULT_STYLE_CLASS_NAME;
	}

	void panel::on_paint (maxim::graphics & g, maxim::rect bounds) {
		// paint itself
		basic_control_container::on_paint (g, bounds);

		// paint children
		for (auto & object : controls ()) {
			object->on_paint (g, panel_item::layout (this, object.get(), bounds));
		}
	}

}