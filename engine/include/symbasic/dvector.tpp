#ifndef __DVECTOR_TPP__
#define __DVECTOR_TPP__

template<class T>
DVector<T>::DVector(size_t dim) 
    : components(dim)
{

}

template<class T>
DVector<T>::DVector(const std::vector<T>& init)
    : components(init)
{

}

template<class T>
DVector<T>::DVector(const std::initializer_list<T> &init)
    : components(init.begin(), init.end())
{

}

template<class T>
const size_t DVector<T>::size() const
{
    return components.size();
}

template<class T>
T& DVector<T>::operator[](size_t i)
{
    return components[i];
}

template<class T>
const T& DVector<T>::operator[](size_t i) const
{
    return components[i];
}

template<class T>
DVector<T>& DVector<T>::operator<<(const T &value)
{
    components.push_back(value);
}

template<class T>
DVector<T>& DVector<T>::operator+=(const Vector<T> &w)
{
    return static_cast<DVector<T>&>(this->add(w));
}

template<class T>
DVector<T>& DVector<T>::operator-=(const Vector<T> &w)
{
    return static_cast<DVector<T>&>(this->sub(w));
}

template<class T>
DVector<T>& DVector<T>::operator*=(double k)
{
    return static_cast<DVector<T>&>(this->mult(k));
}

template<class T>
DVector<T>& DVector<T>::operator/=(double k)
{
    return static_cast<DVector<T>&>(this->divide(k));
}

template<class T>
DVector<T>& DVector<T>::operator~()
{
    return static_cast<DVector<T>&>(this->reverse());
}

template<class T>
DVector<T>& DVector<T>::unit()
{
    return static_cast<DVector<T>&>(this->_unit());
}

template<class T>
const DVector<T> operator-(DVector<T> v)
{
    return ~v;
}

template<class T>
const DVector<T> operator+(DVector<T> v, const Vector<T>& w)
{
    return v += w;
}

template<class T>
const DVector<T> operator-(DVector<T> v, const Vector<T>& w)
{
    return v -= w;
}

template<class T>
const DVector<T> operator*(DVector<T> v, double k)
{
    return v *= k;
}

template<class T>
const DVector<T> operator/(DVector<T> v, double k)
{
    return v /= k;
}

template<class T>
const DVector<T> operator*(double k, DVector<T> v)
{
    return v *= k;
}

template<class T>
const DVector<T> operator/(double k, DVector<T> v)
{
    return v /= k;
}

inline const DVector<double> operator^(const DVector<double>& v, const Vector<double>& w)
{
    DVector<double> u;

    size_t n = (v.size() <= w.size()) ? v.size() : w.size();

    if (n > 2)
    {
        u << v[1] * w[2] - v[2] * w[1];
        u << v[2] * w[0] - v[0] * w[2];
        u << v[0] * w[1] - v[1] * w[0];

    } else if (n == 2)
        u << v[0] * w[1] - v[1] * w[0];
    else if (n == 1)
        u << v[0] * w[0];

    return u;
}

#endif
