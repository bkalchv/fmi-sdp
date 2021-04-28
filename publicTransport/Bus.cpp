#include "Bus.h"

Bus::Bus() : b_nr{ DEFAULT_BUS_NR } {}

Bus::Bus(busNr _b_nr, std::string _b_name) : b_nr{ _b_nr }, b_name{_b_name} {}

bool Bus::operator==(const Bus& other) const
{
    return (b_nr == other.b_nr) && (b_name == other.b_name);
}

bool Bus::operator!=(const Bus& other) const
{
    return !this->operator==(other);
}

void Bus::print() const
{
    std::cout << b_name << " : ";
    for (unsigned int busStationNr : b_route)
    {
        std::cout << busStationNr << " ";
    }
    std::cout << std::endl;
}
