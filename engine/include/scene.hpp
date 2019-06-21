#pragma once

#include "wsdl2/video.hpp"

#include "renderable.hpp"
#include "sprite.hpp"


#include <memory>
#include <optional>
#include <string>

#include <unordered_map>
#include <unordered_set>


namespace flat
{
    /* 
     * A group of textures, tilesets and sprites that manages intelligently its
     * resources.
     */
    class scene : public rendergroup
    {
    public:
        scene();

        std::optional<std::shared_ptr<wsdl2::texture>>
        load_texture(const std::string& path);

        template<typename ...Args>
        std::optional<std::shared_ptr<tileset>>
        load_tileset(const std::string& path, Args&& ...args)
        {
            auto&& _tex = load_texture(path);
            if (!_tex)  {
                npdebug("could not load texture");
                return std::nullopt;
            }

            auto&& tex = _tex.value();

            if (auto&& valid_ptr = std::make_shared<tileset>(tex, args...)) {
                m_tilesets.insert(valid_ptr);
                return valid_ptr;
            }

            return std::nullopt;
        }

        template<typename ...Args>
        std::optional<std::shared_ptr<sprite>>
        load_sprite(const std::string& path, Args&& ...args)
        {
            auto&& _tileset = load_tileset(path);
            if (!_tileset) {
                npdebug("could not load tileset");
                return std::nullopt;
            }

            auto&& tileset = _tileset.value();

            if (auto&& valid_ptr = std::make_shared<sprite>(tileset, args...)) {
                m_sprites.insert(valid_ptr);
                insert(std::static_pointer_cast<renderable>(valid_ptr));
                return valid_ptr;
            }

            return std::nullopt;
        }

        // bool load_tileset(std::shared_ptr<wsdl2::texture> texture);
        // bool load_sprite(std::shared_ptr<wsdl2::texture> texture);

        inline auto& sprites() { return m_sprites; }
        
    private:
        std::unordered_set<std::shared_ptr<sprite>> m_sprites;

        std::unordered_set<std::shared_ptr<tileset>> m_tilesets;

        std::unordered_map<const std::string,
                           std::shared_ptr<wsdl2::texture>,
                           std::hash<std::string>> m_textures;
    };

    /*
     * A renderable object whose job is to always render only the current scene
     */
    struct theater : public renderable
    {
        void render() override;
    };
}
