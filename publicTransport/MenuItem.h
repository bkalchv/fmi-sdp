#pragma once
#include <string>

class MenuItem
{
private:
	std::string mi_number;
	std::string mi_action;

public:
	MenuItem(std::string, std::string);
    MenuItem(const MenuItem&);
    MenuItem& operator=(const MenuItem&);

    std::string getNumber() const;
    void setNumber(std::string);
    std::string getAction() const;
    void setAction(std::string);
    bool match(const std::string&) const;
};

