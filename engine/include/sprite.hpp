#pragma once

#include "wsdl2/video.hpp"

#include "renderable.hpp"
#include "entity.hpp"

#include <unordered_map>

namespace flat {

    class sprite;

    /* A wrapper around a texture referenced by sprites
     *
     * The purpose of a tileset is to map parts of an image (rectangles, viewports)
     * (tileset source texture) to numbers which the sprite uses to access.
     */
    class tileset : private std::unordered_map<unsigned, wsdl2::rect>
    {
    public:
        friend class sprite;

        /// generate with a single rectangle whose size is the entire texture
        tileset(wsdl2::texture&& src);

        /// generate with grid
        tileset(wsdl2::texture&& src,
                std::size_t cell_w,
                std::size_t cell_h,
                std::size_t margin = 0,
                std::size_t spacing = 0);

        /// generate from custom set of rectangles
        tileset(wsdl2::texture&& src,
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
        wsdl2::texture m_texture;
    };

    /* Any graphical entity with an image
     */
    class sprite : public renderable, public entity
    {
    public:
        using vector_type = entity::vector_type;

        sprite(vector_type pos,
               std::shared_ptr<tileset> tileset,
               unsigned tileset_index = 0);

        // TODO: constructors for entity with weird shapes

        virtual void render() override;

    private:
        std::shared_ptr<tileset> m_tileset;
        unsigned m_tileset_index;
    };
}