#include "renderbase.hpp"
#include "core/signal.hpp"
#include "flatland.hpp"

using namespace flat;

renderbase::renderbase(const std::string& id) : core::labelled(id)
{
    // signal creation
    flat::core_channel()->emit(map_pck(this));
}

renderbase::~renderbase()
{
    // signal destruction
    flat::core_channel()->emit(unmap_pck(this));
}

