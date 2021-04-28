#include "NecklaceHasher.h"

size_t NecklaceHasher::operator()(const Necklace& necklace) const
{
	return std::hash<std::string>()(necklace.toString());
}