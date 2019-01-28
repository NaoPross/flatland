#include "exception.hpp"
#include <stdio.h>

using namespace flat;

FlatException::FlatException(const char* error) : m_error(error) {}

FlatException::~FlatException() {}

char buffer[256];

const char* FlatException::what() const throw()
{
    sprintf(buffer, "%s thrown: %s", specific(), m_error);

    return &buffer[0];
}

