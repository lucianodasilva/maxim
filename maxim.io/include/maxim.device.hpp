#pragma once
#ifndef MAXIM_DEVICE_HPP
#define MAXIM_DEVICE_HPP

#include <memory>
#include <vector>

#include "maxim.details.hpp"
#include "maxim.character.hpp"
#include "maxim.encoder.hpp"
#include "maxim.input.hpp"
#include "maxim.input.driver.hpp"
#include "maxim.output.driver.hpp"
#include "maxim.graphics.hpp"
#include "maxim.stream.hpp"

namespace maxim {

    class input_event_handler {
    public:
        virtual void on_key_up (input::key_info info);
        virtual void on_key_down (input::key_info info);
    };

    struct device {
    public:

    	device ();
        ~device();

        [[nodiscard]]
        size const & terminal_size () const;

        maxim::graphics graphics (point transform = {});
        character_buffer & backbuffer();
        maxim::encoder & encoder ();

        [[nodiscard]] bool fullscreen() const;
        void fullscreen(bool enable);

        [[nodiscard]] bool cursor() const;
        void cursor (bool enable);

        [[nodiscard]] maxim::color_mode color_mode() const;
        void color_mode (maxim::color_mode mode);

        void clear();
        void present ();

        void dispatch_input_events (input_event_handler & handler) const;

    private:

		void sync_display_settings ();

        std::unique_ptr < input::driver >
                            _input_driver = input::driver::create ();

        std::unique_ptr < output::driver >
                            _output_driver = output::driver::create ();

        size                _size {_output_driver->terminal_size() };
        character_buffer    _back_buffer { _size };
        maxim::encoder      _encoder;
        bool                _alternate_buffer_enabled {false};
        bool                _cursor_enabled { true };

    };
}

#endif
