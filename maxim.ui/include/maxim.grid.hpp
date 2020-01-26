#pragma once

#ifndef _maxim_grid_hpp_
#define _maxim_grid_hpp_

#include "maxim.panel.hpp"

#include <vector>
#include <maxim.io.hpp>

namespace maxim {

    enum struct value_type {
        constant,
        variable
    };

    struct grid_part_size {
    public:

        float       value;
        value_type  type;

        float       layout_sum;
        float       layout_value;
    };

    constexpr grid_part_size operator "" _const (long double value) {
        return { static_cast < float > (value), value_type::constant, .0F };
    }

    constexpr grid_part_size operator "" _var (long double value) {
        return { static_cast < float > (value), value_type::variable, .0F };
    }

    class grid : public panel {
    public:

        struct range {
            range () = default;

            explicit range (int value) :
                from {value},
                to {value}
            {}

            explicit range (int from_v, int to_v) :
                from { from_v },
                to { to_v }
            {}

            int from {}, to {};
        };

        class base_item : public virtual paintable {
        public:
            property < range > columns;
            property < range > rows;
        };

        template < typename _t >
        class item : public base_item, public _t {
        public:
        };

        std::vector < grid_part_size >
                columns,
                rows;

        void paint (graphics & g, rect const & bounds) override;

        template < typename _t >
        inline item < _t > * create_element () {
            auto * new_item = new item < _t > ();
            _elements.emplace_back (new_item);

            return new_item;
        }

    private:

        static float calc_partition_sum (value_type type, std::vector < grid_part_size > const & partitions);

        static void update_layout_partitions (int size, std::vector < grid_part_size > & partitions);

        std::vector < std::unique_ptr < base_item > > _elements;
    };

}

#endif // _maxim_grid_hpp_
