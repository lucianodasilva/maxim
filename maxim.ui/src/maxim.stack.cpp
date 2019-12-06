#include "maxim.stack.hpp"

namespace maxim {
/*
    void vertical_stack::update_layout(maxim::point const &position, maxim::size const & size) {

        if ((this->docking.get() & docking_type::fill) == docking_type::fill) {

            float dim =
                    size.height -
                    margins.get().height * 2 -
                    (padding.get().height * (panel::elements.size() - 1));

            float control_dim = dim / panel::elements.size();
            auto control_size = maxim::size { size.width, static_cast < int32_t > (control_dim) };

            for (int i = 0; i < panel::elements.size(); ++i) {

                auto pos = point {
                    position.x,
                    static_cast < int32_t > (
                            margins.get().height +
                            control_dim * i +
                            padding.get().height * i)
                };

                panel::elements [i]->update_layout(pos, control_size);
            }
        } else if ((this->docking.get() & docking_type::left) == docking_type::left) {
            
        }
    }
*/
}