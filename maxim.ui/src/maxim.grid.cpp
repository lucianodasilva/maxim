#include "maxim.grid.hpp"
#include "maxim.layout.hpp"

namespace maxim {

	maxim::rect basic_grid_item::layout(class maxim::grid *parent, maxim::rect bounds) const {

		if (parent == nullptr) {
			return bounds;
		}

		int col_from = std::clamp<int>(columns.get().from, 0, parent->columns.size() - 1);
		int col_to = std::clamp<int>(columns.get().to, 0, parent->columns.size() - 1);

		int row_from = std::clamp<int>(rows.get().from, 0, parent->rows.size() - 1);
		int row_to = std::clamp<int>(rows.get().to, 0, parent->rows.size() - 1);

		return maxim::rect{
			bounds.position + maxim::point{
				parent->columns[col_from].near,
				parent->rows[row_from].near
			},
			maxim::size{
				parent->columns[col_to].far - parent->columns[col_from].near,
				parent->rows[row_to].far - parent->rows[row_from].near
			}
		};
	}

	std::string_view grid::default_style_class_name() const {
		return grid::DEFAULT_STYLE_CLASS_NAME;
	}

	void grid::on_paint(maxim::graphics &g, maxim::rect bounds) {
		// update partitions
		layout::partition::refresh(bounds.size.height, rows, [](auto & row) -> decltype (row) & { return row; });
		layout::partition::refresh(bounds.size.width, columns, [](auto & column) -> decltype (column) & { return column; });

		// paint itself and paint children
		control_container::on_paint(g, bounds);
	}

}