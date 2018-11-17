#ifndef __FLATLAYER_H__
#define __FLATLAYER_H__

#include "flatcollector.h"

class FlatLayer : public FlatCollector
{
    
public:

    FlatLayer(FlatLayer *parent); 
    ~FlatLayer();
};

#endif
