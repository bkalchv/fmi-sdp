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

bool Station::operator!=(const Station& other) const
{
    return !this->operator==(other);
}

std::string Station::toString() const
{
    return  std::to_string(this->s_nr).append("-").append(this->s_name);
}

void Station::print() const
{
    std::cout << "[" << s_nr << ", " << s_name << "]";
}

void Station::displayInformation(unsigned int timeInMinutes) const
{
    this->print(); std::cout << " at [" << timeInMinutes << "]: " << std::endl;
    for (const std::pair<Bus, busArrivalMinutes>& p : s_schedule)
    {
        unsigned int closestTime = _getClosestTime(p.second, timeInMinutes);
        std::cout << " [" << p.first.b_name <<  "]";
        if (closestTime == timeInMinutes)
        {
            std::cout << " ";
        }
        else
        {
            std::cout << " +";
        }
        std::cout << (closestTime - timeInMinutes) << "\n";
    }
    std::cout << std::endl;
}

void Station::addSchedule(const Bus& b, const busArrivalMinutes& arrivalMinutes)
{
    this->s_schedule[b] = arrivalMinutes;
}

unsigned int Station::_getClosestTime(const busArrivalMinutes& arrivalMinutes, unsigned int timeInMinutes) const
{   
    for (unsigned int arrivalTime : arrivalMinutes)
    {
        if ((int)arrivalTime - (int)timeInMinutes >= 0)
        {
            return arrivalTime;
        }
    }
}
