#pragma once
#ifndef MAXIM_LAYOUT_HPP
#define MAXIM_LAYOUT_HPP

#include <maxim.io.hpp>
#include <vector>
#include <cmath>

namespace maxim {

	struct margins {
	public:
		maxim::point near;
		maxim::point far;
	};

	namespace layout {

		enum struct partition_kind {
			constant, /// fixed size
			variable  /// variable size
		};

		struct partition {
		public:
			float length;
			partition_kind kind;

			int near;
			int far;

			template<typename iterable_t, typename getter_t>
			static inline float sum (partition_kind kind, iterable_t const & iterable, getter_t const & getter);

			template<typename iterable_t, typename getter_t>
			static inline void refresh(int size, iterable_t & iterable, getter_t const & getter);
		};

		struct partition_range {

			partition_range() = default;

			inline partition_range(int value) :
				from{value},
				to{value} {}

			inline partition_range(int from_v, int to_v) :
				from{from_v},
				to{to_v} {}

			int from{}, to{};
		};

		template<typename iterable_t, typename getter_t>
		float partition::sum (partition_kind kind, iterable_t const & iterable, getter_t const & getter) {
			float sum {};

			for (auto const & item : iterable) {
				auto const & part = getter (item);

				if (part.kind == kind) {
					sum += part.length;
				}
			}

			return sum;
		}

		template<typename iterable_t, typename getter_t>
		void partition::refresh (int size, iterable_t & iterable, getter_t const & getter) {
			// if there are no partitions, return
			if (iterable.empty ()) {
				return;
			}

			// get sum of all constant and variable partitions
			auto dim_const = partition::sum(partition_kind::constant, iterable, getter);
			auto dim_var = partition::sum(partition_kind::variable, iterable, getter);

			// get remaining variable space
			auto rem_var = static_cast < float > (size) - dim_const;

			if (rem_var < .0F) {
				rem_var = .0F;
			}

			// update partitions
			float sum{.0F};

			// update layout begin
			for (auto & item : iterable) {
				auto & part = getter (item);

				// set begin to the current sum
				part.near = static_cast < int > (std::round(sum));

				if (part.kind == partition_kind::constant) {
					// if the pattern is constant, add to the sum
					sum += part.length;
				} else {
					// if the pattern is variable, add to the sum proportionally
					sum += rem_var * (part.length / dim_var);
				}
			}

			// update layout end
			{
				auto it = iterable.begin();
				auto next_it = std::next(it);

				while (next_it != iterable.end()) {
					getter (*it).far = getter(*next_it).near;
					++it; ++next_it;
				}

				// set last partition to the size
				getter(*it).far = size;
			}
		}

		maxim::rect apply_margins (maxim::rect bounds, margins const & margins);
	}

	constexpr layout::partition operator "" _fixed (long double value) {
		return {static_cast < float > (value), layout::partition_kind::constant, 0, 0 };
	}

	constexpr layout::partition operator "" _var (long double value) {
		return {static_cast < float > (value), layout::partition_kind::variable, 0, 0 };
	}

}

#endif
