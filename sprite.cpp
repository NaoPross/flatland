#include "flatland/sprite.hpp"


using namespace flat;

tileset::tileset(std::shared_ptr<wsdl2::texture> src) : m_texture(src)
{
    emplace(0, wsdl2::rect{0, 0, src->width(), src->height()});
}

tileset::tileset(std::shared_ptr<wsdl2::texture> src, std::size_t cell_w, std::size_t cell_h,
                 std::size_t margin /* = 0 */, std::size_t spacing /* = 0 */)
    : m_texture(src)
{
    const std::size_t cols = (src->width()  - 2 * margin) / (cell_w + spacing);
    const std::size_t rows = (src->height() - 2 * margin) / (cell_h + spacing);

    for (std::size_t i = 0; i < cols; i++) {
        for (std::size_t j = 0; j < rows; j++) {
            // TODO: check in edge cases
            emplace(i + j, wsdl2::rect {
                static_cast<int>(margin + i * (cell_w + spacing)),
                static_cast<int>(margin + j * (cell_h + spacing)),
                static_cast<int>(cell_w),
                static_cast<int>(cell_h)
            });
        }
    }
}

tileset::tileset(std::shared_ptr<wsdl2::texture> src,
                 std::initializer_list<std::pair<unsigned, wsdl2::rect>> viewports)
    : std::unordered_map<unsigned, wsdl2::rect>(viewports.begin(), viewports.end()),
      m_texture(src)
{}


/* class sprite */

sprite::sprite(std::shared_ptr<tileset> tileset,
               trait::projector * proj,
               unsigned index /* = 0 */)
    : m_projector(proj), m_tileset(tileset), m_ts_index(index)
{
    if (proj == nullptr) // by default, set pixel mapping
        m_projector.reset(new trait::proj::pixel_positioner);
}


void sprite::render() const
{
    // TODO: replace with a rectangle calculated using the projection matrix
    //       that will be in trait::renderable
#ifdef DEBUG
    if (m_projector == nullptr)
        return;
#endif

    m_tileset->m_texture->render(
        m_tileset->at(m_ts_index),
        m_projector->projection()
    );
}
