#include "EdgeHasher.h"
#include <iostream>

size_t EdgeHasher::operator()(const Edge& e) const
{
    return std::hash<unsigned int>()(e.toStation.s_nr);
}