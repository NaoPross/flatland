#ifndef __VECTOR_TPP__
#define __VECTOR_TPP__
	
/* for auto loop */

template<class T>
typename Vector<T>::iterator Vector<T>::begin()
{
    return iterator(this);
}

template<class T>
typename Vector<T>::iterator Vector<T>::end()
{
    return iterator(this, this->size() - 1);
}

template<class T>
typename Vector<T>::const_iterator Vector<T>::begin() const
{
    return iterator(this);
}

template<class T>
typename Vector<T>::const_iterator Vector<T>::end() const
{
    return iterator(this, this->size() - 1);
}

template<class T>
bool Vector<T>::operator==(const Vector<T>& v) const {
	
	if(this->size() != v.size())
		return false;
	
	//Retourne déjà faux si les deux vecteurs n'ont pas la même dimension
	for(size_t i(0); i < v.size(); ++i){
		
		if ((*this)[i] != v[i])
			return false;
			//Lors que le programme rencontre deux coordonnées différenentes respectivement pour la même dimension des deux vecteurs
			//la fonction retourne false
	}
	return true;
}

template<class T>
bool Vector<T>::operator>=(const Vector<T>& v) const
{
    return sq_module() >= v.sq_module();
}

template<class T>
bool Vector<T>::operator<=(const Vector<T>& v) const
{
    return sq_module() <= v.sq_module();
}

template<class T>
bool Vector<T>::operator>=(double m) const
{
    return sq_module() >= m * m;
}

template<class T>
bool Vector<T>::operator<=(double m) const
{
    return sq_module() <= m * m;
}

template<class T>
bool Vector<T>::operator>(const Vector<T>& v) const
{
    return sq_module() > v.sq_module();
}

template<class T>
bool Vector<T>::operator<(const Vector<T>& v) const
{
    return sq_module() < v.sq_module();
}

template<class T>
bool Vector<T>::operator>(double m) const
{
    return sq_module() > m * m;
}

template<class T>
bool Vector<T>::operator<(double m) const
{
    return sq_module() < m * m;
}

template<class T>
bool Vector<T>::operator!=(const Vector<T>& w) const
{
    return !(*this == w);
}

//Operateurs somme et produit

template<class T>
Vector<T>& Vector<T>::add(const Vector<T>& w)
{
    size_t n = (this->size() <= w.size()) ? this->size() : w.size();

    for (size_t i= 0; i < n; ++i)
        (*this)[i] += w[i]; 

    return *this;
}


template<class T>
Vector<T>& Vector<T>::sub(const Vector<T>& w)
{
    size_t n = (this->size() <= w.size()) ? this->size() : w.size();

    for (size_t i = 0; i < n; ++i)
        (*this)[i] -= w[i]; 

    return *this;
}

template<class T>
Vector<T>& Vector<T>::mult(double k)
{
    for (size_t i = 0; i < this->size(); ++i)
        (*this)[i] *= k; 

    return *this;
}

template<class T>
Vector<T>& Vector<T>::divide(double k)
{
    for (size_t i = 0; i < this->size(); ++i)
        (*this)[i] /= k; 

    return *this;
}

template<class T>
Vector<T>& Vector<T>::_unit() const
{
    return (*this) /= this->module();
}

/* Dot product */
template<class T>
double Vector<T>::operator*(const Vector<T>& w) const
{
    double x = 0;
    size_t n = (this->size() <= w.size()) ? this->size() : w.size();

    for (size_t i = 0; i < n; ++i)
        x += (*this)[i] * w[i];

    return x;
}

template<class T>
Vector<T>& Vector<T>::reverse()
{
    return (*this)*=-1.0;
}

template<class T>
double Vector<T>::sq_module() const 
{
    return (*this) * (*this);
}

template<class T>
double Vector<T>::module() const
{
    return sqrt(this->sq_module());
}

/* Implementation of ostream operator overloading */

/* Le prototype je le mets dans un header séparé parce que
 * il ne fait pas partie de l'implementation d'un vecteur
 * Une classe l'utilisant n'as pas besoin de l'imprimer
 */

#ifdef _USE_VECTOR_OSTREAM
#include <ostream>

template<class T>
void Vector<T>::affiche(std::ostream& os) const
{
    os << "(";

    for (size_t i(0); i < (size() - 1); ++i)
    {
        os << (*this)[i] << ", ";
    }

    os << (*this)[size()-1] << ")";
}

// Flux de sortie permettant d'afficher les coordonnées du vecteur
template<class T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& v)
{
    v.affiche(os);
    return os;
}
#endif

#endif //__VECTOR_TPP__
