#pragma once
#ifndef _maxim_control_hpp_
#define _maxim_control_hpp_

#include "maxim.focus.hpp"
#include <maxim.io.hpp>
#include "maxim.key_handler.hpp"
#include "maxim.paintable.hpp"
#include "maxim.property.hpp"
#include "maxim.style.hpp"

#include <optional>
#include <set>

namespace maxim {

	struct graphics;
	struct rect;

	using control_event = event < class control * /* sender */ >;
	using focus_event = event < class control & /* sender */ >;

	class control :
		public style_client,
		public basic_paintable < control >,
		public basic_key_handler < control >
	{
	public:
		virtual bool has_focus () const = 0;
		virtual void focus () = 0;

		focus_event focused;
		focus_event focus_lost;

		virtual void on_focus () = 0;
		virtual void on_focus_lost () = 0;

		property < bool > tab_stop;

		/// get the focus index of this object
		/// @return a const reference to the focus index
		[[nodiscard]]
		maxim::focus_index const & focus_index () const;

		/// set the focus index of this object
		/// @param index the new focus index
		void focus_index (maxim::focus_index const & index);

		// Control specific members
		virtual void on_initialize_controls () = 0;

		virtual void on_activate () = 0;
		virtual void on_deactivate () = 0;

		control_event activated;
		control_event deactivated;

		control * root ();
		control const * root () const;

		virtual bool is_active () const = 0;

		class control_container * parent () const;
		class application * application () const;

		void attach_to_tree (class application * app, class control_container * parent);

	private:
		class control_container * parent (control_container * parent);
		void application (class application * application);

		class application *         _application = nullptr;
		class control_container *   _parent = nullptr;
		maxim::focus_index          _focus_index;
	};

	class basic_control : public virtual control
	{
	public:
		maxim::style &              style () override;
		maxim::style const &        style () const override;
		std::string_view            style_state () const override;

		// Focus related members
		void on_key_press (maxim::input::key_info const & key) override;

		bool has_focus () const override;
		void focus () override;

		void on_focus () override;
		void on_focus_lost () override;

		// Paintable related members
		void on_paint (graphics & g, rect bounds) override;

		// Control specific members
		void on_initialize_controls () override;

		void on_activate () override;
		void on_deactivate () override;

		bool is_active () const override;
	};

	template < typename control_type >
	class specialized_builder;

	template < typename derived_type >
	class builder_base;

	template < typename control_type >
	class builder_base < specialized_builder < control_type > > {
	public:
		inline explicit builder_base (control_type * inst) : _instance (inst)
		{}

		inline control_type * get () {
			return _instance;
		}
	protected:
		auto & derived () {
			return *static_cast < specialized_builder < control_type > * > (this);
		}
	private:
		control_type * _instance = nullptr;
	};

	template < typename control_type, typename derived_type >
	class builder;

	template < typename derived_type >
	class builder < control, derived_type > : public builder_base < derived_type > {
	public:
		using builder_base < derived_type >::builder_base;

		inline auto & no_tab_stop () {
			this->get()->tab_stop = false;
			return this->derived();
		}

		inline auto & tab_stop (bool value = true) {
			this->get()->tab_stop = value;
			return this->derived();
		}

		inline auto & style_class_name (std::string_view value) {
			this->get()->style_class_name (value);
			return this->derived();
		}
	};

	template < typename control_type >
	class specialized_builder :
		public builder < control_type, specialized_builder < control_type > >
    {
		using builder < control_type, specialized_builder < control_type > >::builder;
	};

}

#endif
