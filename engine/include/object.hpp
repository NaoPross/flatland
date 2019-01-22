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
        class object;
    }
}

class flat::core::object
{
    std::string id;

    /* Common list of objects */
    static std::list<object*> all_objects;

public:
    
    object();
    ~object();

    void set_id(const std::string&);

    const std::string& get_id() const;

    /* Static accessors to allObject */

    static bool is_allocated(object*);
   
    static std::vector<object*>& get_by_id(const std::string& id, std::vector<object*>&);

    static std::string random_id(uint8_t length = 8);
};

