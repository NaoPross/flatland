#pragma once

#include <memory>
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

        struct prioritize {
            bool operator()(const prioritized& lhs, const prioritized& rhs) {
                return lhs.priority() < rhs.priority();
            }

            bool operator()(const std::weak_ptr<prioritized> lhs, const std::weak_ptr<prioritized>& rhs) {
                if (auto l = lhs.lock()) {
                    if (auto r = rhs.lock()) {
                        // if both valid, check their priority
                        // in case they are the same, left is prioritized
                        return l->priority() < r->priority();
                    } else {
                        // if right is expired, left is prioritized
                        return true;
                    }
                } else {
                    // if left is expired, the right is prioritized
                    return false;
                }
            }
        };

        template<typename Prioritized>
        using queue = std::multiset<Prioritized, prioritize>;
    }
}


