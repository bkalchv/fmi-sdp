#pragma once
#include <forward_list>

using busNr = unsigned int;

static const busNr          DEFAULT_BUS_NR = 0;

struct Bus
{
    busNr b_nr;
    std::forward_list<unsigned int> b_route; // fwd_list<bus_station_numbers>

    Bus();
    Bus(busNr);
};