#include "core/labelled.hpp"

#include <stdlib.h>

using namespace std;
using namespace flat::core;

labelled::labelled(const std::string& _label, bool allow_null) 
    : label((!allow_null && _label.empty()) ? random_label() : _label) {}

string labelled::random_label(uint8_t length) {
    
    string out;

    for (uint8_t i = 0; i < length; ++i)
        out += (char)(rand() % 93 + 33);

    return out;
}

