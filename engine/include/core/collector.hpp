#ifndef __FLATCOLLECTOR_H__
#define __FLATCOLLECTOR_H__

#include <set>
#include <memory>

#include "debug.hpp"

namespace flat::core {

template <class T, class Order> class collector;

template <class T, class Order>
class child
{
    collector<T, Order> * m_parent;

    // only collector should access this method
    void set_parent(collector<T, Order> * obj)
    {
        npdebug("Setting parent ", obj)
        m_parent = obj;
    }

public:

    // using set_parent
    friend class collector<T, Order>;

    child() : m_parent(nullptr) {}

    // adapt constructor on T
    //template <class ...Args>
    //explicit child(Args&&... args) : T(std::forward<Args>(args)...), m_parent(nullptr) {}
    //
    //using T::T;
    
    //child(const T& t) : T(t), m_parent(nullptr) {}

    virtual ~child() {}

    collector<T, Order> * parent()
    {
        return m_parent;
    }
    
    bool child_of(collector<T, Order> * obj) const
    {
        return m_parent == obj;
    }
};

/*
 * This class is made in order
 * to take ownership of the objects
 *
 * Insertion is made by moving objects
 */

template <class T, class Order>
class collector
{
    // define a new and more confortable functor 
    struct order_t
    {
        // functor that converts unique_ptr into const references
        // default constructed
        Order converter;

        bool operator()(const std::unique_ptr<child<T, Order>>& lhs,
                         const std::unique_ptr<child<T, Order>>& rhs)
        {
            return converter(*static_cast<const T*>(lhs.get()), *static_cast<const T*>(rhs.get())); 
        }
    };

    // the collection of objects
    std::multiset<std::unique_ptr<child<T, Order>>, order_t> collection;

    using base_t = typename std::multiset<std::unique_ptr<child<T, Order>>, order_t>;

public:

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
            auto it = collection.insert(std::unique_ptr<child<T, Order>>(obj));
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
        auto it = collection.insert(std::unique_ptr<child<T, Order>>(out = new S(args...)));
        (*it).get()->set_parent(this);

        return out;
    }

    /*
     * Detaches object and 
     * invalidates the pointer!
     */
    void detach(child<T, Order>* obj)
    {
        auto it = collection.find_if([&](const auto& p) {
            return p.get() == obj; 
        });

        this->erase(it);
    }

    // multiset clear: clears the container freeing all objects
    void clear()
    {
        collection.clear();
    }

    // multiset size: returns the quantity of objects stored
    std::size_t size()
    {
        return collection.size();
    }

    /*
     * iterator and const_iterator definitions
     *
     * The constness of a unique_ptr should give
     * constness to the owned pointers
     */

    using set_iter = typename std::multiset<std::unique_ptr<child<T, Order>>, Order>::iterator;

    // iterator definition
    class iterator
    {
        typename collector<T, Order>::set_iter base;

    public:

        iterator(set_iter it) : base(it) {}

        T& operator*()
        {
            return *static_cast<T*>((*base).get());
        }

        T * operator->()
        {
            return static_cast<T*>((*base).get());
        }
        
        iterator operator++()
        {
            iterator i = *this;
            base++;            
            return i;
        }

        iterator operator++(int)
        {
            base++;
            return *this;
        }

        bool operator==(const iterator& rhs) const
        { 
            return base == rhs.base; 
        }

        bool operator!=(const iterator& rhs) const
        { 
            return base != rhs.base; 
        }
    };

    using const_set_iter = typename std::multiset<std::unique_ptr<child<T, Order>>, Order>::const_iterator;

    // const iterator definition
    class const_iterator
    {
        collector<T, Order>::const_set_iter base;

    public:

        const_iterator(const_set_iter it) : base(it) {}

        const T& operator*() const
        {
            return *static_cast<T*>((*base).get());
        }

        const T * operator->() const
        {
            return static_cast<T*>((*base).get());
        }
        
        iterator operator++()
        {
            iterator i = *this;
            base++;            
            return i;
        }

        iterator operator++(int)
        {
            base++;
            return *this;
        }

        bool operator==(const const_iterator& rhs) const
        { 
            return base == rhs.base; 
        }

        bool operator!=(const const_iterator& rhs) const
        { 
            return base != rhs.base; 
        }
    };

    /*
     * begin and end methods definition
     */

    iterator begin()
    {
        return iterator(collection.begin());
    }

    const_iterator begin() const
    {
        return const_iterator(collection.begin());
    }

    iterator end()
    {
        return iterator(collection.end());
    }
    
    const_iterator end() const
    {
        return const_iterator(collection.end());
    }
};

}

#endif
