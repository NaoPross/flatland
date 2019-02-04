#include "renderbase.hpp"
#include "core/signal.hpp"
#include "flatland.hpp"

using namespace flat;

renderbase::renderbase(const std::string& id) : core::labelled(id)
{
    // TODO, signal creation
    core::signal<std::shared_ptr<renderbase>, bool> sig(shared_from_this(), true);
    flat::core_channel()->emit(sig);
}

renderbase::~renderbase()
{
    // TODO, signal destruction
    core::signal<std::shared_ptr<renderbase>, bool> sig(shared_from_this(), false);
    flat::core_channel()->emit(sig);
}

