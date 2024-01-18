#pragma once
#ifndef _maxim_ui_tab_panel_hpp_
#define _maxim_ui_tab_panel_hpp_

#include <maxim.control.hpp>
#include <maxim.layout.hpp>

#include <maxim.button.hpp>
#include <maxim.stack.hpp>

namespace maxim {

	class tab_panel_button : public button {
	public:
		std::string_view style_state () const override;

		class tab_panel * host_tab_panel = nullptr;
		int tab_index = -1;
	};

	class tab_panel_item : public virtual control {
	public:
		using builder = specialized_builder < tab_panel_item >;

		property < std::string >        tab_caption;
		property < layout::partition >  tab_length;

		static maxim::rect tab_layout (tab_panel_item * item, int min_pos, maxim::rect bounds);
		static maxim::rect layout (maxim::rect bounds, maxim::rect tab_bounds);
	};

	template < typename control_type >
	class composite_tab_panel_item : public tab_panel_item, public virtual control_type {
	public:
		static_assert(std::is_base_of_v < control, control_type >, "control_type must be derived from maxim::control");

		using builder = specialized_builder < composite_tab_panel_item < control_type > >;
		using control_type::control_type;
	};

	class tab_panel : public virtual control_container, public basic_control {
	public:
		using builder = specialized_builder < tab_panel >;

		constexpr static auto DEFAULT_STYLE_CLASS_NAME = "tab-panel";
		std::string_view default_style_class_name () const override;

		std::string_view style_state () const override;

		void on_initialize_controls () override;

		void on_activate () override;
		void on_deactivate () override;

		void on_paint (maxim::graphics & g, maxim::rect bounds) override;

		void select (int index);
		void select (tab_panel_item * item);

		int selected_index () const;
		tab_panel_item * selected () const;

		inline bool has_selected () const {
			return _selected_index >= 0 && _selected_index < tabs ().size ();
		}

		template < typename control_type, typename ... args_tv >
		inline composite_tab_panel_item < control_type > * make_tab (args_tv && ... args) {
			auto inst_uptr = std::make_unique < composite_tab_panel_item < control_type > > (std::forward < args_tv > (args) ...);
			auto * inst_ptr = inst_uptr.get ();

			this->_tabs.emplace_back (std::move (inst_uptr));
			this->on_control_added (inst_ptr, this->next_focus_index());

			// add actionable tab button
			auto * tab_button = _tab_button_stack.make_control < tab_panel_button > ();
			tab_button->host_tab_panel = this;
			tab_button->tab_index = _tabs.size() - 1;

			// bind tab properties
			tab_button->text.bind (inst_ptr->tab_caption);
			tab_button->length.bind (inst_ptr->tab_length);

			// set press handler
			tab_button->pressed.append_no_scope([this, tab_index=_tabs.size() - 1] (auto &) {
				this->select (tab_index);
			});

			// share our style info
			tab_button->style_class_name(this->style_class_name());

			// if is first panel, activate it
			if (_tabs.size() == 1) {
				this->select (0);
			}

			return inst_ptr;
		}

		template < typename control_type, typename ... args_tv >
		inline typename composite_tab_panel_item < control_type >::builder build_tab (args_tv && ... args) {
			auto * inst_ptr = make_tab < control_type > (std::forward < args_tv > (args) ...);
			return {inst_ptr};
		}

		void accept (control_visitor_t visitor) const override;
		std::vector < std::unique_ptr < tab_panel_item > > const & tabs () const;

	private:

		inline std::size_t next_focus_index () const {
			// we do not remove one as usual since we reserved zero for the tab button stack
			return _tabs.size();
		}

		stack                                               _tab_button_stack;
		std::vector < std::unique_ptr < tab_panel_item > >  _tabs;

		int _selected_index = 0;
	};

	template < typename derived_type >
	class builder < tab_panel_item, derived_type > {
	public:

		inline derived_type & tab_caption (std::string_view text) {
			auto & inst = *static_cast < derived_type * > (this);
			inst.get()->tab_caption = text;

			return inst;
		}

		inline derived_type & tab_length (layout::partition value) {
			auto & inst = *static_cast < derived_type * > (this);
			inst.get()->tab_length = value;

			return inst;
		}
	};

	template < typename derived_type >
	class builder < tab_panel, derived_type > : public builder < control, derived_type > {
	public:
		using builder < control, derived_type >::builder;
	};

}

#endif
