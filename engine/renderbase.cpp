#include "renderbase.hpp"
#include "core/signal.hpp"
#include "flatland.hpp"

using namespace flat;

renderbase::renderbase(uint32_t overlap_) 
    : m_overlap(overlap_), visible(true)
{
    // signal creation
    // flat::core_channel().emit(map_pck(this));
}

renderbase::~renderbase()
{
    // signal destruction
    // flat::core_channel().emit(unmap_pck(this));
}


void renderbase::set_overlap(uint32_t z)
{
    m_overlap = z;
}

uint32_t renderbase::overlap() const
{
    return m_overlap;
}

bool helper::overlap::operator()(const renderbase& lhs, const renderbase& rhs)
{
    return lhs.overlap() < rhs.overlap();
}

