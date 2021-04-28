#pragma once
#include "Bus.h"
#include "BusHasher.h"
#include <string>
#include <unordered_map>

using busArrivalMinutes = std::vector<unsigned int>;
using schedule = std::unordered_map<Bus, busArrivalMinutes, BusHasher>;

struct Station
{
    unsigned int    s_nr;
    std::string     s_name;
    schedule        s_schedule;

    Station();
    Station(unsigned int, std::string);

    bool operator==(const Station&) const;
    bool operator!=(const Station&) const;

    std::string toString() const;
    void print() const;
    void displayInformation(unsigned int) const;
    void addSchedule(const Bus&, const busArrivalMinutes&);

private:
    unsigned int _getClosestTime(const busArrivalMinutes &, unsigned int) const;
};