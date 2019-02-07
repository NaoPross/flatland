#include "renderbase.hpp"
#include "core/signal.hpp"
#include "flatland.hpp"

using namespace flat;

renderbase::renderbase(uint32_t overlap_, const std::string& id) 
    : core::labelled(id), m_overlap(overlap_)
{
    // signal creation
    flat::core_channel()->emit(map_pck(this));
}

renderbase::~renderbase()
{
    // signal destruction
    flat::core_channel()->emit(unmap_pck(this));
}


void renderbase::set_overlap(uint32_t z)
{
    m_overlap = z;
}

uint32_t renderbase::overlap() const
{
    return m_overlap;
}
