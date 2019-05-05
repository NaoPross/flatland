#pragma once

#ifndef __RENDER_GROUP_HPP__
#define __RENDER_GROUP_HPP__

#include "renderbase.hpp"

namespace flat {

/*
 * A simple wrapper for renderbase class derivatives
 */
struct rendergroup : virtual public renderbase, public renderbase::collector
{
    using renderbase::renderbase;

    // render function
    virtual void render() override;
};

}

#endif
