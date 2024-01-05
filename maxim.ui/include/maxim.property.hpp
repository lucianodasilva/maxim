#pragma once
#ifndef _maxim_property_hpp_
#define _maxim_property_hpp_

#include "maxim.event.hpp"

#include <functional>
#include <variant>
#include <utility>

namespace maxim {

	template < typename value_type >
	struct property;

	template < typename value_type >
	using property_changed_event = maxim::event < property < value_type > &, value_type const & >;

	template < typename value_type >
	class property {
	public:

		property_changed_event < value_type > changed;

		using getter_type = std::function < value_type () >;

		property () = default;

		inline property (value_type const & value) : _variant (value) {}                  // NOLINT
		inline property (value_type && value) noexcept : _variant (std::move (value)) {}  // NOLINT

		inline property (property const & other) : _variant (other._variant) {}

		inline property & operator = (value_type const & value) {
			store (value);
			return *this;
		}

		inline property & operator = (value_type && value) {
			store (value);
			return *this;
		}

		inline void set (value_type const & value) {
			store (value);
		}

		inline void set (value_type && value) {
			store(std::move (value));
		}

		[[nodiscard]] inline value_type const & get () const {
			return load ();
		}

		[[nodiscard]] inline value_type & get () {
			return const_cast < value_type & > (std::as_const (*this).get ());
		};

		inline operator value_type const & () const { // NOLINT
			return get ();
		}

		inline void bind (property & other) {
			other.notify_binding();

			_variant = other._variant;
			_is_bound = true;

			on_change();
		}

		inline void bind (std::function < value_type () > getter) {
			this->notify_binding();
			std::get < std::shared_ptr < binding > > (_variant)->getter = getter;

			on_change();
		}

		inline void unbind () {
			this->demote ();
			_is_bound = false;
		}

	private:

		struct binding {
		public:

			binding () = default;

			inline explicit binding (value_type && value) noexcept:
				storage (std::move (value))
			{}

			inline explicit binding (getter_type && getter_inst) noexcept:
				getter (std::move (getter_inst))
			{}

			inline void store (value_type const & value) {
				storage = value;
				getter = nullptr;
			}

			inline void store (value_type && value) {
				storage = std::move (value);
				getter = nullptr;
			}

			inline void store (getter_type && getter_inst) {
				this->getter = std::move (getter_inst);
			}

			[[nodiscard]]
			inline value_type const & load () const {
				if (getter) {
					storage = getter ();
				}

				return storage;
			}

			getter_type         getter;
			mutable value_type  storage {};
		};

		using variant_type = std::variant <
			value_type,                     // value
			std::shared_ptr < binding > >;  // binding

		template < typename arg_type >
		inline void store (arg_type && arg) {
			if (!this->is_local() && !this->is_bound()) {
				std::get < std::shared_ptr < binding > > (_variant)->store (std::forward < arg_type > (arg));
			} else {
				_variant = std::forward<arg_type>(arg);
			}

			this->on_change();
		}

		inline void store (getter_type && getter_inst) {
			if (this->is_local ()) {
				this->notify_binding();
			}

			std::get < std::shared_ptr < binding > > (_variant)->store (std::move (getter_inst));

			this->on_change();
		}

		inline value_type const & load () const {
			if (!is_local()) {
				auto & shared_binding = std::get < std::shared_ptr < binding > > (_variant);

				if (shared_binding) {
					return shared_binding->load ();
				}
			}

			return std::get < value_type > (_variant);
		}

		inline void notify_binding () {
			// if already binding, do nothing
			if (!is_local()) { return; }

			// "promote" to binding
			_variant = std::make_shared < binding > (
				std::move (
					std::get < value_type > (_variant)));
		}

		[[nodiscard]] inline bool is_local () const {
			return std::holds_alternative < value_type > (_variant);
		}

		[[nodiscard]] inline bool is_bound () const {
			return _is_bound;
		}

		inline void on_change () {
			changed.invoke (*this, this->load ());
		}

		mutable variant_type    _variant = { value_type {} };
		bool                    _is_bound = false;
	};

}

#endif
