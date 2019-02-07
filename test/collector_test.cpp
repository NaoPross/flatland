#include "core/collector.hpp"
#include "debug.hpp"

#include <iostream>
#include <memory>

using namespace flat::core;

struct custom_type;

struct overlap_functor
{
    bool operator()(const custom_type& rhs, const custom_type& lhs);
};

struct custom_type : public child<custom_type, overlap_functor>
{
    bool v;
    int overlap;
    const char f;

    custom_type(char _f, int ov = 0, bool _v = true) : f(_f), overlap(ov), v(_v)  
    {

        if (v) {
            npdebug("instanciated custom_type", "   char: ", f)
        }
    }

    custom_type(const custom_type& other) 
        : f(other.f), overlap(other.overlap), v(other.v) {

        if (v) {
            npdebug("copy constructed custom_type (bad)", "   char: ", f)
        }
    }

    custom_type(custom_type&& other) 
        : f(other.f), overlap(other.overlap), v(other.v) {

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

bool overlap_functor::operator()(const custom_type& rhs, const custom_type& lhs)
{
    return lhs.overlap > rhs.overlap;
}

struct custom_collector : public collector<custom_type, overlap_functor>
{
    custom_collector() = default;
};

int main() {
    
    custom_collector coll;

    // test a: construct pointer and give ownership
    custom_type * a;
    
    coll.attach(a = new custom_type('a', 0));

    npdebug(&coll, " size: ", coll.size())

    custom_type * b;

    { 
        // constructor attachment
        b = coll.attach<custom_type>('b', 3);
    }

    // test c: multiple try to attach
    npdebug(&coll, " size: ", coll.size())
    npdebug("")
     
    {
        custom_type * c = new custom_type('c', 2);

        coll.attach(c);
        npdebug(&coll, " size: ", coll.size())

        // should not attach it
        coll.attach(c);
        npdebug(&coll, " size: ", coll.size())
    }

    npdebug(&coll, " size: ", coll.size())

    npdebug("")
    npdebug("Printing all characters")

    // order should be a, c, b
    for (const auto& obj : coll) {
        npdebug("Char printed: ", obj.f)
    }

    return 0;
}

