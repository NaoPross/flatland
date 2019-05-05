#include "entity.hpp"

using namespace flat;

entity::entity(vector_type pos, vector_type bound)
    : m_pos(pos),
      m_bound_shape(bound_shape::rectangle),
      m_bound(bound)
{}

std::pair<entity::bound_shape, std::variant<unsigned, entity::vector_type>>
entity::bound() const
{
    return std::make_pair(m_bound_shape, m_bound);
}

bool entity::collides(const entity&) const 
{
    // TODO implementation
    npdebug("warning: this method is not implemented");
    return false;
}


wsdl2::rect entity::rect() const
{
    if (m_bound_shape == bound_shape::rectangle) {
        return wsdl2::rect {
            m_pos.x(),
            m_pos.y(),
            std::get<vector_type>(m_bound).x(),
            std::get<vector_type>(m_bound).y()
        };
    } else {
        // TODO: implement for sphere and points
        return wsdl2::rect { 0, 0, 0, 0 };
    }
}