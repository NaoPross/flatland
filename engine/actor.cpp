#include "actor.hpp"

#include "flatland.hpp"

using namespace flat;

FlatActor::FlatActor(FlatCollector *parent, FlatBound *bounds)

    : FlatCollector(parent), bounds(bounds)
{
}

FlatActor::~FlatActor()
{
    
}

void FlatActor::setBounds(FlatBound * bounds)
{
    this->bounds = bounds;
}

FlatBound * FlatActor::getBounds() const
{
    return bounds;
}
