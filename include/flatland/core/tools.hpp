#pragma once

#include <memory>

/*
* Various core tools
*/
namespace flat::core {

    // pair composed by weak ptr, but constructed with shared ptr
    template <class T, class U>
    struct weak_pair : public std::pair<std::weak_ptr<T>, std::weak_ptr<U>> {
        
        event(const std::shared_ptr<T>& a,
              const std::shared_ptr<U>& b)
            : pair(a, b) {}
    };
};
