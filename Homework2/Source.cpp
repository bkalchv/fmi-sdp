#include <iostream>
#include <list>
#include <vector>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include "Bead.h"
#include "Necklace.h"
#include "NecklaceHasher.h"
#include "Graph.h"

using namespace std;

//struct Vertex {
//    Necklace necklace;
//    bool visited;
//
//    Vertex()
//    {
//        necklace = Necklace({ Bead('!') });
//        visited = false;
//    }
//
//    Vertex(Necklace _necklace)
//    {
//        necklace = Necklace(_necklace.beads);
//        visited = false;
//    }
//
//    bool isEqual(const Vertex& otherVertex) const
//    {
//        return (this->necklace.isEqual(otherVertex.necklace));
//    }
//};
//
//using Edge = pair<Vertex, int>;
//using AdjacencyLists = vector<list<Edge>>;
//
//class Graph {
//    size_t          size;
//    size_t          graphCapacity;
//    AdjacencyLists  adjacencyLists;
//
//public:
//    Graph(size_t _verticesAmount) : graphCapacity{ _verticesAmount }
//    {
//        for (size_t i{ 0 }; i < graphCapacity; ++i)
//        {
//            Vertex newVertex = Vertex();
//            Edge   newVertexDefaultEdge = Edge(newVertex, 0);
//            list<Edge> newVertexDefaultAdjacencyList = list<Edge>(1);
//            newVertexDefaultAdjacencyList.push_back(newVertexDefaultEdge);
//            adjacencyLists.push_back(newVertexDefaultAdjacencyList);
//        }
//        this->graphCapacity = adjacencyLists.size();
//        this->size = 0;
//    }
//
//    Graph(const Graph& other_graph)
//    {
//        this->size = 0;
//
//        for (size_t i{ 0 }; i < other_graph.graphCapacity; ++i)
//        {
//            adjacencyLists.at(i).pop_front();
//            adjacencyLists.at(i).push_front(other_graph.adjacencyLists.at(i).front());
//            ++size;
//
//            if (other_graph.adjacencyLists.at(i).size() > 1)
//            {
//                for (Edge otherGraphAdjacentEdge : other_graph.adjacencyLists.at(i))
//                {
//                    Vertex adjacentVertex = otherGraphAdjacentEdge.first;
//                    int    adjacentCost = otherGraphAdjacentEdge.second;
//                    adjacencyLists.at(i).push_back(make_pair(adjacentVertex, adjacentCost));
//                }
//            }
//        }
//
//        graphCapacity = other_graph.graphCapacity;
//    }
//
//    void operator= (const Graph& other_graph)
//    {
//        for (size_t i{ 0 }; i < other_graph.graphCapacity; ++i)
//        {
//            while (!adjacencyLists.at(i).empty())
//            {
//                adjacencyLists.at(i).pop_front();
//            }
//
//            if (!other_graph.adjacencyLists.at(i).empty())
//            {
//                for (Edge otherGraphAdjacentEdge : other_graph.adjacencyLists.at(i))
//                {
//                    Vertex adjacentVertex = otherGraphAdjacentEdge.first;
//                    int    adjacentCost = otherGraphAdjacentEdge.second;
//                    adjacencyLists.at(i).push_back(make_pair(adjacentVertex, adjacentCost));
//                    ++size;
//                }
//            }
//        }
//
//        if (graphCapacity < other_graph.graphCapacity)
//        {
//            graphCapacity = other_graph.graphCapacity;
//        }
//
//    }
//
//    AdjacencyLists& getAdjacencyLists()
//    {
//        return this->adjacencyLists;
//    }
//
//    void increaseVerticesAmount()
//    {
//        this->graphCapacity = ++graphCapacity;
//    }
//
//    void increaseGraphSize()
//    {
//        this->size = ++size;
//    }
//
//    void increaseVerticesAmount(size_t manyTimes)
//    {
//        for (size_t i{ 0 }; i < manyTimes; ++i)
//        {
//            increaseVerticesAmount();
//        }
//    }
//
//    bool vertexAlreadyExistsInGraph(const Vertex& ver)
//    {
//        for (size_t i{ 0 }; i < this->graphCapacity; ++i)
//        {
//            if (!this->adjacencyLists.at(i).front().first.necklace.isEqual(Necklace({ Bead('!') })))
//            {
//                Necklace currentNecklace = this->adjacencyLists.at(i).front().first.necklace;
//                if (currentNecklace.isEqual(ver.necklace))
//                {
//                    return true;
//                }
//            }
//        }
//        return false;
//    }
//
//    int indexOfVertexInGraph(const Vertex& vertex)
//    {
//        if (vertexAlreadyExistsInGraph(vertex))
//        {
//            for (size_t i{ 0 }; i < this->graphCapacity; ++i)
//            {
//                Necklace vertexNecklace = vertex.necklace;
//                Vertex currentVertex = this->adjacencyLists.at(i).front().first;
//                Necklace currentNecklace = currentVertex.necklace;
//                if (vertexNecklace.isEqual(currentNecklace))
//                {
//                    return i;
//                }
//            }
//        }
//
//        return -1;
//    }
//
//    void addEdge(const Vertex& from, const Vertex& to, int cost)
//    {
//        if (indexOfVertexInGraph(from) != -1 && indexOfVertexInGraph(to) != -1)
//        {
//            int fromIndex = indexOfVertexInGraph(from);
//            int toIndex = indexOfVertexInGraph(to);
//
//            this->adjacencyLists.at(fromIndex).push_back(make_pair(to, cost));
//        }
//    }
//
//    void printGraph()
//    {
//        for (size_t i{ 0 }; i < this->graphCapacity; ++i)
//        {
//            std::cout << "[" << i << "]:";
//            this->adjacencyLists.at(i).front().first.necklace.printNecklace();
//            std::cout << endl;
//            for (const Edge& currentEdge : this->adjacencyLists.at(i))
//            {
//                if (!currentEdge.first.necklace.isEqual(this->adjacencyLists.at(i).front().first.necklace))
//                {
//                    std::cout << " ->";
//                    currentEdge.first.necklace.printNecklace();
//                    std::cout << "(" << currentEdge.second << ")";
//                    std::cout << endl;
//                }
//            }
//        }
//    }
//};

