#ifndef __FLATOBJECT_H__
#define __FLATOBJECT_H__

#include <list>
#include <vector>
#include <string>
#include <initializer_list>

#include "types.h"

class FlatObject
{
    std::string id;

    /* Common list of objects */
    static std::list<FlatObject*> allObjects;

public:
    
    FlatObject();
    ~FlatObject();

    void setID(const std::string&);

    const std::string& getID() const;

    /* Static accessors to allObject */

    static bool isAllocated(FlatObject*);
   
    static std::vector<FlatObject*>& getByID(const std::string& id, std::vector<FlatObject*>&);

    static std::string randomID(Uint8 length = 8);
};

#endif
