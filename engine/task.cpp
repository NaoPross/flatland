#include "core/task.hpp"

#include <functional>
#include <memory>


namespace flat {
    namespace core {
        task::task(task::callback f, priority_t p)
            : m_callback(f) {}

        std::shared_ptr<task> job::make_task(task::callback f, priority_t p) {
            auto shared = std::make_shared<task>(f, p);
            insert(shared);
            return shared;
        }

        void job::invoke_tasks() {
            std::for_each(begin(), end(), [&](auto tp) {
                // check that the task has not expired
                if (auto t = tp.lock()) {
                    // run task
                    std::invoke(*t);
                } else {
                    // delete task
                    erase(tp);
                }
            });
        }
    }
}
