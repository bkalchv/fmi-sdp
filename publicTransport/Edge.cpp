#include "Edge.h"
#include "Station.h"



Edge::Edge(Station _toStation, size_t _weight) : toStation{ Station(_toStation.s_nr, _toStation.s_name) }, weight{ _weight }  {}

bool Edge::operator==(const Edge& otherStation) const
{
    return (toStation == otherStation.toStation && weight == otherStation.weight);
}