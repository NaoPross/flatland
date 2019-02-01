#include "collector.hpp"

using namespace std;
using namespace flat;

FlatCollector::FlatCollector(FlatCollector *parent)

    : m_parent(parent), m_released(!parent)
{

}

FlatCollector::~FlatCollector()
{
    if (m_parent != 0)
        m_parent->detach(this);

    m_parent = 0;

    for (FlatCollector * child : m_children)
    {
        if (!child->isReleased())
            delete child;
    }
}

bool FlatCollector::isReleased() const
{
    return m_released;
}

void FlatCollector::release()
{
    m_released = true;
}
    
void FlatCollector::attach(FlatCollector *obj)
{
    if (obj == 0)
        return;

    m_children.insert(obj);
    obj->setParent(this); 
}

void FlatCollector::detach(FlatCollector *obj)
{
    m_children.erase(obj);
    obj->releaseParent();
}

void FlatCollector::setParent(FlatCollector *obj)
{
    m_parent = obj;
    m_released = false;
}

void FlatCollector::releaseParent()
{
    m_parent = 0;
    m_released = true;
}

FlatCollector * FlatCollector::getParent()
{
    return m_parent;
}

bool FlatCollector::isParentOf(FlatCollector* obj) const
{
    return this == obj->m_parent;
}

set<FlatCollector*>::iterator FlatCollector::begin()
{
    return m_children.begin();
}

set<FlatCollector*>::iterator FlatCollector::end()
{
    return m_children.end();
}

set<FlatCollector*>::const_iterator FlatCollector::begin() const
{
    return m_children.begin();
}

set<FlatCollector*>::const_iterator FlatCollector::end() const
{
    return m_children.end();
}

