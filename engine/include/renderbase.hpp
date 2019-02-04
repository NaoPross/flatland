#ifndef __FLAT_RENDER_BASE_H__
#define __FLAT_RENDER_BASE_H__

#include "core/labelled.hpp"
#include <memory>
#include <string>

namespace flat {

struct renderbase : virtual core::labelled, virtual std::enable_shared_from_this<renderbase>
{
    /* Send a 'created' signal in component reserved channel */
    renderbase(const std::string& id = "");

    /* Send a 'deleted' signal in component reserved channel */ 
    virtual ~renderbase();

    virtual void render() = 0;
};

}

#endif
