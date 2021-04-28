#include <iostream>
#include <list>
#include <vector>

using namespace std;

bool isValidColor(char c) {
    return ((int)c >= 33 && (int)c <= 126); 
}

struct Bead {
    char color;

    Bead(char c) {

        if (isValidColor(c))
        {
            color = c;
        }
        else
        {
            throw invalid_argument("ASCII of char < 33 or > 126!");
        }
        
    }
};

struct Necklace {
    vector<Bead> beads;

    Necklace() {
        beads = vector<Bead>{'!'};
    }

    Necklace(vector<Bead> _beads) {
        for (Bead b : _beads) 
        {
            beads.push_back(Bead(b.color));
        }
    }

    bool isEqual(const Necklace& other_necklace) const {
        if (beads.size() == other_necklace.beads.size()) {
            for (size_t i{0}; i < beads.size(); ++i) 
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

    bool isAdjacent(const Necklace& other_necklace) const {
        if (beads.size() == other_necklace.beads.size() && !isEqual(other_necklace))
        {   
            size_t amountDifferentBeads{0};
            for (size_t i{0}; i < beads.size(); ++i) 
            {
                if ((int) beads[i].color != (int) other_necklace.beads[i].color)
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

    char singleBeadColorDifference(const Necklace& other_necklace) const {
        for (size_t i{0}; i < beads.size(); ++i) 
        {   
            char fromColor = beads[i].color;
            char toColor = other_necklace.beads[i].color;
            if (fromColor != toColor)
            {
                return toColor;
            }
        }
    }

    bool isValidIndex(size_t idx) {
        return idx < beads.size();
    }

    void changeBeadColor(size_t at_idx, char toColor) {
        if (isValidIndex(at_idx) && isValidColor(toColor))
        {
            beads[at_idx].color = toColor;
        }
        else  
        {
            throw invalid_argument("ASCII of char < 33 or > 126 or invalid index!");
        }        
    }

    int firstAppearanceOfBeadColor(char color) {
        
        for (size_t i{0}; i < beads.size(); ++i) 
        {
            if (beads[i].color == color)
            {
                return i;
            }
        }

        return -1;
    }

    vector<int> getIndicesOfBeadColor(char color) {
        vector<int> result;
        
        if (firstAppearanceOfBeadColor(color) != -1)
        {   
            for(int i{firstAppearanceOfBeadColor(color)}; i < beads.size(); ++i) {
                if (beads[i].color == color) {
                    result.push_back(i);
                }
            }
        }

        return result;
    }

    void printNecklace() const {
        for (size_t i{0}; i < beads.size(); ++i)
        {   
            if (i == beads.size() - 1)
            {
                std::cout << beads[i].color;
                continue;
            }
            std::cout << beads[i].color << "-";
        }
    }
};

struct Vertex {
    Necklace                    necklace;
    bool                        visited;

    Vertex() 
    {
        necklace = Necklace({Bead('!')});
        visited  = false;
    }

    Vertex(Necklace _necklace)
    {
        necklace = Necklace(_necklace.beads);
        visited = false;
    }

    bool isEqual(const Vertex& otherVertex) const
    {
        return (this->necklace.isEqual(otherVertex.necklace));
    }
};

using Edge = pair<Vertex, int>;
using AdjacencyLists = vector< list<Edge>>;

class Graph {
    size_t          verticesAmount;
    AdjacencyLists  adjacencyLists;

public:
    Graph(size_t _verticesAmount) : verticesAmount{_verticesAmount} 
    {
        for(size_t i{0}; i < verticesAmount; ++i)
        {   
            Vertex newVertex                            = Vertex();
            Edge   newVertexDefaultEdge                 = Edge(newVertex, 0);
            list<Edge> newVertexDefaultAdjacencyList    = list<Edge>(1);
            newVertexDefaultAdjacencyList.push_back(newVertexDefaultEdge);
            adjacencyLists.push_back(newVertexDefaultAdjacencyList);
        }
        verticesAmount = adjacencyLists.size();
    }

    Graph(const Graph& other_graph)
    {   
        for (size_t i{0}; i < other_graph.verticesAmount; ++i)
        {   
            adjacencyLists.at(i).pop_front();
            adjacencyLists.at(i).push_front(other_graph.adjacencyLists.at(i).front());
            
            if (other_graph.adjacencyLists.at(i).size() > 1)
            {
                for (Edge otherGraphAdjacentEdge : other_graph.adjacencyLists.at(i))
                {   
                    Vertex adjacentVertex   = otherGraphAdjacentEdge.first;
                    int    adjacentCost     = otherGraphAdjacentEdge.second;
                    adjacencyLists.at(i).push_back(make_pair(adjacentVertex, adjacentCost));
                } 
            }
        }

        verticesAmount = other_graph.verticesAmount;        
    }

    void operator= (const Graph& other_graph)
    {
        for (size_t i{0}; i < other_graph.verticesAmount; ++i)
        {   
            adjacencyLists.at(i).pop_front();
            adjacencyLists.at(i).push_front(other_graph.adjacencyLists.at(i).front());
            
            if (other_graph.adjacencyLists.at(i).size() > 1)
            {
                for (Edge otherGraphAdjacentEdge : other_graph.adjacencyLists.at(i))
                {   
                    if (otherGraphAdjacentEdge.first.necklace.isEqual(Necklace({Bead('!')})))
                    {
                        adjacencyLists.at(i).pop_front();
                        cout << "Pop" << endl;
                    }
                    Vertex adjacentVertex   = otherGraphAdjacentEdge.first;
                    int    adjacentCost     = otherGraphAdjacentEdge.second;
                    adjacencyLists.at(i).push_back(make_pair(adjacentVertex, adjacentCost));
                } 
            }
        }
        
        if (verticesAmount < other_graph.verticesAmount) 
        {
            verticesAmount = other_graph.verticesAmount; 
        }

    } 

    AdjacencyLists& getAdjacencyLists() 
    {
        return this->adjacencyLists;
    }

    void increaseVerticesAmount() 
    {
        this->verticesAmount = ++verticesAmount;
    }

    void increaseVerticesAmount(int manyTimes)
    {
        for (size_t i{0}; i < manyTimes; ++i)
        {
            increaseVerticesAmount();
        }
    }

    bool vertexAlreadyExistsInGraph(const Vertex& ver) 
    {
        for (size_t i{0}; i < this->verticesAmount; ++i)
        {
            if ( this->adjacencyLists.at(i).front().first.necklace.isEqual(ver.necklace) )
            {
                return true;
            }
        }
        return false;
    }  

    int indexOfVertexInGraph(const Vertex& vertex)
    {   
        if(vertexAlreadyExistsInGraph(vertex))
        {
            for (size_t i{0}; i < this->verticesAmount; ++i)
            {   
                Necklace vertexNecklace     = vertex.necklace;
                Vertex currentVertex        = this->adjacencyLists.at(i).front().first;
                Necklace currentNecklace    = currentVertex.necklace; 
                if (vertexNecklace.isEqual(currentNecklace))
                {
                    return i;
                }
            }
        }
        
        return -1;
    }

    void addEdge(const Vertex& from, const Vertex& to, int cost) 
    {   
        if (indexOfVertexInGraph(from) != -1 && indexOfVertexInGraph(to) != -1)
        {
            int fromIndex   = indexOfVertexInGraph(from);
            int toIndex     = indexOfVertexInGraph(to);
    
            this->adjacencyLists.at(fromIndex).push_back(make_pair(to, cost));
        }
    }

    void printGraph() 
    {
        for (size_t i{0}; i < this->verticesAmount; ++i)
        {
            std::cout << "[" << i << "]:";
            this->adjacencyLists.at(i).front().first.necklace.printNecklace();
            std::cout << endl;
            for (const Edge& currentEdge : this->adjacencyLists.at(i))
            {   
                if  (!currentEdge.first.necklace.isEqual(this->adjacencyLists.at(i).front().first.necklace))
                {
                std::cout << " ->";
                currentEdge.first.necklace.printNecklace();
                std::cout << "(" << currentEdge.second << ")";
                std::cout << endl;
                }
            }
        }
    }
};



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
            for (const Bead& bead: copyNecklace.beads) 
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

void generateAllPossibilities(vector<Necklace>& allPossibilities, Necklace necklace, char fromColor, char toColor)
{   
    vector<Necklace> allPossibilitiesVector = generateAllPossibilitiesVector(necklace, fromColor, toColor);
    if (!allPossibilitiesVector.empty())
    {
        allPossibilities.insert(allPossibilities.end(), allPossibilitiesVector.begin(), allPossibilitiesVector.end());
    }
}


int main() {
    unsigned N = 5;
    unsigned M = 3;
    
    vector<Bead> beads1 = {Bead('A'), Bead('b'), Bead('c'), Bead('d'), Bead('d')}; 
    vector<Bead> beads2 = {Bead('a'), Bead('b'), Bead('c'), Bead('d'), Bead('f')};

    Necklace necklace1 = Necklace(beads1);
    Necklace necklace2 = Necklace(beads2);
       
    vector<Necklace> allPossibilitiesNecklace1;
    allPossibilitiesNecklace1.push_back(necklace1);
    generateAllPossibilities(allPossibilitiesNecklace1, necklace1, 'A', 'a');
    while (necklace1.firstAppearanceOfBeadColor('A') != -1) 
    {    
        int firstAppearanceIndex = necklace1.firstAppearanceOfBeadColor('A');
        necklace1.changeBeadColor(firstAppearanceIndex, 'a');
    }

    generateAllPossibilities(allPossibilitiesNecklace1, necklace1, 'd', 'e');
    generateAllPossibilities(allPossibilitiesNecklace1, necklace1, 'f', 'e');    
    while (necklace1.firstAppearanceOfBeadColor('d') != -1) 
    {    
        int firstAppearanceIndex = necklace1.firstAppearanceOfBeadColor('d');
        necklace1.changeBeadColor(firstAppearanceIndex, 'e');
    }


    generateAllPossibilities(allPossibilitiesNecklace1, necklace1, 'f', 'e');
    while (necklace1.firstAppearanceOfBeadColor('f') != -1) 
    {    
        int firstAppearanceIndex = necklace1.firstAppearanceOfBeadColor('f');
        necklace1.changeBeadColor(firstAppearanceIndex, 'e');
    }

    Graph graph = Graph(allPossibilitiesNecklace1.size());

    for (size_t i{0}; i < allPossibilitiesNecklace1.size(); ++i)
    {
        Necklace currentNecklace = allPossibilitiesNecklace1[i];
        Vertex   vertexToAdd     = Vertex(currentNecklace);
        graph.getAdjacencyLists().at(i).pop_front();
        graph.getAdjacencyLists().at(i).push_front(pair<Vertex, int>(vertexToAdd,0));
        graph.getAdjacencyLists().at(i).pop_back();

        for (size_t z{i}; z < allPossibilitiesNecklace1.size(); ++z)
        {   
            if (currentNecklace.isAdjacent(allPossibilitiesNecklace1[z]))
            {   
                Necklace adjacentNecklace = allPossibilitiesNecklace1[z];
                if (currentNecklace.singleBeadColorDifference(adjacentNecklace) == 'a')
                {
                    int cost = 3;
                    graph.getAdjacencyLists().at(i).push_back(make_pair(adjacentNecklace, cost));
                }
                else if (currentNecklace.singleBeadColorDifference(adjacentNecklace) == 'e') 
                {
                    int cost = 3;
                    graph.getAdjacencyLists().at(i).push_back(make_pair(adjacentNecklace, cost));
                }
                else if (currentNecklace.singleBeadColorDifference(adjacentNecklace) == 'f') 
                {
                    int cost = 1;
                    graph.getAdjacencyLists().at(i).push_back(make_pair(adjacentNecklace, cost));
                }
            }
        }
    }

    graph.printGraph();
    std::cout << endl;
    
    vector<Necklace> allPossibilitiesNecklace2;
    allPossibilitiesNecklace2.push_back(necklace2);
    generateAllPossibilities(allPossibilitiesNecklace2, necklace2, 'A', 'a');
    while (necklace2.firstAppearanceOfBeadColor('A') != -1) 
    {    
        int firstAppearanceIndex = necklace2.firstAppearanceOfBeadColor('A');
        necklace2.changeBeadColor(firstAppearanceIndex, 'a');
    }
    generateAllPossibilities(allPossibilitiesNecklace2, necklace2, 'd', 'e');
    generateAllPossibilities(allPossibilitiesNecklace2, necklace2, 'f', 'e');
    while (necklace2.firstAppearanceOfBeadColor('d') != -1) 
    {    
        int firstAppearanceIndex = necklace2.firstAppearanceOfBeadColor('d');
        necklace2.changeBeadColor(firstAppearanceIndex, 'e');
    }
    generateAllPossibilities(allPossibilitiesNecklace2, necklace2, 'f', 'e');
    while (necklace2.firstAppearanceOfBeadColor('f') != -1) 
    {    
        int firstAppearanceIndex = necklace2.firstAppearanceOfBeadColor('f');
        necklace2.changeBeadColor(firstAppearanceIndex, 'e');
    }
    
    // graph.getAdjacencyLists().resize(allPossibilitiesNecklace1.size() + allPossibilitiesNecklace2.size());
    // graph.increaseVerticesAmount(allPossibilitiesNecklace2.size());
    Graph actualGraph = Graph(allPossibilitiesNecklace1.size() + allPossibilitiesNecklace2.size());
    cout << "actualGraph: " << endl;
    actualGraph.printGraph();
    cout << endl;
    actualGraph = graph;

    cout << "actualGraph = graph: " << endl;
    actualGraph.printGraph();
    cout << endl;

    for (size_t i{0}; i < allPossibilitiesNecklace2.size(); ++i)
    {   
        Necklace currentNecklace = allPossibilitiesNecklace2[i];
        Vertex   vertexToAdd     = Vertex(currentNecklace);
        cout << "-> ";
        currentNecklace.printNecklace();
        cout << ": " << endl;
        if (!(actualGraph.vertexAlreadyExistsInGraph(vertexToAdd))) {
            cout << " New vertex goes at " << i + allPossibilitiesNecklace1.size() << endl;
            graph.getAdjacencyLists().at(i + allPossibilitiesNecklace1.size()).pop_front();
            actualGraph.getAdjacencyLists().at(i + allPossibilitiesNecklace1.size()).push_back(make_pair(vertexToAdd, 0));
            //actualGraph.getAdjacencyLists().at(i + allPossibilitiesNecklace1.size()).pop_back();
            actualGraph.increaseVerticesAmount();

            for (size_t z{i}; z < allPossibilitiesNecklace2.size(); ++z) 
            {   
                if (!actualGraph.vertexAlreadyExistsInGraph(Vertex(allPossibilitiesNecklace2[z])) && vertexToAdd.necklace.isAdjacent(allPossibilitiesNecklace2[z]))
                {   
                    cout << "New vertex!" << endl; 
                    Necklace adjacentNecklace = allPossibilitiesNecklace2[z];
                    adjacentNecklace.printNecklace();
                    cout << endl;
                    if (currentNecklace.singleBeadColorDifference(adjacentNecklace) == 'a')
                    {
                        actualGraph.getAdjacencyLists().at(i +allPossibilitiesNecklace1.size()).pop_front();
                        int cost = 3;
                        actualGraph.getAdjacencyLists().at(i + allPossibilitiesNecklace1.size()).push_back(pair<Vertex, int>(vertexToAdd, cost));
                    }
                    else if (currentNecklace.singleBeadColorDifference(adjacentNecklace) == 'e') 
                    {
                        actualGraph.getAdjacencyLists().at(i +allPossibilitiesNecklace1.size()).pop_front();
                        int cost = 3;
                        actualGraph.getAdjacencyLists().at(i + allPossibilitiesNecklace1.size()).push_back(pair<Vertex, int>(vertexToAdd, cost));
                    }
                    else if (currentNecklace.singleBeadColorDifference(adjacentNecklace) == 'f') 
                    {
                        actualGraph.getAdjacencyLists().at(i +allPossibilitiesNecklace1.size()).pop_front();
                        int cost = 1;
                        actualGraph.getAdjacencyLists().at(i + allPossibilitiesNecklace1.size()).push_back(pair<Vertex, int>(vertexToAdd, cost));
                    }   
                }
                else if ((actualGraph.vertexAlreadyExistsInGraph(Vertex(allPossibilitiesNecklace2[z])) && vertexToAdd.necklace.isAdjacent(allPossibilitiesNecklace2[z])))
                {   
                    cout << "Vertex already exists!" << endl;
                    int indexOfExistance = actualGraph.indexOfVertexInGraph(vertexToAdd);
                    actualGraph.getAdjacencyLists().at(indexOfExistance).front().first.necklace.printNecklace();
                    cout << endl;
                    cout <<  "At index: " << indexOfExistance << endl;
                    Necklace adjacentNecklace = actualGraph.getAdjacencyLists().at(indexOfExistance).front().first.necklace;
                    if (currentNecklace.singleBeadColorDifference(adjacentNecklace) == 'a')
                    {
                        int cost = 3;
                        actualGraph.getAdjacencyLists().at(indexOfExistance).push_back(pair<Vertex, int>(vertexToAdd, cost));
                    }
                    else if (currentNecklace.singleBeadColorDifference(adjacentNecklace) == 'e') 
                    {
                        int cost = 3;
                        actualGraph.getAdjacencyLists().at(indexOfExistance).push_back(pair<Vertex, int> (vertexToAdd, cost));
                    }
                    else if (currentNecklace.singleBeadColorDifference(adjacentNecklace) == 'f') 
                    {
                        int cost = 1;
                        actualGraph.getAdjacencyLists().at(indexOfExistance).push_back(pair<Vertex, int>(vertexToAdd, cost));
                    }
                }
                else
                {
                    cout << "UNCAUGHT BEHAVIOR!" << endl;
                }
                
            }
        }       
    }

    // allPossibilitiesNecklace1.insert(allPossibilitiesNecklace1.end(), allPossibilitiesNecklace2.begin(), allPossibilitiesNecklace2.end());
    // graphShrinkToFit(graph);
    actualGraph.printGraph();


    return 0;
}