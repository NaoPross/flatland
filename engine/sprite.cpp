#include "sprite.hpp"


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


sprite::sprite(vector_type pos, std::shared_ptr<tileset> tileset,
        unsigned tileset_index /* = 0 */)
    : renderable(),
      entity(pos, {
          tileset->at(tileset_index).w / 2,
          tileset->at(tileset_index).h / 2
      }),
      m_tileset(tileset),
      m_tileset_index(tileset_index)
{}

void sprite::render()
{
    auto r = rect();

    m_tileset->m_texture->render(
        m_tileset->at(m_tileset_index), r
    );
}
