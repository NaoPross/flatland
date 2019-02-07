#ifndef __FLAT_RENDER_BASE_H__
#define __FLAT_RENDER_BASE_H__

#include "core/labelled.hpp"
#include <memory>
#include <string>
#include "core/collector.hpp"

namespace flat {

class renderbase;

namespace helper
{
    // functor in order to place the object in the right order:
    // major overlapping priority objects render by last,
    // then they visualize over the others
    struct overlap
    {
        bool operator()(const renderbase& lhs, const renderbase& rhs);
    };

}

class renderbase : virtual public core::labelled, public core::child<renderbase, helper::overlap>
{
    uint32_t m_overlap;    

public:

    /*
     * A way to send a c pointer
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
    };

    // needed to send a free signal
    // notice: after destruction the pointer won't be valid anymore
    struct unmap_pck
    {
        unmap_pck(renderbase * _ptr) : uuid(_ptr->uuid) {}
        
        std::size_t uuid;
    };
    
    // Accessors to collectors types
    using collector = typename core::collector<renderbase, helper::overlap>;

    // Send a 'created' signal in component reserved channel
    renderbase(uint32_t overlap = 0, const std::string& id = "");

    // Send a 'deleted' signal in component reserved channel
    virtual ~renderbase();

    virtual void render() = 0;

    void set_overlap(uint32_t z);

    uint32_t overlap() const;
};

}

#endif
