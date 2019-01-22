#include "object.hpp"

#include <stdlib.h>

using namespace std;
using namespace flat::core;

list<object*> object::all_objects;

object::object()
{
    /* Collect this object */
    object::all_objects.push_back(this);
}

object::~object()
{
    /* Eliminate this object reference */
    object::all_objects.remove(this);
}

void object::set_id(const string& id)
{
    this->id = id;
}

const string& object::get_id() const
{
    return id;
}

string object::random_id(uint8_t length) {
    
    string out;

    for (Uint8 i = 0; i < length; ++i)
        out += (char)(rand() % 93 + 33);

    return out;
}

bool object::is_allocated(object *obj)
{
    for (object * o : object::all_objects)
    {
        if (o == obj)
            return true;
    }

    return false;
}

vector<object*>& object::get_by_id(const string& id, vector<object*>& l)
{
    for (object * obj : object::all_objects)
    {
        if (id == obj->getID())
            l.push_back(obj);
    }

    return l;
}
