#pragma once
#include "Bus.h"

struct BusHasher
{
    size_t operator()(const Bus&) const;
};