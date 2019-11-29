#pragma once

#ifndef _maxim_grid_hpp_
#define _maxim_grid_hpp_

#include <vector>
#include "maxim.io.hpp"

namespace maxim {

    enum struct value_type {
        constant,
        variable
    };

    struct grid_part_size {
    public:

        float       value;
        value_type  type;

        float       layout_value;
    };

    constexpr grid_part_size operator "" _const (long double value) {
        return { static_cast < float > (value), value_type::constant, .0F };
    }

    constexpr grid_part_size operator "" _var (long double value) {
        return { static_cast < float > (value), value_type::variable, .0F };
    }

    class grid {
    public:

        std::vector < grid_part_size >
                columns,
                rows;

        void update_layout (maxim::size const & size);

        static float calc_partition_sum (value_type type, std::vector < grid_part_size > const & partitions);

        static void update_layout_partitions (int size, std::vector < grid_part_size > & partitions);
    };

}

#endif // _maxim_grid_hpp_
