#pragma once

#include "flatland/core/tools.hpp"

namespace flat::overlap {

    /*
    * Overlap event structure:
    * Contains the involved bounded objects
    * This signal is generally sent by a collision detector (colldect.hpp).
    * Catch it if you want to check for collisions
    * Intercept it if you want to extend the information sent
    */
    template<bool Begin>
    struct event : public flat::core::weak_pair<flat::bound::bounded, flat::bound::bounded> {

        using flat::core::weak_pair::weak_pair;        

        static constexpr bool is_begin = Begin;
    };

    // begin overlap signal
    using begin = event<true>;

    // end overlap signal
    using end = event<false>;
};
