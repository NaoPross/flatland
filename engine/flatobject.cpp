#include "flatobject.h"

#include <stdlib.h>

using namespace std;

list<FlatObject*> FlatObject::allObjects;

FlatObject::FlatObject()
{
    /* Collect this object */
    FlatObject::allObjects.push_back(this);
}

FlatObject::~FlatObject()
{
    /* Eliminate this object reference */
    FlatObject::allObjects.remove(this);
}

void FlatObject::setID(const string& id)
{
    this->id = id;
}

const string& FlatObject::getID() const
{
    return id;
}

string FlatObject::randomID(Uint8 length) {
    
    string out;

    for (Uint8 i = 0; i < length; ++i)
        out += (char)(rand() % 93 + 33);

    return out;
}

bool FlatObject::isAllocated(FlatObject *obj)
{
    for (FlatObject * o : FlatObject::allObjects)
    {
        if (o == obj)
            return true;
    }

    return false;
}

vector<FlatObject*>& FlatObject::getByID(const string& id, vector<FlatObject*>& l)
{
    for (FlatObject * obj : FlatObject::allObjects)
    {
        if (id == obj->getID())
            l.push_back(obj);
    }

    return l;
}
