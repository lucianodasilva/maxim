#pragma once
#ifndef _maxim_ui_listbox_hpp_
#define _maxim_ui_listbox_hpp_

#include "maxim.control.hpp"
#include "maxim.layout.hpp"

namespace maxim {

	struct listbox_item {
	public:
		std::string text;
		std::any    tag;
	};

	class listbox_item_renderer {
	public:
		listbox_item_renderer(class listbox& host);

		virtual void render(listbox_item const& item, maxim::graphics & g, bool hovered, bool selected, maxim::size size);
		virtual int get_height(listbox_item const& item, int width);

		property < bool > wrap_text = false;

		class listbox & host () const { return _host; }

	private:
		class listbox & _host;
	};

	using listbox_selection_changed_event = event < class listbox & /* sender */, std::optional < listbox_item > /* item */ >;

	class listbox :
		public basic_control {
	public:
		using builder = specialized_builder < listbox >;

		listbox ();
		static constexpr int NO_SELECTION = std::numeric_limits < int >::max();

		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "listbox";
		std::string_view        default_style_class_name() const override;

		std::string_view        style_state () const override;

		void on_focus () override;

		void on_initialize_controls () override;
		void on_paint(maxim::graphics &g, maxim::size size) override;

		// functionality related properties
		property<int>  selected_index = NO_SELECTION;

		// capture keyboard events
		void on_key_press (maxim::input::key_info const & key) override;

		listbox_selection_changed_event selection_changed;
		virtual void on_selection_changed (std::optional < listbox_item > item);

		// TODO: perhaps hide this in the implementation?
		property < std::vector < listbox_item > > items;

		property < bool >           scrollbar;
		property < maxim::color >   scrollbar_color;
		property < maxim::color >   scrollbar_background;
		property < maxim::margins > margins;

		std::unique_ptr < listbox_item_renderer > item_renderer = std::make_unique < listbox_item_renderer > (*this);

		void track_item (int index);

	protected:
		void update_bounds (maxim::rect bounds);

		[[nodiscard]] maxim::rect local_item_bounds (int index);
		[[nodiscard]] maxim::rect local_total_item_bounds();

		void accept_local_item_bounds(std::function < bool (int /* index */, maxim::rect /* bounds */) > const& callback);

	private:
		maxim::rect	_cached_bounds = {};
		int			_hovered_item = 0;
		int			_scroll_offset = 0;
	};

	template < typename derived_type >
	class builder < listbox, derived_type > : public builder < control, derived_type > {
	public:
		using builder < control, derived_type >::builder;

		derived_type & scrollbar (bool value = true) {
			this->get ()->scrollbar = value;
			return this->derived ();
		}

		derived_type & scrollbar_color (maxim::color value) {
			this->get ()->scrollbar_color = value;
			return this->derived ();
		}

		derived_type & scrollbar_background (maxim::color value) {
			this->get ()->scrollbar_background = value;
			return this->derived ();
		}

		derived_type & margins (maxim::margins value) {
			this->get ()->margins = value;
			return this->derived ();
		}
	};
}

#endif