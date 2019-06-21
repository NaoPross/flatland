#pragma once

#include "wsdl2/video.hpp"

#include "renderable.hpp"
#include "sprite.hpp"


#include <memory>
#include <optional>
#include <string>

#include <unordered_map>


namespace flat
{
    class scene
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
                m_tilesets.insert({tex, valid_ptr});
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
                m_sprites.insert({tileset, valid_ptr});
                return valid_ptr;
            }

            return std::nullopt;
        }

        // bool load_tileset(std::shared_ptr<wsdl2::texture> texture);
        // bool load_sprite(std::shared_ptr<wsdl2::texture> texture);
        
    private:
        rendergroup m_renderables;

        std::unordered_multimap<std::shared_ptr<tileset>,
                           std::shared_ptr<sprite>> m_sprites;

        std::unordered_multimap<std::shared_ptr<wsdl2::texture>,
                           std::shared_ptr<tileset>> m_tilesets;

        std::unordered_map<const std::string,
                           std::shared_ptr<wsdl2::texture>,
                           std::hash<std::string>> m_textures;
    };
}
