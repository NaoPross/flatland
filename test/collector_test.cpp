#include "core/collector.hpp"
#include "debug.hpp"

#include <iostream>
#include <memory>

using namespace flat::core;

struct custom_type : public child<custom_type>
{
    const char f;
    bool v;

    custom_type(char _f, bool _v = true) : f(_f), v(_v) {

        if (v) {
            npdebug("instanciated custom_type", "   char: ", f)
        }
    }

    custom_type(const custom_type& other) : f(other.f) {

        if (v) {
            npdebug("copy constructed custom_type (bad)", "   char: ", f)
        }
    }

    custom_type(custom_type&& other) : f(other.f) {

        if (v) {
            npdebug("move constructed custom_type (efficient)", "   char: ", f)
        }
    }

    ~custom_type() {

        if (v) {
            npdebug("deleted custom_type instance", "   char: ", f)
        }
    }
};

struct custom_collector : public collector<custom_type>
{
    custom_collector() = default;
};

int main() {
    
    custom_collector coll;

    // test a: construct pointer and give ownership
    custom_type * a;
    
    coll.attach(a = new custom_type('a'));

    npdebug(&coll, " size: ", coll.size())
    npdebug("Char printed: ", a->f)
    npdebug("")

    custom_type * b;

    { 
        // constructor attachment
        b = coll.attach<custom_type>('b');
    }

    npdebug("Calling ptr ", b)
    npdebug("Char printed: ", b->f)

    // test c: multiple try to attach
    npdebug(&coll, " size: ", coll.size())
    npdebug("")
     
    {
        custom_type * c = new custom_type('d');

        coll.attach(c);
        npdebug(&coll, " size: ", coll.size())

        // should not attach it
        coll.attach(c);
        npdebug(&coll, " size: ", coll.size())
    }

    npdebug(&coll, " size: ", coll.size())

    return 0;
}

