#include "maxim.grid.hpp"
#include "maxim.layout.hpp"

namespace maxim {

	maxim::rect grid_item::layout(grid * parent, grid_item * item, maxim::rect bounds) {
		if (!parent || !item) {
			return {};
		}

		int col_from = std::clamp<int>(
			item->columns.get().from,
			0,
			parent->columns.size() - 1);

		int col_to = std::clamp<int>(
			item->columns.get().to,
			0,
			parent->columns.size() - 1);

		int row_from = std::clamp<int>(
			item->rows.get().from,
			0,
			parent->rows.size() - 1);

		int row_to = std::clamp<int>(
			item->rows.get().to,
			0,
			parent->rows.size() - 1);

		return maxim::rect{
			bounds.x + parent->columns[col_from].near,
			bounds.y + parent->rows[row_from].near,
			parent->columns[col_to].far - parent->columns[col_from].near,
			parent->rows[row_to].far - parent->rows[row_from].near
		};
	}

	std::string_view grid::default_style_class_name() const {
		return grid::DEFAULT_STYLE_CLASS_NAME;
	}

	void grid::on_initialize_controls() {
		basic_control_container::on_initialize_controls();

		this->bind_style_property(margins, properties::MARGINS);
		this->bind_style_property(padding, properties::PADDING);
	}

	void grid::on_paint(maxim::graphics &g, maxim::rect bounds) {
		// paint itself
		basic_control_container::on_paint(g, bounds);

		// apply margins to content
		bounds = maxim::layout::apply_margins(bounds, margins);

		// update partitions
		layout::partition::refresh(bounds.height, padding.get().y, rows, [](auto & row) -> decltype (row) & { return row; });
		layout::partition::refresh(bounds.width, padding.get().x, columns, [](auto & column) -> decltype (column) & { return column; });

		// paint children
		for (auto & object : controls ()) {
			object->on_paint (g, grid_item::layout (this, object.get(), bounds));
		}
	}

}