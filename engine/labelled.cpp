#include "core/labelled.hpp"

#include <stdlib.h>

using namespace std;
using namespace flat::core;

labelled::labelled(const std::string& label, bool allow_null) 
    : label((!allow_null && label.empty()) ? random_label() : label) {}

string labelled::random_label(uint8_t length) {
    
    string out;

    for (uint8_t i = 0; i < length; ++i)
        out += (char)(rand() % 93 + 33);

    return out;
}

