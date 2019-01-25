#ifndef __STATIC_VECTOR_H__
#define __STATIC_VECTOR_H__

#include <array>
#include "vector.h"

template<class T, std::size_t N> 
class SVector : public Vector<T>
{

public:

    /* Constructors */

    SVector(const T &value = T());
    SVector(const std::initializer_list<T> &init);
    SVector(const SVector<T, N>&);

    SVector& operator=(const SVector<T, N>&);

	virtual const size_t size() const override;

	virtual T& operator[](size_t i) override;
	virtual const T& operator[](size_t i) const override;

    /* Plus / minus */
    SVector<T, N>& operator+=(const Vector<T>&);
    SVector<T, N>& operator-=(const Vector<T>&);

    /* Scalar multiplication / division */
    SVector<T, N>& operator*=(double k);
    SVector<T, N>& operator/=(double k);

    /* Reverse operator */
    SVector<T, N>& operator~();

    /* Unit vector */
    SVector<T, N>& unit();

private:

    std::array<T, N> components;
};

template<class T, std::size_t N>
const SVector<T, N> operator+(SVector<T, N> , const Vector<T>& );

template<class T, std::size_t N>
const SVector<T, N> operator-(SVector<T, N> , const Vector<T>& );

template<class T, std::size_t N>
const SVector<T, N> operator*(SVector<T, N> , double);

template<class T, std::size_t N>
const SVector<T, N> operator/(SVector<T, N> , double);

template<class T, std::size_t N>
const SVector<T, N> operator*(double, SVector<T, N>);

template<class T, std::size_t N>
const SVector<T, N> operator/(double, SVector<T, N>);

template<class T, std::size_t N>
const SVector<T, N> operator-(SVector<T, N>);

/*
 * Prototipe for cross product
 */
inline const SVector<double, 3> operator^(const SVector<double, 3>& v, const SVector<double, 3>& w);
inline double operator^(const SVector<double, 2>& v, const SVector<double, 2>& w);
inline double operator^(const SVector<double, 1>& v, const SVector<double, 1>& w);

/*
 * Include definitions file
 */
#include "svector.tpp"

#endif // __SVECTOR_H__