vector<tuple<char, char, int>> changeableColorsReader(/*size_t changeableColorsAmount*/)
{ 
    vector<tuple<char, char, int>> changeableColors /*(changeableColorsAmount)*/;

    /*for (size_t i{0}; i < changeableColors.size(); ++i)
    {
        char fromColorInput;
        cin >> fromColorInput;
        while (!(Bead::isValidColor(fromColorInput)))
        {
            cout << "Invalid color input. Try again!" << endl;
            cin >> fromColorInput;
        }

        char toColorInput;
        cin >> toColorInput;
        while (!(Bead::isValidColor(toColorInput)))
        {
            cout << "Invalid color input. Try again!" << endl;
            cin >> toColorInput;
        }

        int cost;
        cin >> cost;
        while (cost < 0)
        {
            cout << "Invalid cost input. Try again!" << endl;
            cin >> cost;
        }

        tuple<char, char, int> currentChangeableColorTuple;
        changeableColors[i] = currentChangeableColorTuple;
    }*/

    changeableColors.push_back(make_tuple<char, char, int>('A', 'a', 3));
    changeableColors.push_back(make_tuple<char, char, int>('d', 'e', 3));
    changeableColors.push_back(make_tuple<char, char, int>('f', 'e', 1));

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

    cout << "Vertices: " << endl;
    size_t i{ 0 };

    for (const Necklace& n : vertices)
    {
        cout << "[" << i << "] "; n.printNecklace(); cout << endl;
        ++i;
    }

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

        int cheapestTransitions = graph.getShortestPathCostFromTo(matrixCosts, necklace1Index, nearestCommonVertexIndex) + graph.getShortestPathCostFromTo(matrixCosts, necklace2Index, nearestCommonVertexIndex);

        cout << cheapestTransitions << endl;

        for (const pair<Necklace, size_t>& p : verticesToIndex)
        {
            if (p.second == nearestCommonVertexIndex)
            {
                p.first.printNecklace(); cout << endl;
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