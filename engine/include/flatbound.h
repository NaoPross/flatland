#ifndef __FLATBOUND_H__
#define __FLATBOUND_H__

#include "flatobject.h"
#include "svector.h"

typedef SVector<int, 2> pixel;

class FlatBound : virtual public FlatObject
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

#endif
