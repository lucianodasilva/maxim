#pragma once
#ifndef _maxim_property_hpp_
#define _maxim_property_hpp_

#include "maxim.event.hpp"

namespace maxim {

    template < typename _t >
    struct property;

    template < typename _t >
    using property_changed_event = maxim::event < property < _t >, _t const & >;

    template < typename _t >
    struct property {
    public:

        property_changed_event < _t > on_property_changed;

        property() = default;
        inline property (property const & origin) :
            _value { origin._value }
        {};

        explicit inline property (_t const & v) :
                _value { v }
        {}

        property (property &&) noexcept = delete;

        inline property & operator = (_t const & value) {
            _value = value;

            on_property_changed.invoke (*this, _value);
            return *this;
        }

        inline _t get () const {
            return _value;
        }

        explicit inline operator _t () const {
            return get();
        }

    private:
        _t _value;
    };

}

#endif