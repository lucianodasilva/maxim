#pragma once
#ifndef MAXIM_STACK_HPP
#define MAXIM_STACK_HPP

#include "maxim.control.hpp"
#include "maxim.layout.hpp"

namespace maxim {

	enum struct orientation {
		horizontal,
		vertical
	};

	class basic_stack_item {
	public:
		property < layout::partition > length;

		maxim::rect layout (class stack * parent, maxim::rect bounds) const;
	};

	template < typename control_type,  typename std::enable_if_t < std::is_base_of_v < control, control_type >, int > = 0 >
	class stack_item : public basic_stack_item, public virtual control_type {
	public:
		using control_type::control_type;

		inline void on_paint (maxim::graphics & g, maxim::rect bounds) override {
			// HACK: dynamic_cast is a major code smell... but its what i got for now
			control_type::on_paint (g, this->layout (dynamic_cast < class stack * > (this->parent()) , bounds));
		}
	};

	class stack : public specialized_control_container < basic_stack_item, stack_item > {
	public:
		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "stack";
		std::string_view        default_style_class_name () const override;

		property < maxim::orientation > orientation { maxim::orientation::vertical };

		void on_paint (maxim::graphics & g, maxim::rect bounds) override;
	protected:
		void on_control_added(maxim::basic_stack_item *item) override;
		std::vector < basic_stack_item * >  _partition_objects;
	};

}

#endif
