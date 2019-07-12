#include "luaint/lua_scene.hpp"
#include "scene.hpp"

using namespace flatland;

void lua::init_scene(sol::lua_state& s)
{
    /*
     * Texture binding:
     *  - no constructors
     *  - TODO, low level options
     */
    s.new_usertype<lua::texture>("texture",
            "path", &lua::texture::path,
            "owner", &lua::texture::owner);

    /*
     * Scene binding
     */
    auto scene_type = s.new_usertype<lua::scene>("scene", sol::constructors<scene()>(),
                        "title", &scene::title);

    scene_type.set("load_texture", &lua::scene::lua_load_texture);
}
