#pragma once

#include <wsdl2/video.hpp>
#include <mm/mmvec.hpp>

namespace flat::trait {

struct projector
{
    virtual ~projector() = default;

    virtual wsdl2::rect projection() const = 0;
};

namespace proj {

// TODO, implement matrix transformation
// templating on a mm::matrix
// and do nothing when the matrix is identity
template <typename Vector>
struct positioner : public projector
{
    positioner(const Vector& pos, const Vector& s)
        : position(pos), size(s) {}

    Vector position;
    Vector size;

    virtual void move(const Vector& v) { position += v; }
    virtual void move(Vector&& v) { position += v; }

    virtual wsdl2::rect projection() const override {
        return wsdl2::rect{position[0],
                           position[1],
                           size[0],
                           size[1]};
    }
};

struct pixel_positioner : public positioner<mm::vec2<int>>
{
    // set default values
    pixel_positioner(const mm::vec2<int>& pos = {0, 0}, 
                     const mm::vec2<int>& s = {0, 0})
        : positioner<mm::vec2<int>>(pos, s) {}
};

// Fullscreen placer
struct fullscreen : public projector
{
    fullscreen(const wsdl2::window& win) : m_window(win) {}

    virtual wsdl2::rect projection() const override {

        auto size = m_window.size();
        return wsdl2::rect{0, 0, size.x, size.y};
    }

private:
    const wsdl2::window& m_window;
};

}

}
