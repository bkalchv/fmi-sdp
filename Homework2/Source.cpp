#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include "Bead.h"
#include "Necklace.h"
#include "NecklaceHasher.h"
#include "Graph.h"

using namespace std;

size_t readChangeableColorsAmount()
{

    cout << "How many are the changeable colors?" << endl;

    size_t amount;
    cin >> amount;
    while (amount < 0)
    {
        cout << "Invalid amount of changeable color(s) input. Try again!" << endl;
        cin >> amount;
    }

    return amount;
}

vector<tuple<char, char, int>> changeableColorsReader()
{ 
    size_t changeableColorsAmount = readChangeableColorsAmount();
    vector<tuple<char, char, int>> changeableColors(changeableColorsAmount);

    for (size_t i{0}; i < changeableColors.size(); ++i)
    {

        cout << "Please input the initial color of the bead: ";

        char fromColorInput;
        cin >> fromColorInput;
        while (!(Bead::isValidColor(fromColorInput)))
        {
            cout << "Invalid color input. Try again!" << endl;
            cin >> fromColorInput;
        }

        cout << "Please input the result color of the bead: "

        char toColorInput;
        cin >> toColorInput;
        while (!(Bead::isValidColor(toColorInput)))
        {
            cout << "Invalid color input. Try again!" << endl;
            cin >> toColorInput;
        }

        cout << "Please input the cost of the transition: "

        int cost;
        cin >> cost;
        while (cost < 0)
        {
            cout << "Invalid cost input. Try again!" << endl;
            cin >> cost;
        }

        tuple<char, char, int> currentTuple = make_tuple(fromColorInput, toColorInput, cost);
        changeableColors[i] = currentTuple;
    }

    return changeableColors;
}

bool fromColorExists(char fromColor, const vector<tuple<char, char, int>>& changeableColors)
{
    for (const tuple<char, char, int>& tuple : changeableColors)
    {
        if (get<0>(tuple) == fromColor)
        {
            return true;
        }
    }
    return false;
}

bool toColorExists(char toColor, const vector<tuple<char, char, int>>& changeableColors)
{
    for (const tuple<char, char, int>& tuple : changeableColors)
    {
        if (get<1>(tuple) == toColor)
        {
            return true;
        }
    }
    return false;
}

bool isValidTransition(char fromColor, char toColor, const vector<tuple<char, char, int>>& changeableColors)
{
    return fromColorExists(fromColor, changeableColors) && toColorExists(toColor, changeableColors);
}

int getCostOfColorTransition(char from, char to, const vector<tuple<char, char, int>>& changeableColors)
{
    for (size_t i{ 0 }; i < changeableColors.size(); ++i)
    {
        tuple<char, char, int> currentTuple = changeableColors[i];
        if (get<0>(currentTuple) == from && get<1>(currentTuple) == to)
        {
            return get<2>(currentTuple);
        }
    }
}


