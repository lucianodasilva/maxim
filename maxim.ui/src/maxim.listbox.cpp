#include "maxim.listbox.hpp"
#include "maxim.application.hpp"
#include "maxim.layout.hpp"

namespace maxim {

	listbox::listbox() {
		selected_index.changed.append_no_scope([this](auto & sender, int index) {
			if (index == NO_SELECTION) {
				on_selection_changed(std::nullopt);
			} else {
				on_selection_changed(items.get()[index]);
			}
		});

		this->tab_stop = true;
	}

	std::string_view listbox::default_style_class_name() const {
		return listbox::DEFAULT_STYLE_CLASS_NAME;
	}

	std::string_view listbox::style_state() const {
		return states::DEFAULT;
	}

	void listbox::on_focus() {
		// ( doesnt work backwards )
		// if we have no data, focus next control
		// if (items.get().empty()) {
		// 	if (application()) {
		// 		application()->focus_next();
		// 		return;
		// 	}
		// }

		basic_control::on_focus();
	}

	void listbox::on_paint(maxim::graphics &g, maxim::rect bounds) {
		basic_control::on_paint(g, bounds);

		bool    	    border;
		bool            scrollbar;
		maxim::color    scrollbar_color;
		maxim::color    scrollbar_background;
		maxim::margins  margins;

		this->style_reader()
			.get(properties::SCROLLBAR, scrollbar)
			.get(properties::SCROLLBAR_COLOR, scrollbar_color)
			.get(properties::SCROLLBAR_BACKGROUND, scrollbar_background)
			.get(properties::BORDER, border)
			.get(properties::MARGINS, margins)
			;

		// calculate "final" bounds ==============================
		// if we have border, "contract" bounds
		if (border) {
			bounds = { bounds.position() + point {1, 1}, {bounds.width - 2, bounds.height - 2} };
		}

		// then add margins
		bounds = layout::apply_margins(bounds, margins);

		// finally remove scrollbar
		if (scrollbar) {
			bounds = { bounds.position(), {bounds.width - 1, bounds.height} };
		}

		// recalculate view ans cache height
		this->update_view(bounds.height);
		// ========================================================

		// render scrollbar ( one off out of bounds... )
		if (scrollbar) {
			// draw scrollbar
			g.draw_v_line (
				{ bounds.right() + 1, bounds.top() },
				bounds.height,
				scrollbar_background);

			// draw scrollbar cursor
			if (items.get().size () > bounds.height) {
				// proportion of items visible
				auto proportion = float (bounds.height ) / items.get ().size ();

				auto scroll_length = (bounds.height * proportion);
				auto scroll_top = (_scroll_offset + 1) * proportion;

				g.draw_v_line (
					{ bounds.right () + 1, bounds.top () + static_cast < int > (scroll_top) },
					scroll_length,
					scrollbar_color);
			}
		}

		// draw items
		{
			auto item_max_count = std::min (bounds.height, static_cast < int > (items.get ().size ()));

			for (int i = 0; i < item_max_count; ++i) {

				// calculate item bounds
				auto item_bounds = rect { bounds.left (), bounds.top () + i, bounds.width, 1 };

				// find item index
				auto item_index = i + _scroll_offset;

				if (item_index < 0 || item_index >= items.get ().size ()) {
					continue;
				}

				auto item = items.get ()[item_index];

				maxim::color back_color;
				maxim::color text_color;
				maxim::text_attributes text_attributes;

				std::string_view item_state = states::DEFAULT;

				bool is_selected = selected_index.get () == item_index;
				bool is_hovered = _hovered_offset == item_index && this->has_focus ();

				if (is_selected && is_hovered) {
					item_state = states::SELECTED_FOCUSED;
				} else if (is_selected) {
					item_state = states::SELECTED;
				} else if (is_hovered) {
					item_state = states::FOCUSED;
				}

				auto sreader = style_property_reader (this->style (), style_property_key { this->style_class_name(), item_state });

				sreader
					.get (properties::BACKGROUND, back_color)
					.get (properties::TEXT_COLOR, text_color)
					.get (properties::TEXT_ATTRIBUTES, text_attributes);

				// draw item background
				g.draw_box (item_bounds, back_color);

				// draw text
				g.draw_text (item.text, item_bounds, text_color, text_attributes);
			}
		}
	}

	void listbox::on_key_press(maxim::input::key_info const &key) {
		basic_control::on_key_press(key);

		// if there are no items to see, there is nothing to see :P
		if (items.get().empty ()) {
			return;
		}

		if (key.key == maxim::input::virtual_key::up) {
			if (_hovered_offset == NO_SELECTION || _hovered_offset == 0) {
				// if we have no selection or are at the beggining of the list, select the last item
				_hovered_offset = static_cast < int > (items.get().size() - 1);
			} else if (_hovered_offset > 0) {
				--_hovered_offset;
			}
		} else if (key.key == maxim::input::virtual_key::down) {
			// if we have no selection or are at the end of the list, select the first item
			if (_hovered_offset == NO_SELECTION || _hovered_offset == static_cast < int > (items.get().size() - 1)) {
				_hovered_offset = 0;
			} else if (_hovered_offset < static_cast < int > (items.get().size() - 1)) {
				++_hovered_offset;
			}
		} else if (key.key == maxim::input::virtual_key::enter || key.key == maxim::input::virtual_key::space) {
			selected_index = _hovered_offset;
		}
	}

	void listbox::on_selection_changed (std::optional < listbox_item > item) {
		selection_changed.invoke(*this, item);
	}

	void listbox::update_view (int height) {
		// track hovered item up ...
		if (_hovered_offset < _scroll_offset) {
			_scroll_offset = _hovered_offset;
		}

		// ... and down
		if (_hovered_offset >= _scroll_offset + height) {
			_scroll_offset = _hovered_offset - height + 1;
		}

		// track view size and scroll offset
		if (height > items.get().size()) {
			_scroll_offset = 0;
		} else if (items.get().size() - _scroll_offset < height) {
			_scroll_offset = items.get().size() - height;
		}

		_last_height = height;
	}

}