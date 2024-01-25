#pragma once
#ifndef _maxim_ui_textbox_hpp_
#define _maxim_ui_textbox_hpp_

#include "maxim.control.hpp"

#include <chrono>

namespace maxim {

	using validate_event = event < class textbox & /* sender */, std::string_view /* value */, bool & /* cancel */>;

	class textbox : public basic_control {
	public:
		using builder = specialized_builder < textbox >;

		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "text-box";
		std::string_view        default_style_class_name() const override;

		textbox();

		void on_initialize_controls() override;
		void on_paint(maxim::graphics &g, maxim::size size) override;
		void on_key_press(maxim::input::key_info const &key) override;

		// properties
		property < std::string > text;

		property < maxim::color > text_color;
		property < maxim::text_attributes > text_attributes;

		// events
		validate_event validate;
		virtual void on_validate (std::string_view value, bool & cancel);

	private:

		struct box_view {
		public:

			void update_width (int box_width, int text_width) {
				_box_width = box_width;
				track (text_width);
			}

			void cursor_home (int text_size) {
				_cursor = 0;
				track (text_size);
			}

			void cursor_end (int text_size) {
				_cursor = text_size;
				track (text_size);
			}

			void cursor_inc (int text_size) {
				if (_cursor < text_size) {
					++_cursor;
					track(text_size);
				}
			}

			void cursor_dec (int text_size) {
				if (_cursor > 0) {
					--_cursor;
					track(text_size);
				}
			}

			void track (int text_size) {
				_text_width = std::min (_box_width, text_size);

				if (text_size >= _box_width - 1 && cursor_at_end (text_size)) {
					--_text_width; // make way for the cursor at the end

					if (text_size >= _box_width) {
						_position = text_size - _text_width;
					} else {
						_position = 0;
					}
				} else {
					if (_cursor < _position) {
						// cursor to the left of the view
						_position = _cursor;
					} else if (_cursor > _position + _text_width) {
						// cursor to the right of the view
						_position = _cursor - _text_width;
					}
				}
			}

			bool cursor_at_end (int text_size) const {
				return _cursor == text_size;
			}

			int position () const { return _position; }
			int width () const { return _text_width; }
			int cursor () const { return _cursor; }
			int cursor_offset () const { return _cursor - _position; }

		private:
			int _cursor = 0;
			int _position = 0;
			int _text_width = 0;
			int _box_width = 0;
		};

		box_view                _view;
		static constexpr auto   BLINK_INTERVAL = std::chrono::milliseconds(500);
	};

	template < typename derived_type >
	class builder < textbox, derived_type > : public builder < control, derived_type > {
	public:
		using builder < control, derived_type >::builder;

		derived_type & text (std::string_view value) {
			this->get()->text = value;
			return this->derived();
		}

		derived_type & text_color (maxim::color value) {
			this->get()->text_color = value;
			return this->derived();
		}

		derived_type & text_attributes (maxim::text_attributes value) {
			this->get()->text_attributes = value;
			return this->derived();
		}
	};

}

#endif
