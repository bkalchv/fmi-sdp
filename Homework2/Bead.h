#pragma once
#include <stdexcept>



struct Bead
{
    char color;

    Bead(char);
    static bool isValidColor(char c) 
    {
        return ((int)c >= 33 && (int)c <= 126);
    }
    static void validateColor(char c) 
    {
        if ((int)c < 33 && (int)c > 126)
        {
            throw std::invalid_argument("Invalid color! ASCII of char < 33 or > 126.");
        }
    }
};

