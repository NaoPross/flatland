#ifndef __FLAT_COMPONENT_H__
#define __FLAT_COMPONENT_H__

#include "object.hpp"
#include <string>

class Component : virtual public flat::core::object
{
    
    Component * parent;

public:

    /* Zero means attach to main window layer */
    /* Send a 'created' signal in component reserved channel */
    Component(Component *parent = 0, const std::string& id = "");

    /* Send a 'deleted' signal in component reserved channel */ 
    virtual ~Component();

    void setParent(Component*);
    Component * getParent();

    virtual void render() = 0;
};

#endif
