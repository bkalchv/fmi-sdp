#pragma once
#include "Station.h"

static const size_t DEFAULT_EDGE_WEIGHT = 0;

struct Edge
{
    Station toStation;
    size_t  weight;

    Edge(Station, size_t _weight = DEFAULT_EDGE_WEIGHT);

    bool operator==(const Edge&) const;
};