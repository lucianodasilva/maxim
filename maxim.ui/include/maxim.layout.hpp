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
			partition () = default;
			constexpr inline partition (float length) : length(length) {}
			constexpr inline partition (int length) : length (static_cast < int > (length)) {}
			constexpr inline partition (float length, partition_kind kind) : length{length}, kind{kind} {}

			template<typename iterable_t, typename getter_t>
			static inline float sum (partition_kind kind, iterable_t const & iterable, getter_t const & getter);

			template<typename iterable_t, typename getter_t>
			static inline void refresh(int size, int padding, iterable_t & iterable, getter_t const & getter);

			float length = .0F;
			partition_kind kind = partition_kind::constant;

			int near = 0;
			int far = 0;
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
		void partition::refresh (int size, int padding, iterable_t & iterable, getter_t const & getter) {
			// if there are no partitions, return
			if (iterable.empty ()) {
				return;
			}

			auto count = std::distance (std::begin (iterable), std::end (iterable));

			// get sum of all constant and variable partitions
			auto dim_const = partition::sum(partition_kind::constant, iterable, getter);
			auto dim_var = partition::sum(partition_kind::variable, iterable, getter);

			// get remaining variable space
			auto rem_var = static_cast < float > (size) - (dim_const + padding * (count - 1));

			if (rem_var < .0F) {
				rem_var = .0F;
			}

			// update partitions
			float sum {.0F};

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

				sum += padding;
			}

			// update layout end
			{
				auto it = iterable.begin();
				auto next_it = std::next(it);

				while (next_it != iterable.end()) {
					getter (*it).far = getter(*next_it).near - padding;
					++it; ++next_it;
				}

				if (getter(*it).kind == partition_kind::constant){
					getter(*it).far = getter(*it).near + getter(*it).length;
				} else {
					getter(*it).far = size;
				}
			}
		}

		maxim::rect apply_margins (maxim::rect bounds, margins const & margins);
	}

	constexpr layout::partition operator "" _fixed (long double value) {
		return {static_cast < float > (value), layout::partition_kind::constant };
	}

	constexpr layout::partition operator "" _fixed (unsigned long long  value) {
		return {static_cast < float > (value), layout::partition_kind::constant };
	}

	constexpr layout::partition operator "" _var (long double value) {
		return {static_cast < float > (value), layout::partition_kind::variable };
	}

	constexpr layout::partition operator "" _var (unsigned long long value) {
		return {static_cast < float > (value), layout::partition_kind::variable };
	}

}

#endif
