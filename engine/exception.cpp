#include "exception.hpp"
#include <stdio.h>

FlatException::FlatException(const char* error) : error(error) {}

FlatException::~FlatException() {}

char buffer[256];

const char* FlatException::what() const throw()
{
    sprintf(buffer, "%s thrown: %s", specific(), error);

    return &buffer[0];
}

