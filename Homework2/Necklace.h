#pragma once
#include "Bead.h"
#include <vector>
#include <string>

struct Necklace {
    std::vector<Bead> beads;

    Necklace();
    Necklace(std::vector<Bead>);
    bool operator==(const Necklace&) const;
    bool operator!=(const Necklace&) const;
    bool operator<(const Necklace&) const;
    bool isEqual(const Necklace&)    const;
    bool isAdjacent(const Necklace&) const;
    char singleBeadColorDifferenceFrom(const Necklace&) const;
    char singleBeadColorDifferenceTo(const Necklace&) const;
    void isValidIndex(size_t);
    void changeBeadColor(size_t, char);
    int firstAppearanceOfBeadColor(char);
    std::vector<int> getIndicesOfBeadColor(char);
    std::string toString() const;
    void printNecklace() const;
};

