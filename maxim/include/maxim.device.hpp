#pragma once
#ifndef _maxim_device_hpp_
#define _maxim_device_hpp_

#include <memory>
#include <vector>

#include "maxim.details.hpp"
#include "maxim.character.hpp"
#include "maxim.input.driver.hpp"
#include "maxim.output.driver.hpp"
#include "maxim.graphics.hpp"
#include "maxim.stream.hpp"

namespace maxim {

    struct device {
    public:

        size const & get_size () const;

        character_buffer & backbuffer();
        maxim::graphics graphics();

        void fullscreen(bool value);

        void enable_cursor (bool value);

        void clear();
        void present ();

        std::vector<input::key_event_args> get_input_events() const;

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

    };
}

#endif
