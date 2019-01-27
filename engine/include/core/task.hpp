#pragma once

#include "priority.hpp"

#include <unordered_set>
#include <functional>
#include <variant>
#include <memory>

namespace flat {
    namespace core {
        // forward decl
        class job;
        class task;

        class task : public prioritized {
        public:
            using ptr = std::shared_ptr<task>;
            using callback = std::function<void(void)>;

            task() = delete;
            task(callback f, priority_t p = priority_t::none);

            inline void operator()() const { m_callback(); }

        private:
            callback m_callback;
        };

        class job : protected queue<std::weak_ptr<task>> {
        public:
            /// add a task function owned by the job object
            void add_task(task::callback f, priority_t p = priority_t::none);

            /// add a task function not owned by the job object (weak_ptr)
            std::shared_ptr<task> delegate_task(task::callback f, priority_t p = priority_t::none);

            /// add a task methods not owned by the job object (weak_ptr),
            ///  this allows to make the task die when the owner object goes out of scope
            template<typename R, typename T>
            inline std::shared_ptr<task> delegate_task(R T::*mf, T* obj, priority_t p = priority_t::none) {
                return delegate_task(std::bind(mf, obj), p);
            }

            /// run tasks
            void invoke_tasks();
            inline void operator()() { invoke_tasks(); } 
        };
    }
}
