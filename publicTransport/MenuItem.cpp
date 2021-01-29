#include "MenuItem.h"

MenuItem::MenuItem(std::string number, std::string action) : mi_number{number}, mi_action{ action } {}

MenuItem::MenuItem(const MenuItem& other)
{
    this->mi_number = other.mi_number;
    this->mi_action = other.mi_action;
}

MenuItem& MenuItem::operator=(const MenuItem& other)
{
    this->mi_number = other.mi_number;
    this->mi_action = other.mi_action;

    return *this;
}

std::string MenuItem::getNumber() const
{
    return mi_number;
}

void MenuItem::setNumber(std::string number)
{
    this->mi_number = number;
}

std::string MenuItem::getAction() const
{
    return mi_action;
}

void MenuItem::setAction(std::string action)
{
    this->mi_action = action;
}

bool MenuItem::match(const std::string& choice) const
{
    return choice == this->mi_number;
}
