#ifndef __FLATCOLLECTOR_H__
#define __FLATCOLLECTOR_H__

#include <list>
#include <memory>

namespace flat::core {

class collector;

class child
{
    std::weak_ptr<collector> m_parent;

    bool m_released;

public:

    child(std::shared_ptr<collector> _parent = 0);

    virtual ~child();

    std::shared_ptr<collector> parent();

    void set_parent(std::shared_ptr<collector> obj);

    bool child_of(std::shared_ptr<collector> obj) const;

    bool released() const;
    
    /*
     * Take ownership of the object
     */
    void release();
}

class collector : virtual public child, private std::list<std::weak_ptr<child>>
{
public: 

    using child::child(std::shared_ptr<collector>);

    ~collector();
        
    void attach(std::shared_ptr<child> obj);

    void detach(std::shared_ptr<child> obj);

    iterator begin();

    iterator end();
    
    const_iterator begin() const;
    
    const_iterator end() const;
};

}

#endif
