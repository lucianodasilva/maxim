#pragma once

#ifndef _maxim_grid_hpp_
#define _maxim_grid_hpp_

#include "maxim.panel.hpp"

#include <vector>
#include <maxim.io.hpp>

namespace maxim {

    enum struct length_kind {
        constant,
        variable
    };

    struct length_t {
    public:
        float       value;
        length_kind kind;

        int
                    layout_begin,
                    layout_end;
    };

    constexpr length_t operator "" _fixed (long double value) {
        return {static_cast < float > (value), length_kind::constant, 0, 0 };
    }

    constexpr length_t operator "" _var (long double value) {
        return {static_cast < float > (value), length_kind::variable, 0, 0 };
    }

	struct grid_range {
		grid_range () = default;

		grid_range (int value) :
				from {value},
				to {value}
		{}

		grid_range (int from_v, int to_v) :
				from { from_v },
				to { to_v }
		{}

		int from {}, to {};
	};

	class grid_item_base : public virtual paintable {
	public:
		property < grid_range > columns { grid_range {0}};
		property < grid_range > rows { grid_range {0}};
	};

	template < typename _t >
	class grid_item : public grid_item_base, public _t {
	public:
	};

    class grid : public panel {
    public:

        std::vector < length_t >
            columns { 1.0_var },
            rows { 1.0_var };

        void paint (graphics & g, rect const & bounds) override;

        template < typename _t >
        inline grid_item < _t > * create_element () {
            auto * new_item = new grid_item < _t > ();
            _elements.emplace_back (new_item);

            return new_item;
        }

    private:

        static float calc_partition_sum (length_kind type, std::vector < length_t > const & partitions);

        static void update_layout_partitions (int size, std::vector < length_t > & partitions);

        std::vector < std::unique_ptr < grid_item_base > > _elements;
    };

}

#endif // _maxim_grid_hpp_
