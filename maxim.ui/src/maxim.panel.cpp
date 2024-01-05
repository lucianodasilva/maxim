#include "maxim.panel.hpp"

namespace maxim {

	maxim::rect basic_panel_item::layout (maxim::panel * parent, maxim::rect bounds) const {

		auto item_near_margin = location.get ();
		auto item_far_margin = parent->default_size.get().as_point () - (location.get () + size.get().as_point ());

		auto item_bounds = rect { item_near_margin, size.get () };


		if (docking.get () == docking_kind::none) {
			// calculate docking
			switch (docking.get()) {
				case docking_kind::top:

					if (enum_has(anchor.get(), anchor_kind::bottom)) {
						item_bounds.size.height = (bounds.height() - item_far_margin.y);
					}

					item_bounds.position.x = 0;
					item_bounds.position.y = 0;
					item_bounds.size.width = bounds.width();
					break;
				case docking_kind::bottom:

					if (enum_has(anchor.get(), anchor_kind::top)) {
						item_bounds.size.height = (bounds.height() - item_bounds.position.y);
					} else {
						item_bounds.position.y = (bounds.height() - size.get().height);
					}

					item_bounds.position.x = 0;
					item_bounds.size.width = bounds.width();
					break;
				case docking_kind::left:

					if (enum_has(anchor.get(), anchor_kind::right)) {
						item_bounds.size.width = (bounds.width() - item_far_margin.x);
					}

					item_bounds.position.x = 0;
					item_bounds.position.y = 0;
					item_bounds.size.height = bounds.height();
					break;
				case docking_kind::right:

					if (enum_has(anchor.get(), anchor_kind::left)) {
						item_bounds.size.width = (bounds.width() - item_bounds.position.x);
					} else {
						item_bounds.position.x = (bounds.width() - size.get().width);
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
			if (enum_has (anchor.get(), anchor_kind::bottom)) {
				if (enum_has (anchor.get(), anchor_kind::top)) {
					item_bounds.size.height = (bounds.height () - item_far_margin.y - item_near_margin.y);
				}
				else {
					item_bounds.position.y = (bounds.height () - item_far_margin.y - size.get ().height);
				}
			}

			if (enum_has (anchor.get(), anchor_kind::right)) {
				if (enum_has (anchor.get(), anchor_kind::left)) {
					item_bounds.size.width = (bounds.width () - item_far_margin.x - item_near_margin.x);
				}
				else {
					item_bounds.position.x = (bounds.width () - item_far_margin.x - size.get ().width);
				}
			}
		}

		return item_bounds;
	}

	std::string_view panel::default_style_class_name () const {
		return panel::DEFAULT_STYLE_CLASS_NAME;
	}

}