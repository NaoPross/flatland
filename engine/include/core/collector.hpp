#ifndef __FLATCOLLECTOR_H__
#define __FLATCOLLECTOR_H__

#include <list>
#include <memory>

#include "debug.hpp"

namespace flat::core {

template <class T> class collector;

template <class T>
class child
{
    collector<T> * m_parent;

    // only collector should access this method
    void set_parent(collector<T> * obj)
    {
        npdebug("Setting parent ", obj)
        m_parent = obj;
    }

public:

    // using set_parent
    friend class collector<T>;

    child() : m_parent(nullptr) {}

    virtual ~child() {}

    collector<T> * parent()
    {
        return m_parent;
    }
    
    bool child_of(collector<T> * obj) const
    {
        return this == obj->parent();
    }
};

/*
 * This class is made in order
 * to take ownership of the objects
 *
 * Insertion is made by moving objects
 */

template <class T>
struct collector : private std::list<std::unique_ptr<child<T>>>
{

    // automaticly destructs all children
    ~collector() {}

    /*
     * Same as attach(child<T>&& obj)
     * but it copies the entering value
     * Otherwise it is turned into an rvalue
     */
    void attach(T* obj)
    {
        npdebug("Attaching existing object to ", this)

        if (obj->parent() == nullptr) {
            // take ownership of the passed pointer
            auto it = this->emplace(end(), std::unique_ptr<child<T>>(obj));
            (*it).get()->set_parent(this);

        } else {
            npdebug("Object has already a parent")
        }
    }
    
    /*
     * Returns the pointer association
     * to the entering object
     * Do never delete it, collector
     * takes its ownership
     */ 
    template <class S, class ...Args>
    S* attach(Args&&... args)
    {
        npdebug("Attaching new object to ", this)

        S * out;

        // call move push_back
        auto it = this->emplace(end(), std::unique_ptr<child<T>>(out = new S(args...)));
        (*it).get()->set_parent(this);

        return out;
    }

    /*
     * Detaches object and 
     * invalidates the pointer!
     */
    void detach(child<T>* obj)
    {
        this->remove_if([&](const auto& p) {
            return p.get() == obj; 
        });
    }

    using std::list<std::unique_ptr<child<T>>>::clear;

    using std::list<std::unique_ptr<child<T>>>::begin;
    using std::list<std::unique_ptr<child<T>>>::end;

    using std::list<std::unique_ptr<child<T>>>::size;

    /*iterator begin();

    iterator end();
    
    const_iterator begin() const;
    
    const_iterator end() const;*/
};

}

#endif
