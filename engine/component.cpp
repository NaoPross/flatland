#include "component.hpp"
#include "signal.h"

using namespace flat;

component::component(component *parent, const std::string& id)
    : m_parent(parent)
{
    // TODO, check flatland initialization

    if (parent == nullptr) {
        // TODO set screen as parent layer
    }
}

component::~component()
{

}

void component::set_parent(component *parent)
{
    if (parent == nullptr) {
        // TODO set screen as parent layer
    }

    m_parent = parent;
}

component * component::parent()
{
    return m_parent;
}
    
