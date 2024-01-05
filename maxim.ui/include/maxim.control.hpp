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

	class control :
		public style_client,
		public paintable < control >,
	    public focusable < control >
	{
	public:
		maxim::style &              style () override;
		maxim::style const &        style () const override;
		std::string_view            style_state () const override;

		// Focus related members
		void on_key_press (maxim::input::key_info const & key) override;

		bool has_focus () const;
		void focus ();

		// Paintable related members
		void on_paint (graphics & g, rect bounds) override;

		// Control specific members
		virtual void on_initialize_controls ();

		virtual void on_activate ();
		virtual void on_deactivate ();

		control_event activated;
		control_event deactivated;

		class control_container * parent () const;
		class application * application () const;

		control * root ();
		control const * root () const;

		bool is_active () const;
	private:

		void attach_to_tree (class application * app, class control_container * parent);

		inline class control_container * parent (control_container * parent) {
			_parent = parent;
			return _parent;
		}

		inline void application (class application * application) {
			_application = application;
		}

		class application *          _application = nullptr;
		class control_container *   _parent = nullptr;

		friend class control_container;
		friend class application;
	};

	class control_container : public control {
	public:
		void on_initialize_controls () override;

		void on_activate () override;
		void on_deactivate () override;

		void on_paint (graphics & g, rect bounds) override;

	protected:
		void push_back_control (std::unique_ptr < control > && control);
	private:
		using control_list_t = std::vector < std::unique_ptr < control > >;
		control_list_t _controls;
	};

	template < typename item_type, template < class > typename wrapper_type >
	class specialized_control_container : public control_container {
	public:

		template < typename control_type, typename ... args_tv >
		inline wrapper_type < control_type > * make_control (args_tv && ... args) {
			auto inst_uptr = std::make_unique < wrapper_type < control_type > > (std::forward < args_tv > (args)...);
			auto * ptr = inst_uptr.get ();

			this->push_back_control(std::move (inst_uptr));
			this->on_control_added (ptr);

			return ptr;
		}

	protected:
		virtual void on_control_added (item_type * item) {}
	};

	class basic_root_control : public control {
	public:
		void close ();
	};

}

#endif
