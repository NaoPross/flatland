#ifndef __SVECTOR_TPP__
#define __SVECTOR_TPP__

template<class T, std::size_t N>
SVector<T, N>::SVector(const T &value)
{
    components.fill(value);
}

template<class T, std::size_t N>
SVector<T, N>::SVector(const std::initializer_list<T> &init)
{
    size_t n = (init.size() < N) ? init.size() : N;

    /* copy init list without include <algorithm> */
    size_t index = 0;

    /* copy the initializer list until its size */
    for (auto c : init)
    {
        if (index >= n)
            break;

        components[index++] = c;
    }

    /* fill the remaining components with zero */
    while(index < N)
        components[index++] = 0;
}

template<class T, std::size_t N>
SVector<T, N>::SVector(const SVector<T, N> &cpy)
{
    this->components = cpy.components;
}

template<class T, std::size_t N>
SVector<T, N>& SVector<T, N>::operator=(const SVector<T, N> &v)
{
    this->components = v.components;
}

template<class T, std::size_t N>
const size_t SVector<T, N>::size() const
{
    return N;
}

/* Access operators */
template<class T, std::size_t N>
T& SVector<T, N>::operator[](size_t i)    
{ 
    return components[i]; 
}

template<class T, std::size_t N>
const T& SVector<T, N>::operator[](size_t i) const 
{ 
    return components[i]; 
}

template<class T, std::size_t N>
SVector<T, N>& SVector<T, N>::operator+=(const Vector<T> &w)
{
    return static_cast<SVector<T, N>&>(this->add(w));
}

template<class T, std::size_t N>
SVector<T, N>& SVector<T, N>::operator-=(const Vector<T> &w)
{
    return static_cast<SVector<T, N>&>(this->sub(w));
}

template<class T, std::size_t N>
SVector<T, N>& SVector<T, N>::operator*=(double k)
{
    return static_cast<SVector<T, N>&>(this->mult(k));
}

template<class T, std::size_t N>
SVector<T, N>& SVector<T, N>::operator/=(double k)
{
    return static_cast<SVector<T, N>&>(this->divide(k));
}

template<class T, std::size_t N>
SVector<T, N>& SVector<T, N>::operator~()
{
    return static_cast<SVector<T, N>&>(this->reverse());
}

template<class T, std::size_t N>
SVector<T, N>& SVector<T, N>::unit()
{
    return static_cast<SVector<T, N>&>(this->_unit());
}

template<class T, std::size_t N>
const SVector<T, N> operator+(SVector<T, N> v, const Vector<T>& w)
{
    return v += w;
}

template<class T, std::size_t N>
const SVector<T, N> operator-(SVector<T, N> v, const Vector<T>& w)
{
    return v -= w;
}

template<class T, std::size_t N>
const SVector<T, N> operator*(SVector<T, N> v, double k)
{
    return v *= k;
}

template<class T, std::size_t N>
const SVector<T, N> operator/(SVector<T, N> v, double k)
{
    return v /= k;
}

template<class T, std::size_t N>
const SVector<T, N> operator*(double k, SVector<T, N> v)
{
    return v *= k;
}

template<class T, std::size_t N>
const SVector<T, N> operator/(double k, SVector<T, N> v)
{
    return v /= k;
}

template<class T, std::size_t N>
const SVector<T, N> operator-(SVector<T, N> v)
{
    return ~v;
}

inline const SVector<double, 3> operator^(const SVector<double, 3>& v, const SVector<double, 3>& w)
{
    SVector<double, 3> u = {0, 0, 0};

    u[0] = v[1] * w[2] - v[2] * w[1];
    u[1] = v[2] * w[0] - v[0] * w[2];
    u[2] = v[0] * w[1] - v[1] * w[0];

    return u;
}

inline double operator^(const SVector<double, 2>& v, const SVector<double, 2>& w)
{
    return v[0] * w[1] - v[1] * w[0];
}

inline double operator^(const SVector<double, 1>& v, const SVector<double, 1>& w)
{
    return v[0] * w[0];
}

#endif
