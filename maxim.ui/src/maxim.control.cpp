#include "maxim.control.hpp"

namespace maxim {

    void control::update_layout(maxim::point const &position, maxim::size const & size) {

        auto
            left = (
                (docking.get() & maxim::docking_type::right) == maxim::docking_type::right ?
                    position.x + this->size.get().width - size.width :
                    position.x
            ),
            right = (
                (docking.get() & maxim::docking_type::left) == maxim::docking_type::left ?
                    position.x + size.width :
                    position.x + this->size.get().width
            ),
            top = (
                (docking.get() & maxim::docking_type::bottom) == maxim::docking_type::bottom ?
                position.y + this->size.get().height - size.height :
                position.y
            ),
            bottom = (
                (docking.get() & maxim::docking_type::top) == maxim::docking_type::top ?
                position.y + size.height :
                position.y + this->size.get().height
        );

        _layout_position = {left, top};
        _layout_size = { right - left, bottom - top };

    }


}