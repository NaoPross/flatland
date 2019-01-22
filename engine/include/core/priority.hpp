#pragma once

#include <set>

namespace flat
{
    namespace core
    {
        enum class prior_t : unsigned 
        {
            max = 0,
            higher = 1,
            high = 2,
            none = 3,
            low = 4,
            lower = 5,
            min = 6,
        };

        class prioritized
        {
            const prior_t m_priority;

        public:

            prioritized(prior_t m_priority = prior_t::none) : m_priority(m_priority) {}

            const prior priority() const
            {
                return m_priority;
            }
        };
       
        struct prior_criteria
        {
            bool operator()(const prioritized& a, const prioritized& b) const
            {
                return a.priority() < b.priority();
            }
        };

        /* Compiler will complain if don't pass a non prioritized object */

        template <class T>
        using prior_set = std::multiset<T, prior_criteria>;


        /* Equivalent but with pointers */

        struct prior_ptr_criteria
        {
            bool operator()(const prioritized* a, const prioritized* b) const
            {
                return a->priority() < b->priority();
            }
        };

        /* Compiler will complain if don't pass a non prioritized object */

        template <class T>
        using prior_ptr_set = std::multiset<T*, prior_criteria>;
    }
};
