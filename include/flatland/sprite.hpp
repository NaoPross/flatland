#pragma once

#include "trait/renderable.hpp"
#include "trait/projector.hpp"

#include <wsdl2/video.hpp>
#include <unordered_map>


namespace flat
{

    class sprite;

    /* A wrapper around a texture, referenced by sprites
     *
     * The purpose of a tileset is to map parts of an image (rectangles, viewports)
     * (tileset source texture) to numbers which the sprite uses to access.
     */
    class tileset : private std::unordered_map<unsigned, wsdl2::rect>
    {
    public:
        friend class sprite;

        /// generate with a single rectangle whose size is the entire texture
        tileset(std::shared_ptr<wsdl2::texture> src);

        /// generate with grid
        tileset(std::shared_ptr<wsdl2::texture> src,
                std::size_t cell_w,
                std::size_t cell_h,
                std::size_t margin = 0,
                std::size_t spacing = 0);

        /// generate from custom set of rectangles
        tileset(std::shared_ptr<wsdl2::texture> src,
                std::initializer_list<std::pair<unsigned, wsdl2::rect>> viewports);

        /// inherited from unordered_map
        using std::unordered_map<unsigned, wsdl2::rect>::operator[];
        using std::unordered_map<unsigned, wsdl2::rect>::at;

        using std::unordered_map<unsigned, wsdl2::rect>::size;

        using std::unordered_map<unsigned, wsdl2::rect>::begin;
        using std::unordered_map<unsigned, wsdl2::rect>::cbegin;
        using std::unordered_map<unsigned, wsdl2::rect>::end;
        using std::unordered_map<unsigned, wsdl2::rect>::cend;

    private:
        std::shared_ptr<wsdl2::texture> m_texture;
    };

    /* 
     * Any graphical entity with an image
     */
    class sprite : public trait::renderable
                    //public trait::positioned
    {
    public:
        sprite(std::shared_ptr<tileset> tileset,
               trait::projector * proj,
              // mm::vec2<int> pos = {0, 0},
               unsigned index = 0);

        /// trait::renderable
        virtual void render() const;

        inline void set_projector(trait::projector * p)
        {
            if (p != nullptr)
                m_projector = p;
        }

        /// trait::positioned
        // TODO, not the best idea, use an object instead to determine positioning and size
        //mm::vec2<int>& pos() override { return m_pos; }
        //void pos(const mm::vec2<int>& newpos) override { m_pos = newpos; }

    protected:
        //mm::vec2<int> m_pos;

        // find a safer solution
        trait::projector * m_projector;

        std::shared_ptr<tileset> m_tileset;
        unsigned m_ts_index;
    };
}
