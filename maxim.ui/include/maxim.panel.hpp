#pragma once
#ifndef MAXIM_PANEL_HPP
#define MAXIM_PANEL_HPP

#include "maxim.control.hpp"

namespace maxim {

	enum struct anchor_kind {
		none        = 0,
		top	        = 1 << 0,
		bottom      = 1 << 1,
		left        = 1 << 2,
		right       = 1 << 3,
	};

	enum struct docking_kind {
		none,
		top,
		bottom,
		left,
		right,
		fill
	};

	class basic_panel_item {
	public:
		virtual ~basic_panel_item () = default;

		property < maxim::point >       location;
		property < maxim::size >        size;

		property < maxim::anchor_kind > anchor { maxim::anchor_kind::none };
		property < maxim::docking_kind > docking { maxim::docking_kind::none };

		maxim::rect layout (class panel * parent, maxim::rect bounds) const;
	};

	template < typename control_type >
	class panel_item : public basic_panel_item, public control_type {
	public:
		static_assert(std::is_base_of_v < control, control_type >, "control_type must be derived from maxim::control");

		using control_type::control_type;

		inline void on_paint (graphics & g, rect  bounds) override {
			// HACK: dynamic_cast is a major code smell... but its what i got for now
			control_type::on_paint(g, this->layout (dynamic_cast < panel * > (this->parent()), bounds));
		}
	};

	class panel : public specialized_control_container < basic_panel_item, panel_item > {
	public:
		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "panel";
		std::string_view        default_style_class_name () const override;

		property < size > default_size {{ 100, 100 }};
	};

}

#endif
