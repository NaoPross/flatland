#include "component.hpp"
#include "signal.h"

using namespace flat;

component::component(component *parent, const std::string& id)
    : m_parent(parent)
{
    // TODO, check flatland initialization

    if (parent == 0)
    {
        // TODO set screen as parent layer
    }
}

component::~component()
{

}

void component::set_parent(component *parent)
{
    if (parent == 0) {
        // TODO set screen as parent layer
    }

    this->m_parent = m_parent;
}

component * component::parent()
{
    return m_parent;
}
    
