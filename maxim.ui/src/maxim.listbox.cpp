#include "maxim.listbox.hpp"
#include "maxim.application.hpp"
#include "maxim.layout.hpp"

namespace maxim {

	listbox_item_renderer::listbox_item_renderer(class listbox& host) : _host(host) {}

	void listbox_item_renderer::render(listbox_item const& item, maxim::graphics& g, bool hovered, bool selected, maxim::size size) {
		auto bounds = size.as_rect();

		std::string_view item_state = states::DEFAULT;

		if (selected && hovered) {
			item_state = states::SELECTED_FOCUSED;
		}
		else if (selected) {
			item_state = states::SELECTED;
		}
		else if (hovered) {
			item_state = states::FOCUSED;
		}

		maxim::color item_background;
		maxim::color item_text_color;
		maxim::text_attributes item_text_attributes;

		auto get_prop = [&](std::string_view prop_name, auto& prop) {
			_host.style().get(
				{ _host.style_class_name.get(), item_state, prop_name },
				prop);
			};

		get_prop(properties::BACKGROUND, item_background);
		get_prop(properties::TEXT_COLOR, item_text_color);
		get_prop(properties::TEXT_ATTRIBUTES, item_text_attributes);

		// draw item background
		g.draw_box(bounds, item_background);

		if (this->wrap_text) {
			item_text_attributes.wrap = true;
		}

		// draw text
		g.draw_text(item.text, bounds, item_text_color, item_text_attributes);
	}

	int listbox_item_renderer::get_height(listbox_item const& item, int width) {
		if (wrap_text.get()) {
			return maxim::text_height(item.text, width);
		} else {
			return 1;
		}
	}

