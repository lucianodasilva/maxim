#include <numeric>
#include <cmath>

#include "maxim.grid.hpp"

namespace maxim {

    float grid::calc_partition_sum (length_kind type, std::vector<length_t> const & partitions) {
        float sum {};

        for (auto & part : partitions) {
            if (part.kind == type)
                sum += part.value;
        }

        return sum;
    }

    void grid::update_layout_partitions (int size, std::vector<length_t> & partitions) {
        
        if (partitions.empty())
            return;

        auto dim_const = calc_partition_sum(length_kind::constant, partitions);
        auto dim_var = calc_partition_sum(length_kind::variable, partitions);

        auto rem_var = static_cast < float > (size) - dim_const;

        if (rem_var < .0F)
            rem_var = .0F;

        float sum { .0F };

        for (auto & part : partitions) {
            part.layout_begin = static_cast <int> (std::round(sum));

            if (part.kind == length_kind::constant)
                sum += part.value;
            else
                sum += rem_var * (part.value / dim_var);
        }

        for (int i = 0; i < partitions.size() - 1; ++i) {
            partitions[i].layout_end = partitions[i + 1].layout_begin;
        }

        partitions.back().layout_end = size;
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

            auto element_bounds = maxim::rect{
                bounds.position + maxim::point {
                    columns [col_from].layout_begin,
                    rows [row_from].layout_begin
                }, maxim::size {
                    columns [col_to].layout_end - columns [col_from].layout_begin,
                    rows [row_to].layout_end - rows[row_from].layout_begin
                }
            };

            element->paint (g, element_bounds);
        }
    }



}