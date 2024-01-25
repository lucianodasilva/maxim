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

	void stack::on_initialize_controls() {
		basic_control_container::on_initialize_controls();

		this->bind_style_property(margins, properties::MARGINS);
		this->bind_style_property(padding, properties::PADDING);
	}

	void stack::on_paint (maxim::graphics & g, maxim::size size) {
		// paint itself
		basic_control_container::on_paint (g, size);

		// apply margins
		auto bounds = layout::apply_margins ({{}, size }, margins);

		// update partitions
		layout::partition::refresh (
			this->orientation.get () == maxim::orientation::horizontal ? bounds.width : bounds.height,
			padding.get(),
			this->controls(),
			[](auto & stack_item_inst) -> maxim::layout::partition & {
				return stack_item_inst->length.get();
			});

		// paint children
		for (auto & object : this->controls()) {
			auto item_bounds = stack_item::layout(this, object.get(), bounds);
			graphics_transform_guard tnc_guard (g, item_bounds);

			object->on_paint (g, item_bounds.size());
		}
	}

}