#include "core/task.hpp"

#include <functional>


namespace flat {
    namespace core {
        task::task(std::function<void()> callback, priority_t p)
            : m_callback(callback) {}

        void job::invoke_tasks() {
            for (const task& t : *this) {
                t();
            }
        }
    }
}
