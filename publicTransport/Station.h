#pragma once
#include "Bus.h"
#include <string>
#include <list> 
#include <unordered_map>

using busArrivalMinutes = std::list<unsigned int>;
using schedule = std::unordered_map<busNr, busArrivalMinutes>;

struct Station
{
    unsigned int    s_nr;
    std::string     s_name;
    std::list<Bus>  s_busesList; // might be better to only have a schedule 
    schedule        s_schedule;

    Station();
    Station(unsigned int, std::string);

    bool operator==(const Station&) const;

    void print() const;
};