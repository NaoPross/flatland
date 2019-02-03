#ifndef __FLATCOLLECTOR_H__
#define __FLATCOLLECTOR_H__

#include <list>
#include <memory>

namespace flat::core {

template <class T> class collector;

template <class T>
class child
{
    std::weak_ptr<collector<T>> m_parent;

public:

    child(std::shared_ptr<collector<T>> _parent = nullptr)
        : m_parent(_parent)
    {
    }

    virtual ~child() {}

    std::shared_ptr<collector<T>> parent()
    {
        return m_parent.lock();
    }

    void set_parent(std::shared_ptr<collector<T>> obj)
    {
        m_parent = obj;
    }

    bool child_of(std::shared_ptr<collector<T>> obj) const
    {
        return this == obj->parent().lock().get();
    }
};

template <class T>
struct collector : private std::list<std::shared_ptr<child<T>>>
{
    ~collector() {}
        
    void attach(std::shared_ptr<child<T>> obj)
    {
        if (obj != nullptr)
        {
            insert(obj);
            obj->set_parent(this); 
        }
    }

    void detach(std::shared_ptr<child<T>> obj)
    {
        remove(obj);
        obj->set_parent(nullptr);
    }

    using std::list<std::shared_ptr<child<T>>>::clear;

    using std::list<std::shared_ptr<child<T>>>::begin;
    using std::list<std::shared_ptr<child<T>>>::end;

    /*iterator begin();

    iterator end();
    
    const_iterator begin() const;
    
    const_iterator end() const;*/
};

}

#endif
