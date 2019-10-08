#pragma once

#include "wsdl2/video.hpp"

#include <memory>
#include <set>

namespace flat
{
    namespace trait
    {
        /// Literally anything that needs to be rendered
        struct renderable {
        public:
            /* The Z value indicates the rendering order.
            * A Z value of 0 is just after the background.
            * the higher the value the more on the front (rendered on top)
            */
            unsigned z = 0;
            bool visible = true;
            // TODO: add projection matrix

            virtual ~renderable() = default;
            virtual void render() const = 0;

            // TODO, stack<scene> order problem?
            // YES IT DOES
            /*bool operator<(const flat::trait::renderable& rhs) const
            {
                return z < rhs.z;
            }*/

            struct zlevel_less {
                bool operator()(const std::shared_ptr<renderable>& lhs, const std::shared_ptr<renderable>& rhs) const {
                    return lhs->z < rhs->z;
                }
            };
        };

        
    }

    /* A group of renderable objects sorted by their z value */
    class rendergroup : public trait::renderable,
                        public std::multiset<std::shared_ptr<trait::renderable>, trait::renderable::zlevel_less>
    {
    public:
        void render() const
        {
            for (auto&& child : *this)
                if (child->visible)
                    child->render();
        }
    };
}

