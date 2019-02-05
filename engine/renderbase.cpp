#include "renderbase.hpp"
#include "core/signal.hpp"
#include "flatland.hpp"

using namespace flat;

renderbase::renderbase(const std::string& id) : core::labelled(id)
{
    // TODO, signal creation
    //core::signal sig(shared_from_this(), true);
    flat::core_channel()->emit(shared_from_this(), true);
}

renderbase::~renderbase()
{
    // TODO, signal destruction
    //core::signal sig(shared_from_this(), false);
    flat::core_channel()->emit(shared_from_this(), false);
}

