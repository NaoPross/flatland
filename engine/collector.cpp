#include "collector.hpp"

using namespace std;
using namespace flat;

FlatCollector::FlatCollector(FlatCollector *parent)

    : parent(parent), released(!parent)
{

}

FlatCollector::~FlatCollector()
{
    if (parent != 0)
        parent->detach(this);

    parent = 0;

    for (FlatCollector * child : children)
    {
        if (!child->isReleased())
            delete child;
    }
}

bool FlatCollector::isReleased() const
{
    return released;
}

void FlatCollector::release()
{
    released = true;
}
    
void FlatCollector::attach(FlatCollector *obj)
{
    if (obj == 0)
        return;

    children.insert(obj);
    obj->setParent(this); 
}

void FlatCollector::detach(FlatCollector *obj)
{
    children.erase(obj);
    obj->releaseParent();
}

void FlatCollector::setParent(FlatCollector *obj)
{
    parent = obj;
    released = false;
}

void FlatCollector::releaseParent()
{
    parent = 0;
    released = true;
}

FlatCollector * FlatCollector::getParent()
{
    return parent;
}

bool FlatCollector::isParentOf(FlatCollector* obj) const
{
    return this == obj->parent;
}

set<FlatCollector*>::iterator FlatCollector::begin()
{
    return children.begin();
}

set<FlatCollector*>::iterator FlatCollector::end()
{
    return children.end();
}

set<FlatCollector*>::const_iterator FlatCollector::begin() const
{
    return children.begin();
}

set<FlatCollector*>::const_iterator FlatCollector::end() const
{
    return children.end();
}

