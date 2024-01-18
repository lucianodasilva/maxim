#include "maxim.panel.hpp"

namespace maxim {

	maxim::rect panel_item::layout (panel * parent, panel_item * item, maxim::rect bounds) {

		auto item_near_margin = item->location.get ();
		auto item_far_margin = parent->default_size.get().as_point () - (item->location.get () + item->size.get().as_point ());

		auto item_bounds = rect { item_near_margin, item->size.get () };


		if (item->docking.get () == docking_kind::none) {
			// calculate docking
			switch (item->docking.get()) {
				case docking_kind::top:

					if (enum_has(item->anchor.get(), anchor_kind::bottom)) {
						item_bounds.size.height = (bounds.height() - item_far_margin.y);
					}

					item_bounds.position.x = 0;
					item_bounds.position.y = 0;
					item_bounds.size.width = bounds.width();
					break;
				case docking_kind::bottom:

					if (enum_has(item->anchor.get(), anchor_kind::top)) {
						item_bounds.size.height = (bounds.height() - item_bounds.position.y);
					} else {
						item_bounds.position.y = (bounds.height() - item->size.get().height);
					}

					item_bounds.position.x = 0;
					item_bounds.size.width = bounds.width();
					break;
				case docking_kind::left:

					if (enum_has(item->anchor.get(), anchor_kind::right)) {
						item_bounds.size.width = (bounds.width() - item_far_margin.x);
					}

					item_bounds.position.x = 0;
					item_bounds.position.y = 0;
					item_bounds.size.height = bounds.height();
					break;
				case docking_kind::right:

					if (enum_has(item->anchor.get(), anchor_kind::left)) {
						item_bounds.size.width = (bounds.width() - item_bounds.position.x);
					} else {
						item_bounds.position.x = (bounds.width() - item->size.get().width);
					}

					item_bounds.position.y = 0;

					item_bounds.size.height = bounds.height();
					break;
				case docking_kind::fill:
					item_bounds.position = {0, 0};
					item_bounds.size = bounds.size;
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
					item_bounds.size.height = (bounds.height () - item_far_margin.y - item_near_margin.y);
				}
				else {
					item_bounds.position.y = (bounds.height () - item_far_margin.y - item->size.get ().height);
				}
			}

			if (enum_has (item->anchor.get(), anchor_kind::right)) {
				if (enum_has (item->anchor.get(), anchor_kind::left)) {
					item_bounds.size.width = (bounds.width () - item_far_margin.x - item_near_margin.x);
				}
				else {
					item_bounds.position.x = (bounds.width () - item_far_margin.x - item->size.get ().width);
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