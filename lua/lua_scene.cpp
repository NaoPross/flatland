#include "lua/lua_scene.hpp"
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

    auto scene_type = s.new_usertype<scene>("scene", sol::constructors<scene()>(),
                        "title", &scene::title);

    scene_type.set("load_texture", std::object [this] scene::(const std::string& path) {

        auto opt = this->load_texture(path);
        return (opt) ? lua::texture(path, *this, *opt) : sol::lua_nil;
    });
}
