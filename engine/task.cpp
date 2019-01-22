#include "task.hpp"

#include <functional>


namespace flat {
    namespace core {
        bool operator<(const task& lhs, const task& rhs) {
            // task::priority::max is 0, a higher number is lower priority
            return lhs.m_priority < rhs.m_priority;
        }

        task::task(std::function<void()> callback, task::priority p)
            : m_priority(p), m_callback(callback) {}

        void job::invoke_tasks() {
            for (const task& t : *this) {
                t();
            }
        }
    }
}