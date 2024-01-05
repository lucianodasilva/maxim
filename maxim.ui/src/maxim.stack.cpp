#include "maxim.stack.hpp"
#include "maxim.layout.hpp"

namespace maxim {

	maxim::rect basic_stack_item::layout(class maxim::stack *parent, maxim::rect bounds) const {
		if (parent == nullptr) {
			return bounds;
		}

		return maxim::rect {
			bounds.position + maxim::point {
				parent->orientation.get() == maxim::orientation::horizontal ? length.get ().near : 0,
				parent->orientation.get() == maxim::orientation::vertical ? length.get ().near : 0
			},
			maxim::size {
				parent->orientation.get() == maxim::orientation::horizontal ? length.get ().far - length.get ().near : bounds.size.width,
				parent->orientation.get() == maxim::orientation::vertical ? length.get ().far - length.get ().near : bounds.size.height
			}
		};
	}

	std::string_view stack::default_style_class_name () const {
		return stack::DEFAULT_STYLE_CLASS_NAME;
	}

	void stack::on_paint (maxim::graphics & g, maxim::rect bounds) {
		// update partitions
		layout::partition::refresh (
			this->orientation.get () == maxim::orientation::horizontal ? bounds.size.width : bounds.size.height,
			_partition_objects,
			[](auto * stack_item_inst) -> maxim::layout::partition & {
				return stack_item_inst->length.get();
			});

		// paint itself and paint children
		control_container::on_paint (g, bounds);
	}

	void stack::on_control_added(maxim::basic_stack_item *item) {
		_partition_objects.push_back(item);
	}

}