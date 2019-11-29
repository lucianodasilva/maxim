#pragma once
#ifndef _maxim_element_hpp_
#define _maxim_element_hpp_

#include <maxim.io.hpp>
#include <maxim.property.hpp>

namespace maxim {

    class element {
    public:

        virtual void update_layout (maxim::point const & position, maxim::size const & size);

    };

}

#endif