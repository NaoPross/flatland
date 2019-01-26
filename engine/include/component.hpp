#ifndef __FLAT_COMPONENT_H__
#define __FLAT_COMPONENT_H__

#include "object.hpp"
#include "core/labelled.hpp"
#include <string>

namespace flat {

class component : virtual public object, virtual public core::labelled
{
    
    component * m_parent;

public:

    /* Zero means attach to main window layer */
    /* Send a 'created' signal in component reserved channel */
    component(component *parent = 0, const std::string& id = "");

    /* Send a 'deleted' signal in component reserved channel */ 
    virtual ~component();

    void set_parent(component*);
    component * parent();

    virtual void render() = 0;
};

}

#endif
