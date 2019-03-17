#include "sprite.hpp"
#include "flatland.hpp"

using namespace flat;

/*
 * Texture loading section
 */

std::unordered_map<std::string, std::weak_ptr<wsdl2::texture>> sprite::textures;

sprite * sprite::from_file(const std::string& path,
                           const wsdl2::rect& bounds,
                           const wsdl2::rect& viewport, 
                           const std::string& lab, 
                           uint32_t overlap)
{
    std::shared_ptr<wsdl2::texture> tex = nullptr;
    auto it = textures.find(path);

    auto rend = renderer();

    // check for non existence in list
    if (it == textures.end() || it->second.expired())
    {
        // check for renderer initialization state
        if (rend != NULL)
        {
            npdebug("Trying to load ", path)

            // get the optional
            auto opt_tex = wsdl2::texture::load(path, *rend);

            npdebug("Loaded ", path)

            if (opt_tex)
            {
                // force to initialize pointer with the move semantic
                tex = std::shared_ptr<wsdl2::texture>(new wsdl2::texture(std::move(*opt_tex)));
                textures.insert(std::pair<std::string, std::weak_ptr<wsdl2::texture>>(path, tex));

            } else {
                npdebug("Could not load the texture ", path, ". Non-existing file or wrong format");
            }

        } else {
            
            npdebug("Could not load the texture: ", path, ". Loading before calling flat::init")
        }

    } else
        tex = it->second.lock();

    return (tex != nullptr) ? new sprite(tex, bounds, viewport, lab, overlap) : NULL;
}

std::shared_ptr<wsdl2::texture> sprite::create_texture( std::size_t width, 
                                                        std::size_t height,
                                                        wsdl2::pixelformat::format p)
{
    std::shared_ptr<wsdl2::texture> tex = nullptr;
    auto rend = renderer();

    // check initialization state and non existence
    if (rend != NULL)
        tex = std::make_shared<wsdl2::texture>(*rend, p, wsdl2::texture::access::static_, width, height);

    return tex;
}

/* 
 * Sprite section
 */ 

/*
 * Construct a sprite basing on a loaded texture 
 * Passing a null texture will throw an exception
 */
sprite::sprite( std::shared_ptr<wsdl2::texture> tex,
                const wsdl2::rect& bounds,
                const wsdl2::rect& viewport, 
                const std::string& lab,
                uint32_t overlap)

    : textured(bounds, viewport, lab, overlap), m_texture(tex)
{
    if (tex == nullptr)
    {
        npdebug("Giving a null texture to the sprite labelled: ", lab)
        throw std::runtime_error("Giving a null texture to a sprite");
    }

    if (bounds.w < 1)
        set_width(tex->width());

    if (bounds.h < 1)
        set_width(tex->height());

    if (viewport.w < 1 || viewport.h < 1)
    {
        // set both
        wsdl2::rect v = viewport;

        if (v.w < 1)
            v.w = static_cast<int>(width());

        if (v.h < 1)
            v.h = static_cast<int>(height());

        set_viewport(v);
    }
}

/*
 * Tileset section
 */

tileset::tileset( std::shared_ptr<wsdl2::texture> tex,
                  const wsdl2::rect& bounds,
                  const std::initializer_list<wsdl2::rect>& init,
                  const std::string& lab,
                  uint32_t overlap)

    : sprite(tex, bounds, wsdl2::rect{0, 0, 0, 0}, lab, overlap), m_current(0)
{
    if (init.size() == 0)
        map(0, wsdl2::rect{0, 0, static_cast<int>(tex->width()), static_cast<int>(tex->height())});
    else {
       
        std::size_t i = 0; 

        for (auto rect : init)
            map(i++, rect);
    }

    // set first index viewport
    set(0);
}

void tileset::map(std::size_t i, const wsdl2::rect& rect)
{
    this->insert(std::pair<std::size_t, wsdl2::rect>(i, rect));
}

void tileset::set(std::size_t i)
{
    auto it = this->find(i);

    if (it != this->end())
    {
        m_current = i;
        set_viewport(it->second);
    }
}

std::size_t tileset::current() const
{
    return m_current;
}

void tileset::clear()
{
    std::unordered_map<std::size_t, wsdl2::rect>::clear();

    // take the default viewport
    map(0, wsdl2::rect{0, 0, static_cast<int>(texture_ptr()->width()), static_cast<int>(texture_ptr()->height())});
    m_current = 0;
}

void tileset::erase(std::size_t i)
{
    std::unordered_map<std::size_t, wsdl2::rect>::erase(i);

    if (size() == 0)
    {
        // take the default viewport
        map(0, wsdl2::rect{0, 0, static_cast<int>(texture_ptr()->width()), static_cast<int>(texture_ptr()->height())});
        m_current = 0;
    }

    if (m_current == i)
        set(this->begin()->first);
}


