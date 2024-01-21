#pragma once
#ifndef _maxim_ui_property_hpp_
#define _maxim_ui_property_hpp_

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

		property (value_type const & value) : _variant (value) {}                  // NOLINT
		property (value_type && value) noexcept : _variant (std::move (value)) {}  // NOLINT

		property (property const & other) : _variant (other._variant) {}

		property & operator = (value_type const & value) {
			store (value);
			return *this;
		}

		property & operator = (value_type && value) {
			store (value);
			return *this;
		}

		void set (value_type const & value) {
			store (value);
		}

		void set (value_type && value) {
			store(std::move (value));
		}

		[[nodiscard]] value_type const & get () const {
			return load ();
		}

		[[nodiscard]] value_type & get () {
			return const_cast < value_type & > (std::as_const (*this).get ());
		};

		operator value_type const & () const { // NOLINT
			return get ();
		}

		void bind (property & other) {
			other.promote_binding();

			_variant = std::weak_ptr < binding >(
				other.is_bound() ?
					std::get < std::weak_ptr < binding > >(other._variant) :
					std::get < std::shared_ptr < binding > >(other._variant));

			on_change();
		}

		void bind (std::function < value_type () > getter) {
			this->promote_binding();
			std::get < std::shared_ptr < binding > > (_variant)->getter = getter;

			on_change();
		}

	private:

		struct binding {
		public:

			binding () = default;

			explicit binding (value_type && value) noexcept:
				storage (std::move (value))
			{}

			explicit binding (getter_type && getter_inst) noexcept:
				getter (std::move (getter_inst))
			{}

			void store (value_type const & value) {
				storage = value;
				getter = nullptr;
			}

			void store (value_type && value) {
				storage = std::move (value);
				getter = nullptr;
			}

			void store (getter_type && getter_inst) {
				this->getter = std::move (getter_inst);
			}

			[[nodiscard]]
			value_type const & load () const {
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
			std::shared_ptr < binding >,	// heap	value
			std::weak_ptr < binding > >;	// binding

		template < typename arg_type >
		void store (arg_type && arg) {
			if (!this->is_local() && !this->is_bound()) {
				std::get < std::shared_ptr < binding > > (_variant)->store (std::forward < arg_type > (arg));
			} else {
				_variant = std::forward<arg_type>(arg);
			}

			this->on_change();
		}

		void store (getter_type && getter_inst) {
			if (this->is_local ()) {
				this->promote_binding();
			}

			std::get < std::shared_ptr < binding > > (_variant)->store (std::move (getter_inst));

			this->on_change();
		}

		value_type const & load () const {
			if (is_local()) {
				// if local, just return value
				return std::get < value_type >(_variant);
			} else {
				std::shared_ptr < binding > shared_binding;
				if (is_bound()) {
					if (std::get < std::weak_ptr < binding > > (_variant).expired()) {
						// if original binding is expired, reset to local
						_variant = value_type{};
						return std::get < value_type > (_variant);
					};

					// if bound, get shared pointer
					return std::get < std::weak_ptr < binding > > (_variant)
						.lock()
						->load();
				} else {
					// if heap value, get shared pointer
					return std::get < std::shared_ptr < binding > >(_variant)
						->load();
				}
			}
		}

		void promote_binding () {
			if (is_local()) {
				// "promote" local value to heap
				_variant = std::make_shared < binding >(
					std::move(
						std::get < value_type >(_variant)));
			}
		}

		[[nodiscard]] bool is_local () const {
			return std::holds_alternative < value_type > (_variant);
		}

		[[nodiscard]] bool is_bound () const {
			return std::holds_alternative < std::weak_ptr < binding > > (_variant);
		}

		void on_change () {
			changed.invoke (*this, this->load ());
		}

		mutable variant_type    _variant = { value_type {} };
	};

}

#endif
