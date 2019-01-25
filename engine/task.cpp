#include "core/task.hpp"

#include <functional>
#include <memory>
#include <algorithm>
#include <cassert>

#include <vector>

#include <iostream>

using namespace std;

namespace flat {
    namespace core {
        task::task(task::callback f, priority_t p)
            : prioritized(p), m_callback(f) {}

        std::shared_ptr<task> job::delegate_task(task::callback f, priority_t p) {
            auto shared = std::make_shared<task>(f, p);
            insert(shared);

            return shared;
        }

        void job::add_task(task::callback f, priority_t p) {
            insert(std::make_shared<task>(f, p));
        }

        void job::invoke_tasks() {
            // expired tasks to remove
            std::vector<job::value_type> to_erase;

            for (auto tp : *this) {
                // check that the task has not expired
                if (std::shared_ptr<task> t = tp.lock())
                    std::invoke(*t);
                else
                    to_erase.push_back(tp);
            }

            // delete expired tasks
            for (auto tp : to_erase) {
                erase(tp);
            }
        }

    }
}
