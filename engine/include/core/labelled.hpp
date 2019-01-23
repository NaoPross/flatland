#pragma once

#include <list>
#include <vector>
#include <string>
#include <initializer_list>

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

    labelled(const std::string& label = "");

    static std::string random_label(uint8_t length = 8);
};

