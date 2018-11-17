#ifndef __FLATCOLLECTOR_H__
#define __FLATCOLLECTOR_H__

#include "flatobject.h"
#include <set>

class FlatCollector : virtual public FlatObject
{
    FlatCollector * parent;

    bool released;

    std::set<FlatCollector*> children;

public:

    FlatCollector(FlatCollector *parent = 0);

    virtual ~FlatCollector();

    /* Ownership functions */

    bool isReleased() const;

    /* Release object from automatic ownership destruction */
    void release();
    
    void attach(FlatCollector*);
    void detach(FlatCollector*);

    void setParent(FlatCollector*);
    void releaseParent();

    FlatCollector * getParent();

    bool isParentOf(FlatCollector*) const;

    std::set<FlatCollector*>::iterator begin();
    std::set<FlatCollector*>::iterator end();

    std::set<FlatCollector*>::const_iterator begin() const;
    std::set<FlatCollector*>::const_iterator end() const;
};

#endif
