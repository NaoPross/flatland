#pragma once

#include "wsdl2/video.hpp"

#include <memory>
#include <set>

namespace flat
{
    namespace trait {
        /// Literally anything that needs to be rendered
        struct renderable {
        public:
            /* The Z value indicates the rendering order.
            * A Z value of 0 is just after the background.
            * the higher the value the more on the front (rendered on top)
            */
            unsigned z = 0;
            bool visible = true;

            virtual ~renderable() {}
            virtual void render() = 0;
        };
    }

    /* A group of renderable objects sorted by their z value */
    class rendergroup : public trait::renderable,
                        public std::set<std::shared_ptr<trait::renderable>>
    {
    public:
        void render() override
        {
            for (auto&& child : *this)
                if (child->visible)
                    child->render();
        }
    };
}

inline bool operator<(const flat::trait::renderable& lhs, const flat::trait::renderable& rhs)
{
    return lhs.z < rhs.z;
}
