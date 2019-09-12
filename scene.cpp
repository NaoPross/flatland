#include "flatland/scene.hpp"

#include "flatland/flatland.hpp"

using namespace flat;

scene::scene() {}

std::shared_ptr<wsdl2::texture>
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
        auto tex = std::make_shared<wsdl2::streaming_texture>(state.window().get_renderer(), *surf);
        auto pair = std::make_pair<const std::string, std::shared_ptr<wsdl2::texture>>(std::move(path), std::static_pointer_cast<wsdl2::texture>(tex));
        // TODO, revision, too complex
        // insert into local and global state
        m_textures.insert(pair);
        state.m_textures.insert(pair);

        return std::static_pointer_cast<wsdl2::texture>(tex);
    } else {
        npdebug("failed to load surface");
    }

    return nullptr;
}

std::shared_ptr<sprite>
scene::load_sprite(std::shared_ptr<tileset> tileset, 
            const mm::vec2<int>& pos,
            unsigned index)
{
      if (tileset != nullptr) {

          auto&& valid_ptr = std::make_shared<sprite>(tileset, pos, index);
          m_sprites.insert(valid_ptr);
          insert(std::static_pointer_cast<renderable>(valid_ptr));
          return valid_ptr;
      }

      return nullptr;
}

std::shared_ptr<sprite>
scene::load_sprite(const std::string& path, 
                   const mm::vec2<int>& pos,
                   unsigned index)
{
    auto _tileset = load_tileset(path);

    if (_tileset == nullptr) {

        npdebug("could not load tileset");
        return nullptr;
    }

    return load_sprite(_tileset, pos, index);
}

void theater::render() const
{
    state::get().current_scene().render();
}

