#include "Station.h"
#include <iostream>
#include <string>

static const unsigned int   DEFAULT_STATION_NR = 0;
static const std::string    DEFAULT_STATION_NAME = "unknown-station";

Station::Station() : s_nr{ DEFAULT_STATION_NR }, s_name{ DEFAULT_STATION_NAME } {}
Station::Station(unsigned int _s_nr, std::string _s_name) : s_nr{ _s_nr }, s_name{ _s_name } {}

bool Station::operator==(const Station& otherStation) const
{
    return (s_nr == otherStation.s_nr && s_name == otherStation.s_name);
}

void Station::print() const
{
    std::cout << "[" << s_nr << ", " << s_name << "]";
}