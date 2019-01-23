#include "core/labelled.hpp"

#include <stdlib.h>

using namespace std;
using namespace flat::core;

labelled::labelled(const std::string& label) : label(label) {}

string labelled::random_id(uint8_t length) {
    
    string out;

    for (uint8_t i = 0; i < length; ++i)
        out += (char)(rand() % 93 + 33);

    return out;
}

