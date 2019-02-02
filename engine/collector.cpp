#include "collector.hpp"

child::child(std::shared_ptr<collector> _parent)
    : m_parent(_parent), m_released(_parent == nullptr)
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
    m_released = (obj == nullptr);
}

bool child::child_of(std::shared_ptr<collector> obj) const
{
    return this == (obj->m_parent).get();
}

bool child::released() const
{
    return m_released;
} 

void child::release()
{
    m_released = true;
}

collector::~collector()
{
    // Should auto-erase all children
    for (auto c : *this)
    {
        std::shared_ptr<child> pt;

        if (pt = c.lock() && !pt.unique() && !pt->released() && pt->child_of(this))
            delete pt.get(); // be careful
    }
}

void collector::attach(std::shared_ptr<child> obj)
{
    if (obj == nullptr)
        return;

    insert(obj);
    obj->set_parent(this); 
}

void collector::detach(std::shared_ptr<child> obj)
{
    if (!reading)
        remove(obj);

    obj->set_parent(0);
}

collector::iterator collector::begin()
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
}
