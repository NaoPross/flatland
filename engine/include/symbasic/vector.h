#ifndef __VECTOR_H__
#define __VECTOR_H__ 

#ifdef _USE_VECTOR_OSTREAM
#include <iosfwd>
#endif

#include <cstddef>

template <class T>
class Vector {

public:

    typedef std::size_t size_t;
    
    class iterator
    {
        size_t i;
        Vector<T> *v;

    public:

        iterator(Vector<T> *v, size_t i = 0)
            : v(v), i(i)
        {
        }

        iterator& operator++()
        {
            ++i;    
        }

        iterator& operator--()
        {
            --i;
        }

        bool operator!=(const iterator& it)
        {
            return this->i != it.i;
        }

        bool operator==(const iterator& it)
        {
            return !(*this != it);
        }

        T& operator*()
        {
            return (*v)[i];
        }
    };

    typedef const iterator const_iterator;

    /* For auto implementation */
    /* it allows to loop through components
     *
     * for (auto comp : v)
     * {
     *      // loop content
 * }
     */

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;
	
	//Virtual accesseurs

	virtual const size_t size() const = 0;

	/* Accède à la i-ème valeur */
	virtual T& operator[](size_t i) = 0;
	virtual const T& operator[](size_t i) const = 0;

	/* Return the module */
    double module() const;
    double sq_module() const; // squared module
    
    //Operateurs
    
	/* Operateurs de comparaison */
	bool operator==(const Vector<T>&) const;
    bool operator!=(const Vector<T>&) const;

    bool operator>=(const Vector<T>&) const;
    bool operator<=(const Vector<T>&) const;

    bool operator>=(double) const;
    bool operator<=(double) const;

    bool operator>(const Vector<T>&) const;
    bool operator<(const Vector<T>&) const;

    bool operator>(double) const;
    bool operator<(double) const;

    /* Dot product */
    double operator*(const Vector<T>&) const;

    /* For printing a Vector on a ostream */
#ifdef _USE_VECTOR_OSTREAM
    void affiche(std::ostream&) const;
#endif

protected: // not stable operations

    /* Plus / minus */
    Vector<T>& add(const Vector<T>&);
    Vector<T>& sub(const Vector<T>&);

    /* Scalar multiplication / division */
    Vector<T>& mult(double k);
    Vector<T>& divide(double k);

    Vector<T>& reverse();
    Vector<T>& _unit() const;
};

#ifdef _USE_VECTOR_OSTREAM
// forward declaration of ostream

template<class T>
std::ostream& operator<<(std::ostream&, const Vector<T>&);
#endif

#include "vector.tpp"

#endif
