#pragma once
#ifndef MAXIM_LISTBOX_HPP
#define MAXIM_LISTBOX_HPP

#include "maxim.control.hpp"

namespace maxim {

	struct listbox_item {
	public:
		std::string text;
		std::any    tag;
	};

	class listbox :
		public control {
	public:
		static constexpr int NO_SELECTION = std::numeric_limits < int >::max();

		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "listbox";
		std::string_view        default_style_class_name() const override;

		std::string_view        style_state () const override;

		void on_paint(maxim::graphics &g, maxim::rect bounds) override;

		// functionality related properties
		property<int>                       selected_index = NO_SELECTION;

		// capture keyboard events
		void on_key_press (maxim::input::key_info const & key) override;

		void on_focus_lost () override;
		void on_focus () override;

		// TODO: perhaps hide this in the implementation?
		property < std::vector < listbox_item > > items;

	protected:
		void update_view (int height);
	private:
		int _last_height = 0;
		int _hovered_offset = 0;
		int _scroll_offset = 0;
	};
}

#endif