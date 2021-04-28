#include "StationHasher.h"
#include <iostream>
#include <string>

size_t StationHasher::operator()(const Station& s) const
{
    return std::hash<unsigned int>()(s.s_nr) ^ (std::hash<unsigned int>()(s.s_name.size() << 1));
}
