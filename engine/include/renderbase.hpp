#ifndef __FLAT_RENDER_BASE_H__
#define __FLAT_RENDER_BASE_H__

#include "core/labelled.hpp"
#include <memory>
#include <string>

namespace flat {

struct renderbase : virtual core::labelled
{
    /*
     * Very hacky way to send a c pointer
     * into a signal
     *
     * Flatland should catch the
     * signals alloc_pck and free_pck in order to 
     * map or unmap a renderbase object
     */
        
    // needed to send alloc signal
    struct map_pck
    {
        map_pck(renderbase * _ptr) : sender(_ptr) {}

        renderbase * sender;
    }

    // needed to send a free signal
    // notice: after destruction the pointer won't be valid anymore
    struct unmap_pck
    {
        unmap_pck(renderbase * _ptr) : uuid(_ptr->hash) {}
        
        std::size_t uuid;
    }

    /* Send a 'created' signal in component reserved channel */
    renderbase(const std::string& id = "");

    /* Send a 'deleted' signal in component reserved channel */ 
    virtual ~renderbase();

    virtual void render() = 0;
};

}

#endif
