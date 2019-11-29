#pragma once
#ifndef _maxim_control_hpp_
#define _maxim_control_hpp_

#include <maxim.property.hpp>
#include <maxim.element.hpp>
#include <maxim.layout.hpp>

namespace maxim {

    class control : public element {
    public:

        property < docking_type >   docking;
        property < maxim::size >    size;

        void update_layout(maxim::point const &position, maxim::size const & size) override;

    private:

        maxim::point    _layout_position {};
        maxim::size     _layout_size {};

    };

}

#endif