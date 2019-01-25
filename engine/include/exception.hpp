#ifndef __FLAT_ERROR_H__
#define __FLAT_ERROR_H__

#include <exception>

namespace flat {

class FlatException : public std::exception
{
    const char * error;

protected:

    virtual const char * specific() const = 0;

public:

    FlatException(const char* error);
    virtual ~FlatException();

    virtual const char* what() const throw() override;
};

}

#endif
