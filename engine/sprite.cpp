#include "sprite.hpp"

using namespace flat;

void sprite::render()
{
    // TODO, render texture
}

wsdl2::texture& sprite::texture()
{
    return m_texture;
}

const wsdl2::texture& sprite::texture() const
{
    return m_texture;
}

void sprite::set_focused(bool flag)
{
    // TODO, send signal: focus taken
    m_focused = flag;
}

bool sprite::focused() const
{
    return m_focused;
}

