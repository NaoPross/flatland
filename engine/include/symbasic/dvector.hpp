#ifndef __DVECTOR_H__
#define __DVECTOR_H__

#include <vector>
#include <initializer_list>

#include "vector.h"

template<class T>
class DVector : public Vector<T>
{

public :

    DVector(size_t dim = 0);
	DVector(const std::vector<T>&);
    DVector(const std::initializer_list<T> &init);

	virtual const size_t size() const override;

	virtual T& operator[](size_t i) override;
	virtual const T& operator[](size_t i) const override;

    DVector& operator<<(const T&);

    /* Plus / minus */
    DVector<T>& operator+=(const Vector<T>&);
    DVector<T>& operator-=(const Vector<T>&);

    /* Scalar multiplication / division */
    DVector<T>& operator*=(double k);
    DVector<T>& operator/=(double k);

    /* Reverse operator */
    DVector<T>& operator~();

    /* Unit vector */
    DVector<T>& unit();
    
private:

    std::vector<T> components; 
};

template<class T>
const DVector<T> operator+(DVector<T> , const Vector<T>& );

template<class T>
const DVector<T> operator-(DVector<T> , const Vector<T>& );

template<class T>
const DVector<T> operator*(DVector<T> , double);

template<class T>
const DVector<T> operator/(DVector<T> , double);

/*template<class T>
const DVector<T> operator+(const Vector<T>&, DVector<T>);

template<class T>
const DVector<T> operator-(const Vector<T>&, DVector<T>);*/

template<class T>
const DVector<T> operator*(double, DVector<T>);

template<class T>
const DVector<T> operator/(double, DVector<T>);


template<class T>
const DVector<T> operator-(DVector<T>);

inline const DVector<double> operator^(const DVector<double>& v, const Vector<double>&);

/*
 * Include definitions file
 */
#include "dvector.tpp"

#endif
