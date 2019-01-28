#include "actor.hpp"

#include "flatland.hpp"

using namespace flat;

FlatActor::FlatActor(FlatCollector *parent, FlatBound *bounds)

    : FlatCollector(parent), m_bounds(bounds)
{
}

FlatActor::~FlatActor()
{
    
}

void FlatActor::setBounds(FlatBound * bounds)
{
    m_bounds = m_bounds;
}

FlatBound * FlatActor::getBounds() const
{
    return m_bounds;
}

