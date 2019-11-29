#pragma once
#ifndef _maxim_stack_hpp_
#define _maxim_stack_hpp_

#include "maxim.layout.hpp"
#include "maxim.panel.hpp"

namespace maxim {

    class stack : public panel {
    public:

        property < docking_type >
                            docking;
        property < size >   margins;
        property < size >   padding;

    protected:



    };

    class vertical_stack : public stack {
    public:

        void update_layout(maxim::point const &position, maxim::size const & size) override;

    };

}

#endif
