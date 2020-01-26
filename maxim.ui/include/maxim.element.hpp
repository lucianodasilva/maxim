#pragma once
#ifndef _maxim_element_hpp_
#define _maxim_element_hpp_

#include <maxim.io.hpp>
#include <maxim.property.hpp>

namespace maxim {

    class paintable {
    public:
        virtual void paint (graphics & g, rect const & bounds) = 0;
    };

    class element : public virtual paintable {
    public:
        property < maxim::style_t > style;
        void paint (graphics & g, rect const & bounds) override;
    };

}

#endif