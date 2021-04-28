#pragma once
#include "Station.h"

struct StationHasher
{
    size_t operator()(const Station&) const;
};