#include "core/task.hpp"

#include <functional>
#include <memory>

#include <vector>

#include <iostream>

using namespace std;

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
            //std::for_each(begin(), end(), [&](auto tp) {

            vector<weak_ptr<task>> to_erase;

            for (auto tp : (*this)) {
                if (tp.expired())
                    to_erase.push_back(tp);
            }

            for (auto tp : to_erase)
                erase(tp);

            for (auto tp : (*this)) {
                // check that the task has not expired
                    // run task
                    auto t = tp.lock();
                    std::invoke(*t);
            }
        }

    }
}
