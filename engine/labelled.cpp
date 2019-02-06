#include "core/labelled.hpp"

#include<functional>
#include <random>

using namespace flat::core;

std::hash<std::string> hash_gen;

labelled::labelled(const std::string& _label) 
    : label(_label), 
      hash((_label.empty()) ? random_hash() : hash_gen(_label))
{
}

// random generation

std::default_random_engine generator(std::random_device{}());
std::uniform_int_distribution<long long unsigned> distribution(0,0xFFFFFFFFFFFFFFFF);


std::size_t labelled::random_hash() {

    return distribution(generator);  
}

