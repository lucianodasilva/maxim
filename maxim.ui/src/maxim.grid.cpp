#include <numeric>
#include <cmath>

#include "maxim.grid.hpp"

namespace maxim {

    float grid::calc_partition_sum(value_type type, std::vector<grid_part_size> const & partitions) {
        float sum {};

        for (auto & part : partitions) {
            if (part.type == type)
                sum += part.value;
        }

        return sum;
    }

    void grid::update_layout_partitions (int size, std::vector<grid_part_size> & partitions) {
        auto dim_const = calc_partition_sum(value_type::constant, partitions);
        auto dim_var = calc_partition_sum(value_type::variable, partitions);

        auto rem_var = static_cast < float > (size) - dim_const;

        if (rem_var < .0F)
            rem_var = .0F;

        float sum { .0F };

        for (auto & part : partitions) {
            part.layout_sum = sum;

            if (part.type == value_type::constant)
                part.layout_value = part.value;
            else
                part.layout_value = rem_var * (part.value / dim_var);

            sum += part.layout_value;
        }
    }

    void grid::paint(maxim::graphics & g, const maxim::rect &bounds) {
        // update partitions
        update_layout_partitions (bounds.size.height, rows);
        update_layout_partitions (bounds.size.width, columns);

        // paint itself
        panel::paint(g, bounds);

        // calculate element bounds and paint
        for (auto & element : _elements) {
            int
                col_from = std::clamp < int > (
                        element->columns.get().from,
                        0,
                        columns.size () - 1),
                col_to = std::clamp < int > (
                        element->columns.get().to,
                        0,
                        columns.size () - 1);

            int
                row_from = std::clamp < int > (
                        element->rows.get().from,
                        0,
                        rows.size() - 1),
                row_to = std::clamp < int > (
                        element->rows.get().to,
                        0,
                        rows.size() - 1);

            auto element_bounds = maxim::rect {
                bounds.position + maxim::point {
                    static_cast < int > (std::round (columns [col_from].layout_sum)),
                    static_cast < int > (std::round (rows [row_from].layout_sum))
                }, maxim::size {
                    static_cast < int > (std::round (columns [col_to].layout_sum - columns [col_from].layout_sum + columns [col_to].layout_value)),
                    static_cast < int > (std::round (rows [row_to].layout_sum - rows [row_from].layout_sum + rows  [row_to].layout_value))
                }
            };

            element->paint (g, element_bounds);
        }
    }



}