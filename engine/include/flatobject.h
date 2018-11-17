#ifndef __FLATOBJECT_H__
#define __FLATOBJECT_H__

#include <list>
#include <vector>
#include <initializer_list>

#include "types.h"

class FlatObject
{
    char id[32];

    /* Common list of objects */
    static std::list<FlatObject*> allObjects;

public:
    
    FlatObject();
    ~FlatObject();

    void setID(const char*);

    const char* getID() const;

    /* Static accessors to allObject */

    static bool isAllocated(FlatObject*);
   
    static std::vector<FlatObject*>& getByID(const char *id, std::vector<FlatObject*>&);
};

#endif