vector<Necklace> generateAllPossibilitiesVectorFromTo(Necklace necklace, char fromColor, char toColor) {
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

unordered_set<Necklace, NecklaceHasher> generateAllPossibilities(Necklace necklace, const vector<tuple<char, char, int>>& changeableColors)
{
    unordered_set<Necklace, NecklaceHasher> result;

    Necklace copyNecklace = necklace;

    result.insert(necklace);

    for (size_t i{ 0 }; i < changeableColors.size(); ++i)
    {
        tuple<char, char, int> currentTuple = changeableColors[i];
        char fromColor  = get<0>(currentTuple);
        char toColor    = get<1>(currentTuple);
        int  cost       = get<2>(currentTuple);

        vector<Necklace> possibilities = generateAllPossibilitiesVectorFromTo(copyNecklace, fromColor, toColor);
        if (!possibilities.empty())
        {
            for (const Necklace& n : possibilities)
            {
                result.insert(n);
            }
        }

        while (copyNecklace.firstAppearanceOfBeadColor(fromColor) != -1)
        {
            copyNecklace.changeBeadColor(copyNecklace.firstAppearanceOfBeadColor(fromColor), toColor);
        }

        for (size_t j{ i + 1 }; j < changeableColors.size(); ++j) // every other possible color transition
        {
            tuple<char, char, int> anotherTuple = changeableColors[j];
            char anotherFromColor   = get<0>(anotherTuple);
            char anotherToColor     = get<1>(anotherTuple);
            int  anotherCost        = get<2>(anotherTuple);

            if (isValidTransition(anotherFromColor, anotherToColor, changeableColors))
            {
                vector<Necklace> possibilities1 = generateAllPossibilitiesVectorFromTo(necklace, anotherFromColor, anotherToColor);
                if (!possibilities1.empty())
                {
                    for (const Necklace& n : possibilities1)
                    {
                        result.insert(n);
                    }
                }
            }
        }
    }

    return result;
}

set<Necklace> generateVertices(Necklace necklace1, Necklace necklace2, const vector<tuple<char, char, int>>& changeableColors)
{
    set<Necklace> result;

    unordered_set<Necklace, NecklaceHasher> possibilities1 = generateAllPossibilities(necklace1, changeableColors);
    for (const Necklace& n : possibilities1)
    {
        result.insert(n);
    }

    unordered_set<Necklace, NecklaceHasher> possibilities2 = generateAllPossibilities(necklace2, changeableColors);
    for (const Necklace& n : possibilities2)
    {
        result.insert(n);
    }

    return result;
}

unordered_map<Necklace, size_t, NecklaceHasher> generateVerticesMap(const set<Necklace>& vertices)
{
    unordered_map<Necklace, size_t, NecklaceHasher> result;
    size_t index{ 0 };
    for (const Necklace& necklace : vertices)
    {
        result[necklace] = index;
        ++index;
    }
    return result;
}

void initGraph(Graph& graph, const set<Necklace>& vertices, const unordered_map<Necklace, size_t, NecklaceHasher>& verticesToIndex, const vector<tuple<char, char, int>>& changeableColors)
{
    for (const Necklace& necklace : vertices)
    {
        for (const Necklace& otherNecklace : vertices)
        {
            char  fromColor = necklace.singleBeadColorDifferenceFrom(otherNecklace);
            char  toColor = necklace.singleBeadColorDifferenceTo(otherNecklace);
            if (necklace != otherNecklace && necklace.isAdjacent(otherNecklace) && isValidTransition(fromColor, toColor, changeableColors))
            {

                size_t  fromIndex = verticesToIndex.at(necklace);
                size_t  toIndex = verticesToIndex.at(otherNecklace);
                int     cost = getCostOfColorTransition(fromColor, toColor, changeableColors);
                graph.addEdge(fromIndex, toIndex, cost);

                /* cout << "Edge from "; necklace.printNecklace(); cout << " to "; otherNecklace.printNecklace(); cout << " with the cost of " << cost << " added." << endl;
                 cout << "[singleBeadColorDifference = "  << fromColor << " to " << toColor << "]" << endl;*/
            }
        }
    }
}

int main()
{
    vector<tuple<char, char, int>> changeableColors = changeableColorsReader();

    Necklace necklace1 = Necklace({ Bead('A'), Bead('b'), Bead('c'), Bead('d'), Bead('d') });
    Necklace necklace2 = Necklace({ Bead('a'), Bead('b'), Bead('c'), Bead('d'), Bead('f') });

    set<Necklace> vertices = generateVertices(necklace1, necklace2, changeableColors);
    unordered_map<Necklace, size_t, NecklaceHasher> verticesToIndex = generateVerticesMap(vertices);

    //cout << "Vertices: " << endl;
    //size_t i{ 0 };
    //for (const Necklace& n : vertices)
    //{
    //    cout << "[" << i << "] "; n.printNecklace(); cout << endl;
    //    ++i;
    //}

    Graph graph(vertices.size());
    initGraph(graph, vertices, verticesToIndex, changeableColors);
    graph.print();

    size_t necklace1Index = verticesToIndex[necklace1];
    size_t necklace2Index = verticesToIndex[necklace2];
    int nearestCommonVertexIndex = graph.findNearestCommonVertex(necklace1Index, necklace2Index);

    if (nearestCommonVertexIndex != -1)
    {
        cout << "Nearest common vertex index: " << nearestCommonVertexIndex << endl;
        int** matrixCosts = graph.shortestPathsMatrix();

        int cheapestTransition = graph.getShortestPathCostFromTo(matrixCosts, necklace1Index, nearestCommonVertexIndex) + graph.getShortestPathCostFromTo(matrixCosts, necklace2Index, nearestCommonVertexIndex);

        cout << "Cheapest cost: " << cheapestTransition << endl;

        for (const pair<Necklace, size_t>& p : verticesToIndex)
        {
            if (p.second == nearestCommonVertexIndex)
            {
                cout << "Nearest common vertex: "; p.first.printNecklace();
            }
        }

        graph.clearShortestPathsMatrix(matrixCosts);
    }
    else
    {
        cout << "-1" << endl;
        cout << "Not possible" << endl;
    }  

    return 0;
}