	listbox::listbox() {
		selected_index.changed.append_no_scope([this](int index) {
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

	void listbox::on_initialize_controls() {
		basic_control::on_initialize_controls();

		this->bind_style_property(scrollbar, properties::SCROLLBAR);
		this->bind_style_property(scrollbar_color, properties::SCROLLBAR_COLOR);
		this->bind_style_property(scrollbar_background, properties::SCROLLBAR_BACKGROUND);
		this->bind_style_property(margins, properties::MARGINS);
	}

	void listbox::on_paint(maxim::graphics &g, maxim::size size) {
		basic_control::on_paint(g, size);

		auto bounds = rect { {0, 0}, size };

		// calculate "final" bounds ==============================
		// if we have border, "contract" bounds
		if (border) {
			bounds = bounds.offset ({1, 1}).expand({-2, -2});
		}

		// then add margins
		bounds = layout::apply_margins(bounds, margins);

		// finally remove scrollbar
		if (scrollbar) {
			bounds = { bounds.position(), {bounds.width - 1, bounds.height} };
		}

		// recalculate scroll offset and cache bounds
		this->update_bounds(bounds);
		// ========================================================

		int item_total_size = 0;

		// check for item renderer instance and render items
		if (this->item_renderer) {
			accept_local_item_bounds([this, &g, &item_total_size](int index, maxim::rect item_bounds) {
				// transform bounds
				item_bounds = item_bounds.offset({ 0, -_scroll_offset });

				auto& item = this->items.get()[index];

				// if item is visible, draw it
				if (g.clip_window().intersects(item_bounds.offset(g.transform()))) {
					bool is_selected = this->selected_index.get() == index;
					bool is_hovered = this->_hovered_item == index && this->has_focus();

					// items can be "cut" by the clip window, so we need to calculate the "real" clip window and transform offset
					auto item_clip_window = g.clip_window().intersection(item_bounds.offset(g.transform()));
					graphics_transform_guard tnc_guard(g, item_bounds.position(), item_clip_window);

					this->item_renderer->render(item, g, is_hovered, is_selected, item_bounds.size());
				}

				// store item bottom
				item_total_size += item_bounds.height;

				return true;
			});
		}

		// render scrollbar ( one off out of bounds... )
		if (scrollbar) {
			// draw scrollbar
			g.draw_v_line(
				{ bounds.right() + 1, bounds.top() },
				bounds.height,
				scrollbar_background);

			// draw scrollbar cursor
			if (item_total_size > bounds.height) {
				// proportion of items visible
				auto proportion = float(bounds.height) / item_total_size;

				auto scroll_length = (bounds.height * proportion);
				auto scroll_top = (_scroll_offset + 1) * proportion;

				g.draw_v_line(
					{ bounds.right() + 1, bounds.top() + static_cast <int> (scroll_top) },
					scroll_length,
					scrollbar_color);
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
			if (_hovered_item == NO_SELECTION || _hovered_item == 0) {
				// if we have no selection or are at the beggining of the list, select the last item
				_hovered_item = static_cast < int > (items.get().size() - 1);
			} else if (_hovered_item > 0) {
				--_hovered_item;
			}

			this->track_item(_hovered_item);
		} else if (key.key == maxim::input::virtual_key::down) {
			// if we have no selection or are at the end of the list, select the first item
			if (_hovered_item == NO_SELECTION || _hovered_item == static_cast < int > (items.get().size() - 1)) {
				_hovered_item = 0;
			} else if (_hovered_item < static_cast < int > (items.get().size() - 1)) {
				++_hovered_item;
			}

			this->track_item(_hovered_item);
		} else if (key.key == maxim::input::virtual_key::enter || key.key == maxim::input::virtual_key::space) {
			selected_index = _hovered_item;
		}
	}

	void listbox::on_selection_changed (std::optional < listbox_item > item) {
		selection_changed.invoke(*this, item);
	}

	void listbox::track_item(int index) {
		auto item_bounds = this->local_item_bounds(index);

		if (item_bounds.is_zero()) {
			// something is not right, we should not be here
			return;
		}

		// check if rendered position is out of bounds
		auto offset_item_bounds = item_bounds.offset({ 0, -_scroll_offset });

		// track up
		if (offset_item_bounds.top() < 0) {
			_scroll_offset = item_bounds.top();
		}

		// track down
		if (offset_item_bounds.bottom() >= _cached_bounds.height) {
			_scroll_offset = (item_bounds.bottom() + 1 ) - _cached_bounds.height;
		}
	}

	void listbox::update_bounds(maxim::rect bounds) {
		if (bounds == _cached_bounds) {
			return;
		}

		_cached_bounds = bounds;

		if (_hovered_item != NO_SELECTION) {
			this->track_item(_hovered_item);
		}

		auto total_item_bouds = this->local_total_item_bounds();

		// adjust scroll offset
		if (bounds.height > total_item_bouds.height) {
			_scroll_offset = 0;
		}
		else if (total_item_bouds.height - _scroll_offset < bounds.height) {
			_scroll_offset = total_item_bouds.height - bounds.height;
		}
	}

	maxim::rect listbox::local_item_bounds(int index) {
		// if we have no items, return empty rect
		if (index < 0 || index >= items.get().size()) {
			return {};
		}

		rect item_bounds;

		accept_local_item_bounds([&](int current_index, maxim::rect current_bounds) {
			item_bounds = current_bounds;
			return index != current_index; // if we are not at the index we want, continue
		});

		return item_bounds;
	}

	maxim::rect listbox::local_total_item_bounds() {
		rect total_bounds { 0, 0, _cached_bounds.width, 0 };

		for (int i = 0; i < items.get().size(); ++i) {
			auto & item = items.get()[i];
			total_bounds.height += this->item_renderer->get_height(item, _cached_bounds.width);
		}

		return total_bounds;
	}

	void listbox::accept_local_item_bounds(std::function < bool(int, maxim::rect) > const & callback) {
		// calculate top of first item
		int item_top = 0;

		// iterate through items
		for (int i = 0; i < items.get().size(); ++i) {
			auto& item = items.get()[i];

			// calculate item bounds from top and height
			auto item_bounds = rect{
				_cached_bounds.left(),
				item_top,
				_cached_bounds.width,
				this->item_renderer->get_height(item, _cached_bounds.width) };

			if (!callback(i, item_bounds)) {
				break;
			}

			// increment top
			item_top += item_bounds.height;
		}
	}

}