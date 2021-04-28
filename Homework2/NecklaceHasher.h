#pragma once
#include "Necklace.h"
struct NecklaceHasher
{
    size_t operator() (const Necklace&) const;
};