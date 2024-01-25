#pragma once
#ifndef _maxim_ui_checkbox_hpp_
#define _maxim_ui_checkbox_hpp_

#include "maxim.label.hpp"

namespace maxim {

	class checkbox : public label {
	public:

		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "checkbox";
		std::string_view        default_style_class_name() const override;

		checkbox();

		void on_paint(maxim::graphics & g, maxim::size size) override;

		property < bool > value;

		void on_key_press (maxim::input::key_info const & key) override;
	};

	template < typename derived_type >
	class builder < checkbox, derived_type > : public builder < label, derived_type > {
	public:
		using builder < label, derived_type >::builder;

		inline auto & value (bool value) {
			this->get ()->value = value;
			return this->derived();
		}
	};
}

#endif
