#include "Necklace.h"
#include <stdexcept>
#include <iostream>

Necklace::Necklace() {
    beads = std::vector<Bead>{ '!' };
}

Necklace::Necklace(std::vector<Bead> _beads) {
    for (Bead b : _beads)
    {
        beads.push_back(Bead(b.color));
    }
}

bool Necklace::operator==(const Necklace& other_necklace) const
{
    if (beads.size() == other_necklace.beads.size()) {
        for (size_t i{ 0 }; i < beads.size(); ++i)
        {
            if (other_necklace.beads[i].color != beads[i].color)
            {
                return false;
            }
        }

        return true;
    }

    return false;
}

bool Necklace::operator!=(const Necklace& other_necklace) const
{
    return !(*this == other_necklace);
}

bool Necklace::operator<(const Necklace& other_necklace) const {
    return this->toString() < other_necklace.toString();
}

bool Necklace::isEqual(const Necklace& other_necklace) const {
    return *this == other_necklace;
}

bool Necklace::isAdjacent(const Necklace& other_necklace) const {
    if (beads.size() == other_necklace.beads.size() && !isEqual(other_necklace))
    {
        size_t amountDifferentBeads{ 0 };
        for (size_t i{ 0 }; i < beads.size(); ++i)
        {
            if ((int)beads[i].color != (int)other_necklace.beads[i].color)
            {
                ++amountDifferentBeads;
            }
        }

        if (amountDifferentBeads == 1)
        {
            return true;
        }
        else
        {
            return false;
        }

    }

    return false;
}

char Necklace::singleBeadColorDifferenceFrom(const Necklace& other_necklace) const
{
    for (size_t i{ 0 }; i < beads.size(); ++i)
    {
        char fromColor = beads[i].color;
        char toColor = other_necklace.beads[i].color;
        if (fromColor != toColor)
        {
            return fromColor;
        }
    }
}

char Necklace::singleBeadColorDifferenceTo(const Necklace& other_necklace) const {
    for (size_t i{ 0 }; i < beads.size(); ++i)
    {
        char fromColor  = beads[i].color;
        char toColor    = other_necklace.beads[i].color;
        if (fromColor != toColor)
        {
            return toColor;
        }
    }
}

void Necklace::isValidIndex(size_t idx) {
    if (idx > beads.size()) 
        throw std::invalid_argument("Invalid index!");
}

void Necklace::changeBeadColor(size_t at_idx, char toColor) {
    isValidIndex(at_idx);
    Bead::validateColor(toColor);
    beads[at_idx].color = toColor;
}

int Necklace::firstAppearanceOfBeadColor(char color) {

    for (size_t i{ 0 }; i < beads.size(); ++i)
    {
        if (beads[i].color == color)
        {
            return i;
        }
    }

    return -1;
}

std::vector<int> Necklace::getIndicesOfBeadColor(char color) {
    std::vector<int> result;

    if (firstAppearanceOfBeadColor(color) != -1)
    {
        for (int i{ firstAppearanceOfBeadColor(color) }; i < beads.size(); ++i) {
            if (beads[i].color == color) {
                result.push_back(i);
            }
        }
    }

    return result;
}

std::string Necklace::toString() const
{
    std::string result;
    for (const Bead& b : this->beads)
    {
        result += b.color;
    }
    return result;
}

void Necklace::printNecklace() const {
    for (size_t i{ 0 }; i < beads.size(); ++i)
    {
        if (i == beads.size() - 1)
        {
            std::cout << beads[i].color;
            continue;
        }
        std::cout << beads[i].color << "-";
    }
}