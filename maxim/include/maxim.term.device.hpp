#pragma once
#ifndef _maxim_term_device_hpp_
#define _maxim_term_device_hpp_

#include <memory>

#include "maxim.term.character.hpp"
#include "maxim.term.driver.hpp"
#include "maxim.term.graphics.hpp"
#include "maxim.term.stream.hpp"

namespace maxim {
    namespace term {

        struct device {
        public:

            size const & get_size () const;

			character_buffer & backbuffer();
			maxim::term::graphics graphics();

            void fullscreen(bool value);

            void enable_cursor (bool value);

			void clear();
            void present ();

			inline maxim::term::driver const & driver () const {
				return _driver;
			}

        private:

            maxim::term::driver
						_driver;
            size       _size           { _driver.get_size() };

            character_buffer
                        _back_buffer    { _size },
                        _front_buffer   { _size };

			stream		_stream;

        };

    }
}

#endif
