#pragma once
#ifndef _maxim_label_hpp_
#define _maxim_label_hpp_

#include "maxim.element.hpp"

namespace maxim {

    class label : public element {
    public:

        property < maxim::color_t > 		foreground;
        property < maxim::text_alignment >	text_alignment;
        property < std::string >    		text;

        void paint (graphics & g, rect const & bounds) override;

    };

}

#endif