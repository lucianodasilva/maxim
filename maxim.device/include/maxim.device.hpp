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
        virtual void on_key_up (input::key_info const & key) = 0;
        virtual void on_key_down (input::key_info const & key) = 0;
    };

    struct device {
    public:

        ~device();

        [[nodiscard]]
        size const & get_size () const;

        character_buffer & backbuffer();

        void set_fullscreen(bool enable);
        void set_cursor (bool enable);

        void clear();
        void present ();

        void dispatch_input_events (input_event_handler & handler) const;

        template < typename _on_key_down_t, typename _on_key_up_t >
        void dispatch_input_events (_on_key_down_t & key_down_handler, _on_key_up_t & key_up_handler = [](input::key_info const &){}) const {

            class : input_event_handler {
            public:
                _on_key_down_t & _key_down_handler;
                _on_key_up_t & _key_up_handler;

                void on_key_up (input::key_info const & key) override {
                    _key_up_handler (key);
                }

                void on_key_down (input::key_info const & key) override {
                    _key_down_handler (key);
                }

            } handler { key_down_handler, key_up_handler };

            dispatch_input_events(handler);
        }

    private:

        std::unique_ptr < input::driver >
            _input_driver = input::driver::create ();

        std::unique_ptr < output::driver >
            _output_driver = output::driver::create ();

        size       _size           { _output_driver->get_size() };

        character_buffer
                    _back_buffer    { _size },
                    _front_buffer   { _size };

        stream		_stream;
        bool        _alternate_buffer_enabled {false};
        bool        _cursor_enabled { true };

    };
}

#endif
