#pragma once
#ifndef _maxim_ui_label_hpp_
#define _maxim_ui_label_hpp_

#include "maxim.control.hpp"
#include "maxim.layout.hpp"

namespace maxim {

	class label : public basic_control {
	public:
		using builder = specialized_builder < label >;

		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "label";
		std::string_view        default_style_class_name () const override;

		property < maxim::color > 	        text_color;
		property < maxim::text_attributes > text_attributes;
		property < maxim::margins >         margins;

		property < std::string >            text;

		void on_initialize_controls () override;
		void on_paint (maxim::graphics & g, maxim::size size) override;
	};

	template < typename derived_type >
	class builder < label, derived_type > : public builder < control, derived_type >{
	public:
		using builder < control, derived_type >::builder;

		derived_type & text_color (maxim::color value) {
			this->get()->text_color = value;
			return this->derived();
		}

		derived_type & text_attributes (maxim::text_attributes value) {
			this->get()->text_attributes = value;
			return this->derived();
		}

		derived_type & margins (maxim::margins value) {
			this->get()->margins = value;
			return this->derived();
		}

		derived_type & text (std::string_view value) {
			this->get()->text = std::string(value);
			return this->derived();
		}
	};

}

#endif
