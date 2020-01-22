#pragma once
#ifndef _maxim_device_hpp_
#define _maxim_device_hpp_

#include <memory>
#include <vector>

#include "maxim.details.hpp"
#include "maxim.character.hpp"
#include "maxim.input.hpp"
#include "maxim.input.driver.hpp"
#include "maxim.output.driver.hpp"
#include "maxim.graphics.hpp"
#include "maxim.stream.hpp"

namespace maxim {

    class input_event_handler {
    public:
        virtual void on_key_up (input::key_info const & key);
        virtual void on_key_down (input::key_info const & key);
    };

    struct device {
    public:

        ~device();

        [[nodiscard]]
        size const & get_size () const;

        character_buffer & backbuffer();
        maxim::stream & stream ();

        void set_fullscreen(bool enable);
        void set_cursor_enabled (bool enable);

        void reset_attributes ();
        void clear_screen();
        void begin_present ();
        void end_present ();

        void dispatch_input_events (input_event_handler & handler) const;

    private:

        std::unique_ptr < input::driver >
            _input_driver = input::driver::create ();

        std::unique_ptr < output::driver >
            _output_driver = output::driver::create ();

        size       _size           { _output_driver->get_size() };

        character_buffer
                    _back_buffer    { _size },
                    _front_buffer   { _size };

        maxim::stream
        			_stream;
        bool        _alternate_buffer_enabled {false};
        bool        _cursor_enabled { true };

    };
}

#endif
