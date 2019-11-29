#include <numeric>
#include "maxim.grid.hpp"

namespace maxim {

    void grid::update_layout(maxim::size const &size) {
        update_layout_partitions (size.height, rows);
        update_layout_partitions (size.width, columns);
    }

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

        for (auto & part : partitions) {
            if (part.type == value_type::constant)
                part.layout_value = part.value;
            else
                part.layout_value = rem_var * (part.value / dim_var);
        }
    }

}