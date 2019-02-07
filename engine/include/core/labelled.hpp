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

class flat::core::labelled
{
    static std::size_t count;

public:

    const std::string label;
    const std::size_t uuid;

    labelled(const std::string& label = "");

    //static std::size_t random_hash();
};

