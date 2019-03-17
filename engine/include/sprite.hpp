#ifndef __FLATSPRITE_H__
#define __FLATSPRITE_H__

#include "textured.hpp"
#include <list>

namespace flat {

/*
 *  Textured sprite, used for static rendering.
 *  Textures are shared between the other sprites.
 */
class sprite : public textured
{

    std::shared_ptr<wsdl2::texture> m_texture;

    virtual wsdl2::texture& texture() override
    {
        return *m_texture;
    }

public:

    sprite( std::shared_ptr<wsdl2::texture>,
            const wsdl2::rect& bounds = wsdl2::rect{0, 0, 0, 0},
            const wsdl2::rect& viewport = wsdl2::rect{0, 0, 0, 0}, 
            const std::string& lab = "",
            uint32_t overlap = 1);

    /*
     * Sprite bounds are the same as the image bounds:
     *
     * Bounds = (0, 0, img-width, img-height)
     */
    sprite( std::shared_ptr<wsdl2::texture> tex,
            int x = 0, int y = 0,
            const wsdl2::rect& viewport = wsdl2::rect{0, 0, 0, 0}, 
            const std::string& lab = "",
            uint32_t overlap = 1)
        : sprite(tex, 
                 wsdl2::rect{x, y, 0, 0},
                 viewport,
                 lab,
                 overlap) {}

    ~sprite() {}

    inline std::shared_ptr<wsdl2::texture> texture_ptr()
    {
        return m_texture;
    }

    inline std::shared_ptr<const wsdl2::texture> texture_ptr() const
    {
        return std::const_pointer_cast<const wsdl2::texture>(m_texture);
    }

    /*
     * Get a sprite from a specified path.
     * If the sprite's texture was not loaded before, then
     * it returns a new loaded sprite.
     * In case of failure (ex: non-existing file), it returns a nullptr.
     *
     * Warning: making a texture go out of scope
     *          implies that it will be freed and 
     *          no more available.
     *          A texture persistence depends on
     *          how many instances of this texture
     *          are present.
     */
    static std::unordered_map<std::string, std::weak_ptr<wsdl2::texture>> textures;

    static sprite * from_file(const std::string& path, 
                              const wsdl2::rect& bounds = wsdl2::rect{0, 0, 0, 0},
                              const wsdl2::rect& viewport = {0, 0, 0, 0}, 
                              const std::string& lab = "", 
                              uint32_t overlap = 1);

    inline static sprite * from_file(const std::string& path, 
                                     int x = 0, int y = 0,
                                     const wsdl2::rect& viewport = {0, 0, 0, 0}, 
                                     const std::string& lab = "", 
                                     uint32_t overlap = 1)
    {
        return from_file(path, wsdl2::rect{x, y, 0, 0}, viewport, lab, overlap);
    }

    /*
     * It creates a static rendered texture which can to be applied to
     * a sprite. The management of the returned pointer is totally customizable.
     */
    static std::shared_ptr<wsdl2::texture> create_texture( std::size_t width, 
                                                           std::size_t height,
                                                           wsdl2::pixelformat::format p = wsdl2::pixelformat::format::unknown);
};

/*
 * A sprite that automaticly maps the tileset by indexes
 *
 * A zero value must always exist, then clearing the map
 * means to reset to the default viewport
 */
class tileset : public sprite, private std::unordered_map<std::size_t, wsdl2::rect>
{
    std::size_t m_current;

public:

     /*
     * Bounds are deduced from the image size, precisely:
     *
     * Bounds = (x, y, img-width, img-height)
     *
     * Init list = {} takes the full image as viewport
     */
    tileset( std::shared_ptr<wsdl2::texture>,
             const wsdl2::rect& bounds,
             const std::initializer_list<wsdl2::rect>& init = {},
             const std::string& lab = "",
             uint32_t overlap = 1);

    tileset( std::shared_ptr<wsdl2::texture> tex,
             int x = 0, int y = 0,
             const std::initializer_list<wsdl2::rect>& init = {},
             const std::string& lab = "",
             uint32_t overlap = 1)
        : tileset(tex, wsdl2::rect{x, y, 0, 0}, init, lab, overlap) {}

    void map(std::size_t, const wsdl2::rect&);

    void set(std::size_t);
    std::size_t current() const;

    // access by []
    using std::unordered_map<std::size_t, wsdl2::rect>::operator[];

    void clear();

    void erase(std::size_t);

    // size
    using std::unordered_map<std::size_t, wsdl2::rect>::size;
};

}

#endif
