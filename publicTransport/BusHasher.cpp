#include "BusHasher.h"
#include <iostream>
#include <string>

size_t BusHasher::operator()(const Bus& b) const
{
    return std::hash<unsigned int>()(b.b_nr) ^ (std::hash<unsigned int>()(b.b_name.size() << 1));
}