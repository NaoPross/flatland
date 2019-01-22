#pragma once

#include <set>

namespace flat {
    namespace core {

        enum class priority_t : unsigned  {
            max = 0,
            higher = 1,
            high = 2,
            none = 3,
            low = 4,
            lower = 5,
            min = 6,
        };

        class prioritized {
        private:
            const priority_t m_priority;

        public:
            prioritized(priority_t priority = priority_t::none)
                : m_priority(priority) {}

            const priority_t priority() const {
                return m_priority;
            }
        };       
        
        bool operator<(const prioritized& lhs, const prioritized& rhs) {
            return lhs.priority() < rhs.priority();
        }
    }
}


