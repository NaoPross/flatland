#ifndef __FLATSPRITE_H__
#define __FLATSPRITE_H__

#include "renderbase.hpp"
#include "wsdl2/video.hpp"
#include "mm/mmvec.hpp"
#include <list>

namespace flat {

namespace texloader
{
     /*
     * Get a texture, from a specified path.
     * If the texture was not loaded, then
     * it returns a new loaded texture.
     * In case loading fails it returns a nullptr
     *
     * Warning: making the texture go out of scope
     *          implies that it will be freed and 
     *          no more available.
     *          A texture persistence depends on
     *          how many instances of this texture
     *          are present.
     *
     * In case the texture already exists the pixelformat and the access
     * parameters are ignored
     */
    std::shared_ptr<wsdl2::texture> get(const std::string& path);

    std::shared_ptr<wsdl2::texture> create( const std::string& name, // needed for mapping
                                            std::size_t width, 
                                            std::size_t height,
                                            wsdl2::pixelformat::format p = wsdl2::pixelformat::format::unknown, 
                                            wsdl2::texture::access a = wsdl2::texture::access::static_);
}

/*
 *  Textured sprite, used for static rendering
 */
class sprite : virtual public renderbase
{
    // location and rectangular size, in pixel
    wsdl2::rect m_bounds;

    // source viewport 
    wsdl2::rect m_viewport;

protected:

    std::shared_ptr<wsdl2::texture> m_texture;

public:

    /*
     * Construct a sprite basing on a loaded texture 
     * Passing a null texture will throw an exception
     */
    sprite( std::shared_ptr<wsdl2::texture>,
            const wsdl2::rect& bounds,
            const wsdl2::rect& viewport, 
            const std::string& lab = "",
            uint32_t overlap = 1);
    
    /*
     * Sprite bounds are the same as the image bounds:
     *
     * Bounds = (0, 0, img-width, img-height)
     */
    sprite( std::shared_ptr<wsdl2::texture>,
            int x, int y,
            const wsdl2::rect& viewport, 
            const std::string& lab = "",
            uint32_t overlap = 1);

    /*
     * Viewport and bounds are deduced from the image size, precisely:
     *
     * Viewport = (0, 0, img-width, img-height)
     * Bounds = (x, y, img-width, img-height)
     */
    sprite( std::shared_ptr<wsdl2::texture>,
            int x = 0, int y = 0,
            const std::string& lab = "",
            uint32_t overlap = 1);

    sprite( std::shared_ptr<wsdl2::texture>,
            const wsdl2::rect& bounds,
            const std::string& lab = "",
            uint32_t overlap = 1);

    ~sprite() {}

    struct focus_call
    {
        focus_call(sprite *s) : sender(s) {}

        sprite * sender;
    };

    // TODO, is it useful?
    // send a focus signal (destinated to the owner actor)
    void focus();

    // bounds accessors
    void set_location(int x, int y);
    void set_location(const mm::vec2<int>&);

    void set_width(std::size_t width);
    void set_height(std::size_t height);
    void set_size(std::size_t width, std::size_t height);

    void set_bounds(const wsdl2::rect&);

    mm::vec2<int> location() const;

    std::size_t width() const;
    std::size_t height() const;
    
    wsdl2::rect bounds();
    const wsdl2::rect& bounds() const;

    // viewport accessors
    void set_viewport(const wsdl2::rect&);

    wsdl2::rect viewport();
    const wsdl2::rect& viewport() const;

    // TODO add other features
    //
    // TODO flip angle?
    
    // render the texture
    virtual void render() override;
};

/*
 * A sprite that automaticly maps the tileset by indexes
 *
 * A zero value must always exist, then clearing the map
 * means to reset to the default viewport
 */
class tileset : public sprite, private std::unordered_map<std::size_t, wsdl2::rect>
{
    // privatize viewport accessors
    using sprite::set_viewport;
    using sprite::viewport;

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

    tileset( std::shared_ptr<wsdl2::texture>,
             int x = 0, int y = 0,
             const std::initializer_list<wsdl2::rect>& init = {},
             const std::string& lab = "",
             uint32_t overlap = 1);

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
