#pragma once
#ifndef _maxim_io_device_hpp_
#define _maxim_io_device_hpp_

#include <memory>

#include "maxim.character_buffer.hpp"
#include "maxim.details.hpp"
#include "maxim.encoder.hpp"
#include "maxim.input.driver.hpp"
#include "maxim.input.hpp"
#include "maxim.output.driver.hpp"

namespace maxim {

    /// Event handler for input events
    class input_event_handler {
    public:
        /// Called when a keyboard button is pressed down
        /// \param key - Key information
        virtual void on_key_up (input::key_info const & key);

        /// Called when a keyboard button is released
        /// \param key - Key information
        virtual void on_key_down (input::key_info const & key);

        /// Called when the application is notified of a key press (including repetitions)
        /// \param key - Key information
		virtual void on_key_press (input::key_info const & key);
    };

    /// Terminal Device
    struct device {
    public:

    	device ();
        ~device();

        /// Get the current size of the terminal
        /// \return 2D Size of the terminal
        [[nodiscard]] size const & get_size () const;

        /// Get a reference to the current backbuffer
        character_buffer & backbuffer();

        /// Get a reference to the encoder
        maxim::encoder & encoder ();

        /// Set the terminal to fullscreen mode
        /// \param enable - True to enable fullscreen mode
        void set_fullscreen(bool enable);

        /// Set the cursor visibility
        /// \param enable - True to enable cursor
        void set_cursor_enabled (bool enable);

        /// Set the color mode
        /// \param mode - Color mode
        void set_color_mode (color_mode mode);

        /// Clear the terminal
        void clear_screen();

        /// Present the backbuffer to the terminal
        void present ();

        /// Dispatch input events to the handler
        void dispatch_input_events (input_event_handler & handler) const;
    private:

        /// Synchonize display settings with the terminal
		void sync_display_settings ();

        std::unique_ptr < input::driver >
                            _input_driver = input::driver::create ();

        std::unique_ptr < output::driver >
                            _output_driver = output::driver::create ();

        size                _size { _output_driver->get_size() };
        character_buffer    _back_buffer { _size };
        maxim::encoder      _encoder;
        bool                _alternate_buffer_enabled {false};
        bool                _cursor_enabled { true };

    };
}

#endif
