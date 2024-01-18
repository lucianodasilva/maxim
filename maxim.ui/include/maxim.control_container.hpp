#pragma once
#ifndef MAXIM_CONTROL_CONTAINER_HPP
#define MAXIM_CONTROL_CONTAINER_HPP

#include "maxim.control.hpp"

namespace maxim {

	using control_visitor_t = std::function < bool (control *) >;

	class control_container : public virtual control {
	public:
		virtual void accept(control_visitor_t visitor) const = 0;

	protected:
		void on_control_added(maxim::control *control, std::size_t index);
	};

	class basic_control_container : public virtual control_container, public basic_control {
	public:
		void on_initialize_controls() override;

		void on_activate() override;

		void on_deactivate() override;
	};

	template < typename control_type, typename item_type >
	class composite_item : public control_type, public item_type {
	public:
		static_assert(std::is_base_of_v < control, control_type >, "control_type must be derived from maxim::control");

		using builder = specialized_builder < composite_item < control_type, item_type > >;
		using control_type::control_type;
	};

	template < typename control_type, typename item_type, typename derived_type >
	class builder < composite_item < control_type, item_type >, derived_type > :
		public builder < control_type, derived_type >,
		public builder < item_type, derived_type >
	{
	public:
		using builder < control_type, derived_type >::builder;
	};

	template < typename item_type >
	class composite_control_container : public basic_control_container {
	public:

		template < typename control_type >
		using composite_item_type = composite_item < control_type, item_type >;

		template<typename control_type, typename ... args_tv>
		inline composite_item_type < control_type > * make_control(args_tv &&... args) {
			// create instance and store pointer to use after move into container
			auto inst_uptr = std::make_unique< composite_item_type < control_type > >(std::forward<args_tv>(args)...);
			auto *ptr = inst_uptr.get();

			// move into control container
			this->_controls.emplace_back(std::move(inst_uptr));
			// setup control into control hierarchy
			this->on_control_added(ptr, this->_controls.size() - 1);

			return ptr;
		}

		template<typename control_type, typename ... args_tv>
		inline auto build_control (args_tv &&... args) {
			auto * ctrl = make_control < control_type > (std::forward<args_tv>(args)...);
			return typename composite_item_type < control_type >::builder (ctrl);
		}

		inline void accept(control_visitor_t visitor) const override {
			if (!visitor) {
				return;
			}

			for (auto &object: this->controls()) {
				if (!visitor(object.get())) {
					break;
				}
			}
		}

		inline std::vector <std::unique_ptr<item_type>> const &controls() const {
			return _controls;
		}

	private:
		std::vector <std::unique_ptr<item_type>> _controls;
	};

	class basic_root_control : public control {
	public:
		void close();
	};

}
#endif
