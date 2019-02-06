#pragma once

#include <string>

#include "types.hpp"

namespace flat
{
    namespace core
    {
        class labelled;
    }
}

struct flat::core::labelled
{
    const std::string label;
    const std::size_t hash;

    labelled(const std::string& label = "");

    static std::size_t random_hash();
};

