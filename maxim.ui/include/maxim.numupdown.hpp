#pragma once
#ifndef _maxim_ui_numupdown_hpp_
#define _maxim_ui_numupdown_hpp_

#include "maxim.control.hpp"
#include "maxim.label.hpp"

namespace maxim {

	class numupdown : public virtual maxim::control, private maxim::label {
	public:
		using builder = specialized_builder < numupdown >;

		numupdown ();

		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "numupdown";
		std::string_view        default_style_class_name() const override;

		property < int >        value = 0;
		property < int >        min = 0;
		property < int >        max = 0;
		property < int >        step = 1;

		void                    on_paint (maxim::graphics & g, maxim::size size) override;
		void                    on_key_press (maxim::input::key_info const & e) override;

	};

	template < typename derived_type >
	class builder < numupdown, derived_type > : public builder < control, derived_type > {
	public:
		using builder < control, derived_type >::builder;

		derived_type & value (int value) {
			this->get()->value = value;
			return this->derived();
		}

		derived_type & min (int value) {
			this->get()->min = value;
			return this->derived();
		}

		derived_type & max (int value) {
			this->get()->max = value;
			return this->derived();
		}

		derived_type & step (int value) {
			this->get()->step = value;
			return this->derived();
		}
	};

}

#endif
