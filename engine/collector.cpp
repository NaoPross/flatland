#include "collector.hpp"

/*child::child(std::shared_ptr<collector> _parent)
    : m_parent(_parent)
{
}

child::~child()
{
}

std::shared_ptr<collector> child::parent()
{
    return m_parent.lock();
}

void child::set_parent(std::shared_ptr<collector> obj)
{
    m_parent = obj;
}

bool child::child_of(std::shared_ptr<collector> obj) const
{
    return this == obj->parent().lock().get();
}

collector::~collector()
{
    // Should auto-erase all children
}

void collector::attach(std::shared_ptr<child> obj)
{
    if (obj != nullptr)
    {
        insert(obj);
        obj->set_parent(this); 
    }
}

void collector::detach(std::shared_ptr<child> obj)
{
    if (!reading)
        remove(obj);

    obj->set_parent(nullptr);
}*/

/*collector::iterator collector::begin()
{
    return begin();
}

collector::iterator collector::end()
{
    return end();
}

collector::const_iterator collector::begin() const
{
    return begin();
}

collector::const_iterator collector::end() const
{
    return end();
}*/

