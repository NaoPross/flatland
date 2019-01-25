#ifndef __FLATLAYER_H__
#define __FLATLAYER_H__

#include "collector.hpp"

namespace flat {
    class FlatLayer : public FlatCollector
    {
        
    public:

        FlatLayer(FlatLayer *parent); 
        ~FlatLayer();
    };
}
#endif
