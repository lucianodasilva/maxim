#pragma once
#ifndef _maxim_ui_label_hpp_
#define _maxim_ui_label_hpp_

#include "maxim.control.hpp"

namespace maxim {

	class label : public basic_control {
	public:
		using builder = specialized_builder < label >;

		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "label";
		std::string_view        default_style_class_name () const override;

		property < std::string > text;

		void on_paint (maxim::graphics & g, maxim::rect bounds) override;
	};

	template < typename derived_type >
	class builder < label, derived_type > : public builder < control, derived_type >{
	public:
		using builder < control, derived_type >::builder;

		inline derived_type & text (std::string_view value) {
			this->get()->text = std::string(value);
			return this->derived();
		}
	};

}

#endif
