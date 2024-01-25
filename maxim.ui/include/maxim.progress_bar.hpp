#pragma once
#ifndef _maxim_ui_progress_bar_hpp_
#define _maxim_ui_progress_bar_hpp_

#include "maxim.control.hpp"

namespace maxim {

	class progress_bar : public basic_control {
	public:
		using builder = specialized_builder < progress_bar >;

		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "progress-bar";
		std::string_view        default_style_class_name() const override;

		void on_initialize_controls() override;
		void on_paint(maxim::graphics &g, maxim::size size) override;

		property < int >            min = 0;
		property < int >            max = 0;
		property < int >            value = 0;

		property < std::string >    text;

		property < maxim::color >			text_color;
		property < maxim::text_attributes >	text_attributes;
	};

	template < typename derived_type >
	class builder < progress_bar, derived_type > : public builder < basic_control, derived_type > {
	public:
		using builder < basic_control, derived_type >::builder;

		derived_type & min (int value) {
			this->get()->value;
			return this->derived();
		}

		derived_type & max (int value) {
			this->get()->max = value;
			return this->derived();
		}

		derived_type & value (int value) {
			this->get()->value = value;
			return this->derived();
		}

		derived_type & text (std::string_view value) {
			this->get()->text = value;
			return this->derived();
		}

	derived_type & text_color (maxim::color value) {
			this->get()->text_color = value;
			return this->derived();
		}

		derived_type & text_attributes (maxim::text_attributes value) {
			this->get()->text_attributes = value;
			return this->derived();
		}
	};

}

#endif
