#ifndef __FLATBOUND_H__
#define __FLATBOUND_H__

#include "object.hpp"
#include "svector.h"

namespace flat {

typedef SVector<int, 2> pixel;

class FlatBound : virtual public flat::object
{
    pixel location;

public:

    FlatBound() {}
    virtual ~FlatBound() {}

    virtual void scale(double ratio) = 0;

    virtual pixel extremum(const FlatBound*) const = 0;

    bool isColliding(const FlatBound *other) const
    {
        return (other.extremum(this) - location) < (this.extremum(other) - location);
    }

    const pixel& getLocation() const
    {
        return location;
    }

    void setLocation(const pixel &loc)
    {
        this->location = loc;
    }
};

}

#endif
