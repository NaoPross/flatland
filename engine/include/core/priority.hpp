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
        public:
            const priority_t priority;

            prioritized(priority_t p = priority_t::none)
                : priority(p) {}
        };       

        struct prioritize {
            bool operator()(const prioritized& lhs, const prioritized& rhs) {
                return lhs.priority < rhs.priority;
            }

            bool operator()(const prioritized * const lhs, const prioritized * const rhs) {
                return lhs->priority < rhs->priority;
            }

            bool operator()(const std::unique_ptr<const prioritized> lhs,
                            const std::unique_ptr<const prioritized> rhs)
            {
                return operator()(lhs.get(), rhs.get());
            }

            bool operator()(const std::weak_ptr<const prioritized> lhs,
                            const std::weak_ptr<prioritized> rhs)
            {
                if (auto l = lhs.lock()) {
                    if (auto r = rhs.lock()) {
                        // if both valid, check their priority
                        // in case they are the same, left is prioritized
                        return operator()(l.get(), r.get());
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


