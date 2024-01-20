#include "maxim.stack.hpp"
#include "maxim.layout.hpp"

namespace maxim {

	maxim::rect stack_item::layout(maxim::stack *parent, stack_item * item, maxim::rect bounds) {
		if (parent == nullptr || item == nullptr) {
			return {};
		}

		return maxim::rect {
			bounds.position () + maxim::point {
				parent->orientation.get() == maxim::orientation::horizontal ? item->length.get ().near : 0,
				parent->orientation.get() == maxim::orientation::vertical ? item->length.get ().near : 0
			},
			{
				parent->orientation.get() == maxim::orientation::horizontal ? item->length.get ().far - item->length.get ().near : bounds.width,
				parent->orientation.get() == maxim::orientation::vertical ? item->length.get ().far - item->length.get ().near : bounds.height
			}
		};
	}

	std::string_view stack::default_style_class_name () const {
		return stack::DEFAULT_STYLE_CLASS_NAME;
	}

	void stack::on_paint (maxim::graphics & g, maxim::rect bounds) {

		int padding;

		this->style_reader()
			.get (properties::PADDING, padding);

		// update partitions
		layout::partition::refresh (
			this->orientation.get () == maxim::orientation::horizontal ? bounds.width : bounds.height,
			padding,
			this->controls(),
			[](auto & stack_item_inst) -> maxim::layout::partition & {
				return stack_item_inst->length.get();
			});

		// paint itself
		basic_control_container::on_paint (g, bounds);

		// paint children
		for (auto & object : this->controls()) {
			object->on_paint (g, stack_item::layout (this, object.get (), bounds));
		}
	}

}