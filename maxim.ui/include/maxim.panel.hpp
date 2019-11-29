#pragma once
#ifndef _maxim_panel_hpp_
#define _maxim_panel_hpp_

#include <maxim.element.hpp>
#include <memory>

namespace maxim {



    class panel : public element {
    public:

        void update_layout(maxim::point const &position, maxim::size const & size) override;

    protected:

        std::vector < std::unique_ptr < element > > elements;

    };

}

#endif // _maxim_panel_hpp_
