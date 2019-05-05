#pragma once

#ifndef __FLATSTREAMER_HPP__
#define __FLATSTREAMER_HPP__

#include "renderbase.hpp"

class streamer : virtual public renderbase
{
    virtual void render() override;
};

#endif
