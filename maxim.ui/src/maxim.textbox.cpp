#include "maxim.textbox.hpp"
#include "maxim.application.hpp"

namespace maxim {

	std::string_view textbox::default_style_class_name() const {
		return textbox::DEFAULT_STYLE_CLASS_NAME;
	}

	textbox::textbox() {
		this->tab_stop = true;
	}

	void textbox::on_initialize_controls() {
		basic_control::on_initialize_controls();

		this->bind_style_property(text_color, properties::TEXT_COLOR);
	}

	void textbox::on_paint(maxim::graphics &g, maxim::rect bounds) {

		// calculate text window
		_view.update_width(bounds.width, text.get().size());

		// render background
		g.draw_box(bounds, background, true);

		// render cursor
		if (this->has_focus()) {

			// render text
			{
				// calculate text window
				if (_view.width () > 0) {
					auto text_to_display = text.get().substr(_view.position(), _view.width ());
					g.draw_text(text_to_display, bounds, text_color, maxim::text_attributes { maxim::alignment::near, maxim::alignment::center });
				}
			}

			auto epoch_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now().time_since_epoch()).count();

			if (epoch_ms % (BLINK_INTERVAL.count() * 2) < BLINK_INTERVAL.count()) {
				auto calc_position = std::min (_view.cursor_offset(), bounds.width - 1);

				// direct manipulation of the buffer
				auto * cursor_char = g.get_char (bounds.position() + maxim::point{calc_position, 0});

				if (cursor_char != nullptr) {
					cursor_char->attributes.flags.underlined = true;
				}
			}
		} else {
			// render text
			g.draw_text(text, bounds, text_color, maxim::text_attributes { maxim::alignment::near, maxim::alignment::center });
		}
	}

	void textbox::on_key_press(maxim::input::key_info const &key) {

		// if alphanumeric, append to text
		if (std::isprint(key.char_value) != 0) {

			if (_view.cursor_at_end (text.get().size())) {
				// append to end
				text.get().push_back (key.char_value);
			} else {
				// insert text at cursor position
				text.get().insert (text.get().begin() + _view.cursor(), key.char_value);
			}

			_view.cursor_inc(text.get().size());

			return;
		}

		// if backspace, remove char before cursor
		if (key.matches (maxim::input::virtual_key::backspace)) {
			if (!text.get().empty()) {
				// remove last character
				if (_view.cursor_at_end(text.get().size())) {
					text.get().pop_back();
				} else {
					text.get().erase (text.get().begin() + _view.cursor() - 1);
				}

				_view.cursor_dec(text.get().size());
			}
			return;
		}

		// if delete, remove char after cursor
		if (key.matches (maxim::input::virtual_key::del)) {
			if (!text.get().empty()) {
				// remove last character
				if (!_view.cursor_at_end(text.get().size())) {
					text.get().erase (text.get().begin() + _view.cursor());
				}
			}
			return;
		}

		// cursor home
		if (key.matches (maxim::input::virtual_key::home)) {
			_view.cursor_home (text.get().size());
			return;
		}

		// cursor end
		if (key.matches (maxim::input::virtual_key::end)) {
			_view.cursor_end (text.get().size());
			return;
		}

		// cursor advance
		if (key.matches (maxim::input::virtual_key::right)) {
			_view.cursor_inc(text.get().size());
			return;
		}

		// cursor back
		if (key.matches (maxim::input::virtual_key::left)) {
			_view.cursor_dec(text.get().size());
			return;
		}

		// on enter, validate, and focus next
		if (key.matches(maxim::input::virtual_key::enter) || key.matches (maxim::input::virtual_key::tab)) {
			bool canceled = false;
			on_validate (text.get(), canceled);

			if (!canceled) {
				if (this->application()) {
					this->application()->focus_next();
				}
			}

			return;
		}

		// on enter, validate, and focus next
		if (key.matches(maxim::input::virtual_key::tab, maxim::input::modifier_key::shift)) {
			bool canceled = false;
			on_validate (text.get(), canceled);

			if (!canceled) {
				if (this->application()) {
					this->application()->focus_prev();
				}
			}

			return;
		}

		// do remaining default key behaviour
		basic_control::on_key_press(key);
	}

	void textbox::on_validate (std::string_view value, bool & cancel) {
		validate.invoke (*this, value, cancel);
	}

}