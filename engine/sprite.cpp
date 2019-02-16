#include "sprite.hpp"
#include "flatland.hpp"
#include "core/signal.hpp"

using namespace flat;

/*
 * Texture loading section
 */

std::unordered_map<std::string, std::weak_ptr<wsdl2::texture>> textures;

std::shared_ptr<wsdl2::texture> texloader::get(const std::string& path)
{
    std::shared_ptr<wsdl2::texture> tex = nullptr;
    auto it = textures.find(path);

    auto rend = renderer();

    // check non existence
    if (it == textures.end() || it->second.expired())
    {
        // check initialization state
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

    return tex;
}

std::shared_ptr<wsdl2::texture> create( const std::string& name,
                                        std::size_t width, 
                                        std::size_t height,
                                        wsdl2::pixelformat::format p, 
                                        wsdl2::texture::access a)
{
    std::shared_ptr<wsdl2::texture> tex = nullptr;
    auto it = textures.find(name);

    auto rend = renderer();

    // check initialization state and non existence
    if (rend != NULL && (it == textures.end() || it->second.expired()))
    {
        tex = std::make_shared<wsdl2::texture>(*rend, p, a, width, height);
        textures.insert(std::pair<std::string, std::weak_ptr<wsdl2::texture>>(name, tex));
    }

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

    : renderbase(overlap, lab), 
      m_texture(tex), 
      m_bounds(bounds),
      m_viewport(viewport) 
{
    if (tex == nullptr)
    {
        npdebug("Giving a null texture to the sprite labelled: ", lab)
        throw std::runtime_error("Giving a null texture to a sprite");
    }
}


/*
 * Sprite bounds are the same as the image bounds:
 *
 * Bounds = (0, 0, img-width, img-height)
 */
sprite::sprite( std::shared_ptr<wsdl2::texture> tex,
                int x, int y,
                const wsdl2::rect& viewport, 
                const std::string& lab,
                uint32_t overlap)

    : sprite(tex, wsdl2::rect{x, y, tex->width(), tex->height()}, viewport, lab, overlap) {}

/*
 * Viewport and bounds are deduced from the image size, precisely:
 *
 * Bounds = (x, y, img-width, img-height)
 * Viewport = (0, 0, img-width, img-height)
 */
sprite::sprite( std::shared_ptr<wsdl2::texture> tex,
                int x, int y,
                const std::string& lab,
                uint32_t overlap)

    : sprite(tex, wsdl2::rect{x, y, tex->width(), tex->height()}, {0, 0, tex->width(), tex->height()}, lab, overlap) {}


sprite::sprite( std::shared_ptr<wsdl2::texture> tex,
                const wsdl2::rect& bounds,
                const std::string& lab,
                uint32_t overlap)

    : sprite(tex, bounds, wsdl2::rect{0, 0, tex->width(), tex->height()}, lab, overlap) {}


void sprite::render()
{
    // TODO, generalize with flip angle
    m_texture->render(m_viewport, m_bounds); 
}

void sprite::focus()
{
    core_channel().emit(focus_call(this)); 
}

void sprite::set_location(int x, int y)
{
    m_bounds.x = x;
    m_bounds.y = y;
}

void sprite::set_location(const mm::vec2<int>& l)
{
    m_bounds.x = l[0];
    m_bounds.y = l[1]; 
}

void sprite::set_width(std::size_t width)
{
    m_bounds.w = width;
}

void sprite::set_height(std::size_t height)
{
    m_bounds.h = height;
}

void sprite::set_size(std::size_t width, std::size_t height)
{
    m_bounds.w = width;
    m_bounds.h = height;
}

void sprite::set_bounds(const wsdl2::rect& b)
{
    m_bounds = b;
}

mm::vec2<int> sprite::location() const
{
    return mm::vec2<int>({m_bounds.x, m_bounds.y});
}

std::size_t sprite::width() const
{
    return m_bounds.w;
}

std::size_t sprite::height() const
{
    return m_bounds.h;
}

wsdl2::rect sprite::bounds()
{
    return m_bounds;
}

const wsdl2::rect& sprite::bounds() const
{
    return m_bounds;
}

void sprite::set_viewport(const wsdl2::rect& v)
{
    m_viewport = v;
}

wsdl2::rect sprite::viewport()
{
    return m_viewport;
}

const wsdl2::rect& sprite::viewport() const
{
    return m_viewport;
}

/*
 * Tileset section
 */

tileset::tileset( std::shared_ptr<wsdl2::texture> tex,
                  const wsdl2::rect& bounds,
                  const std::initializer_list<wsdl2::rect>& init,
                  const std::string& lab,
                  uint32_t overlap)

    : sprite(tex, bounds, lab, overlap), m_current(0)
{
    if (init.size() == 0)
        map(0, wsdl2::rect{0, 0, tex->width(), tex->height()});
    else {
       
        std::size_t i = 0; 

        for (auto rect : init)
            map(i++, rect);
    }
}

tileset::tileset( std::shared_ptr<wsdl2::texture> tex,
                  int x, int y,
                  const std::initializer_list<wsdl2::rect>& init,
                  const std::string& lab,
                  uint32_t overlap)

    : tileset(tex, wsdl2::rect{x, y, tex->width(), tex->height()}, init, lab, overlap)
{

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
    map(0, wsdl2::rect{0, 0, m_texture->width(), m_texture->height()});
    m_current = 0;
}

void tileset::erase(std::size_t i)
{
    std::unordered_map<std::size_t, wsdl2::rect>::erase(i);

    if (size() == 0)
    {
        // take the default viewport
        map(0, wsdl2::rect{0, 0, m_texture->width(), m_texture->height()});
        m_current = 0;
    }

    if (m_current == i)
        set(this->begin()->first);
}


