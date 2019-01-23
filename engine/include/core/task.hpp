#pragma once

#include "priority.hpp"

#include <functional>
#include <memory>

namespace flat {
    namespace core {
        // forward decl
        class job;
        class task;

        class task : public prioritized {
        public:
            using callback = std::function<void(void)>;

            task() = delete;
            task(callback f, priority_t p = priority_t::none);

            inline void operator()() const { m_callback(); }

        private:
            callback m_callback;
        };

        struct job : public queue<std::weak_ptr<task>> {
            std::shared_ptr<task> make_task(task::callback f, priority_t p = priority_t::none);
            void invoke_tasks();

            inline void operator()() {
                invoke_tasks();
            }
        };
    }
}
