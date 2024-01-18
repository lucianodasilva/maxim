#pragma once
#ifndef MAXIM_STACK_HPP
#define MAXIM_STACK_HPP

#include "maxim.control_container.hpp"
#include "maxim.layout.hpp"

namespace maxim {

	enum struct orientation {
		horizontal,
		vertical
	};

	class stack_item : public virtual control {
	public:
		using builder = specialized_builder < stack_item >;

		property < layout::partition > length;

		static maxim::rect layout (class stack * parent, stack_item * item, maxim::rect bounds);
	};

	class stack : public composite_control_container < stack_item > {
	public:
		using builder = specialized_builder < stack >;

		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "stack";
		std::string_view        default_style_class_name () const override;

		property < maxim::orientation > orientation { maxim::orientation::vertical };

		void on_paint (maxim::graphics & g, maxim::rect bounds) override;
	};

	template < typename derived_type >
	class builder < stack, derived_type > : public builder < control, derived_type > {
	public:
		using builder < control, derived_type >::builder;

		inline derived_type & orientation (maxim::orientation value) {
			this->get()->_orientation = value;
			return this->derived();
		}

		inline derived_type & horizontal () {
			this->get()->_orientation = maxim::orientation::horizontal;
			return this->derived();
		}

		inline derived_type & vertical () {
			this->get()->_orientation = maxim::orientation::vertical;
			return this->derived();
		}
	};

	template < typename derived_type >
	class builder < stack_item, derived_type > {
	public:
		inline derived_type & length (layout::partition value) {
			auto & inst = *static_cast < derived_type * > (this);
			inst.get()->length = value;

			return inst;
		}
	};

}

#endif
