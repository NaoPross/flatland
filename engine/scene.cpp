#include "scene.hpp"

#include "flatland.hpp"

using namespace flat;

scene::scene() {}

std::optional<std::shared_ptr<wsdl2::texture>>
scene::load_texture(const std::string& path)
{
    auto&& state = flat::state::get();

    // check if not already loaded in the local state
    if (m_textures.count(path)) {
        npdebug("found texture already loaded in local state");
        return m_textures[path];

    // check if not already loaded in the global state
    } else if (state.m_textures.count(path)) {
        // check that is not expired
        if (auto&& valid_ptr = state.m_textures[path].lock()) {
            npdebug("found texture already loaded in global state");
            // copy to local state
            m_textures.insert({path, valid_ptr});
            return valid_ptr;
        }
    }

    // load new texture
    if (auto&& surf = wsdl2::surface::load(path)) {
        auto tex = std::make_shared<wsdl2::texture>(state.renderer(), *surf);
        // insert into local and global state
        m_textures.insert({path, tex});
        state.m_textures.insert({path, tex});

        return tex;
    } else {
        npdebug("failed to load surface");
    }

    return std::nullopt;
}

void theater::render()
{
    state::get().current_scene().render();
}
