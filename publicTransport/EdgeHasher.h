#pragma once
#include "Edge.h"

struct EdgeHasher
{
    size_t operator()(const Edge&) const;
};