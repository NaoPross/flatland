#include "layer.hpp"

using namespace flat;

/*FlatLayer::FlatLayer(FlatLayer *parent)
{

}

FlatLayer::~FlatLayer()
{

}*/

void layer::render()
{
    for (auto& child : *this)
        child.render();
}
