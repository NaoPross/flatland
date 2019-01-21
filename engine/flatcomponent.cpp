#include "flatcomponent.h"
#include "flatsignal.h"

Component::Component(Component *parent, const std::string& id)
    : parent(parent)
{
    // TODO, check flatland initialization

    if (id.empty())
        setID(FlatObject::randomID());

    if (parent == 0)
    {
        // TODO set screen as parent layer
    }
}

Component~Component()
{

}

void Component::setParent(Component *parent)
{
    if (parent == 0)
        // TODO set screen as parent layer
        ;

    this->parent = parent;
}

Component * getParent()
{
    return parent;
}
    
