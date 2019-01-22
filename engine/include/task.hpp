#pragma once

#include <functional>
#include <set>

namespace flat {
    namespace core {
        class task {
        public:
            enum class priority : unsigned {
                max = 0,
                high = 1,
                none = 2,
                low = 3,
                min = 4,
            };

            // to pass a member function (method) use std::bind(f, obj)
            task(std::function<void()> callback, priority p = priority::none);
            task() = delete;

            inline void operator()() const { m_callback(); }
            friend bool operator<(const task& lhs, const task& rhs);

        private:
            const priority m_priority;
            std::function<void()> m_callback;
        };

        struct job : public std::multiset<task> {
            inline void add_task(task t) {
                this->insert(t);
            }

            void invoke_tasks();
        };
    }
}