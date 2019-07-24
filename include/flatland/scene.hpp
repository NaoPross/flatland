#pragma once

#include "flatland/trait/renderable.hpp"
#include "sprite.hpp"
#include "debug.hpp"

#include <optional>
#include <unordered_set>
#include <unordered_map>

namespace flat
{
    /* 
     * A group of textures, tilesets and sprites that manages intelligently its
     * resources.
     */
    class scene : public rendergroup
    {
    public:
        std::string title = "Untitled Scene";

        scene();

        std::shared_ptr<wsdl2::texture>
        load_texture(const std::string& path);

        template<typename ...Args>
        std::shared_ptr<tileset>
        load_tileset(const std::string& path, Args&& ...args)
        {
            auto&& _tex = load_texture(path);
            if (!_tex)  {
                npdebug("could not load texture");
                return nullptr;
            }

            return load_tileset(_tex.value(), std::forward<Args...>(args)...);
        }

        template<typename ...Args>
        std::shared_ptr<tileset>
        load_tileset(std::shared_ptr<wsdl2::texture> tex, Args&& ...args)
        {
            if (auto&& valid_ptr = std::make_shared<tileset>(tex, args...)) {
                m_tilesets.insert(valid_ptr);
                return valid_ptr;
            }

            return nullptr;
        }

        /*
         * It loads a sprite given a path for a tileset,
         * it returns nullptr in case of a null tileset argument
         */
        std::shared_ptr<sprite>
        load_sprite(const std::string& path, 
                    const mm::vec2<int>& pos = {0, 0},
                    unsigned index = 0);

        /*
         * It loads a sprite given a tileset,
         * it returns nullptr in case of a null tileset argument
         */
        std::shared_ptr<sprite>
        load_sprite(std::shared_ptr<tileset> tileset, 
                    const mm::vec2<int>& pos = {0, 0},
                    unsigned index = 0);

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
    struct theater : public trait::renderable
    {
        void render() const override;
    };
}
