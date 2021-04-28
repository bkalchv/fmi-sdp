#include <iostream>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include "Bead.h"
#include "Necklace.h"

using namespace std;

vector<Necklace> generateAllPossibilitiesVector(Necklace necklace, char fromColor, char toColor) {
    vector<Necklace> result;

    while (necklace.firstAppearanceOfBeadColor(fromColor) != -1)
    {
        vector<int> indicesOfAppearance = necklace.getIndicesOfBeadColor(fromColor);
        for (const size_t& idx : indicesOfAppearance)
        {
            Necklace copyNecklace = necklace;
            vector<Bead> newNecklaceBeads;
            copyNecklace.changeBeadColor(idx, toColor);
            for (const Bead& bead : copyNecklace.beads)
            {
                newNecklaceBeads.push_back(Bead(bead.color));
            }
            Necklace newNecklace = Necklace(newNecklaceBeads);
            result.push_back(newNecklace);
        }
        necklace.changeBeadColor(necklace.firstAppearanceOfBeadColor(fromColor), toColor);
    }

    return result;
}

using Edge = pair<Necklace, int>;
using Graph = std::unordered_map<Necklace, unordered_set<Edge>>;

int main()
{
    Necklace necklace1 = Necklace({ Bead('A'), Bead('b'), Bead('c'), Bead('d'), Bead('d') });
    Necklace necklace2 = Necklace({ Bead('a'), Bead('b'), Bead('c'), Bead('d'), Bead('f') });

    vector<Necklace> possibilities1 = generateAllPossibilitesVector(necklace1);
    vector<Necklace> posibilities2 = generateAllPossibilitesVector(necklace2);

    cout << "Possibilities for necklace 1:" << endl;
    for (const Necklace& n : possibilities1)
    {
        cout << " "; n.printNecklace(); cout << endl;
    }

    cout << "Possibilities for necklace 2:" << endl;
    for (const Necklace& n : possibilities2)
    {
        cout << " "; n.printNecklace(); cout << endl;
    }

	return 0;
}
