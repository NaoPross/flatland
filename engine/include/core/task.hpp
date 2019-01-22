#pragma once

#include "priority.hpp"

#include <functional>
#include <set>

namespace flat {
    namespace core {
        class task : public prioritized {
        public:
            task() = delete;
            task(std::function<void()> callback, priority_t p = priority_t::none);

            inline void operator()() const { m_callback(); }

        private:
            std::function<void()> m_callback;
        };

        struct job : public std::multiset<task> {
            inline auto add_task(task t) {
                this->insert(t);
            }

            void invoke_tasks();
        };
    }
}