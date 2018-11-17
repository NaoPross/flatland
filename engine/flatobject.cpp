#include "flatobject.h"

#include <string.h>

using namespace std;

FlatObject::FlatObject()
{
    /* Collect this object */
    allObjects.push_back(this);
}

FlatObject::~FlatObject()
{
    /* Eliminate this object reference */
    allObjects.remove(this);
}

void FlatObject::setID(const char *id)
{
    strncpy(this->id, id, 31);
    this->id[31] = '\0';
}

const char* FlatObject::getID() const
{
    return &id[0];
}

bool FlatObject::isAllocated(FlatObject *obj)
{
    for (FlatObject * o : allObjects)
    {
        if (o == obj)
            return true;
    }

    return false;
}

vector<FlatObject*>& FlatObject::getByID(const char *id, vector<FlatObject*>& l)
{
    for (FlatObject * obj : allObjects)
    {
        if (!strcmp(id, obj->getID()))
            l.push_back(obj);
    }

    return l;
}
