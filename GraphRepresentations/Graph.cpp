// Is there a cycle in the graph?
// BFS/DFS Vertices iteration
// using Adjacency Lists

#include <iostream>
#include <map>
#include <forward_list>
#include <set>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

map<size_t, forward_list<size_t>> adjacencyLists;

bool hasEdge(size_t from, size_t to)
{
	if (adjacencyLists.find(from) != adjacencyLists.end() && adjacencyLists.find(from) != adjacencyLists.end())
	{
		for (size_t neighbour : adjacencyLists[from])
		{
			if (neighbour == to)
			{
				return true;
			}
		}
	}

	return false;
}

// Adds an edge, if it doesn't exist already (directed Graph)
void addEdge(size_t from, size_t to)
{
	if (adjacencyLists.find(from) != adjacencyLists.end() && adjacencyLists.find(to) != adjacencyLists.end())
	{
		if (!hasEdge(from, to))
		{
			adjacencyLists[from].push_front(to);
		}
	}		
}

void readVerticesAddEdges(const std::string& filename, std::set<size_t>& vertices)
{
	ifstream inMyfile;
	inMyfile.open(filename);
	
	if (inMyfile)
	{
		std::string line;
		int number1;
		int number2;
		char openingBracket;
		char closingBracket;
		while (getline(inMyfile, line))
		{
			std::istringstream iss1(line);
			std::istringstream iss2(line);

			if (iss1 >> openingBracket >> number1 >> number2 >> closingBracket
				&& openingBracket == '['
				&& closingBracket == ']')
			{
				//cout << "line: " << openingBracket << number1 << " " << number2 << closingBracket << endl;
				vertices.insert(number1);
				vertices.insert(number2);

				if (adjacencyLists.find(number1) == adjacencyLists.end())
				{
					adjacencyLists[number1];
					cout << "Vertex [" << number1 << "] added successfully." << endl;
				}

				if (adjacencyLists.find(number2) == adjacencyLists.end())
				{
					adjacencyLists[number2];
					cout << "Vertex [" << number2 << "] added successfully." << endl;
				}

				if (!hasEdge(number1, number2))
				{
					addEdge(number1, number2);
					if (adjacencyLists[number1].empty())
					{
						cout << "Faulty add!" << endl;
					}
					else
					{
						cout << "Edge between " << number1 << " and " << number2 << " added successfully!" << endl;
					}
				}
			}

			if (iss2 >> openingBracket >> number1 >> closingBracket
				&& openingBracket == '['
				&& closingBracket == ']')
			{
				//cout << "line: " << openingBracket << number1 << closingBracket << endl;
				vertices.insert(number1);

				if (adjacencyLists.find(number1) == adjacencyLists.end())
				{
					adjacencyLists[number1];
				}
			}
		}
	}
	else
	{
		cout << "Couldn't open file." << endl;
	}

	inMyfile.close();
}

void printAdjacencyLists()
{
	for (const std::pair<size_t, forward_list<size_t>>& p : adjacencyLists)
	{
		cout << "[" << p.first << "]";

		if (!p.second.empty())
		{
			cout << "-> {";
			for (const size_t& neighbour : p.second)
			{
				cout << "[" << neighbour << "]";
			}
			cout << "}";
		}
		else
		{
			cout << " *no neighbours*";
		}

		cout << endl;
	}
}

// DFS hasCycleRec() for a directed graph
bool hasCycleRec(size_t vertex, set<size_t>& visited, set<size_t>& current)
{
	if (current.find(vertex) != current.end())
	{
		return true;
	}

	current.insert(vertex);

	for (size_t neighbour : adjacencyLists[vertex])
	{
		if (visited.find(neighbour) == visited.end())
		{
			if (hasCycleRec(neighbour, visited, current))
				return true;
		}
	}

	visited.insert(vertex);
	return false;
}

// DFS hasCycle() for a directed graph
bool hasCycle()
{
	set<size_t> isVisited;
	set<size_t> isCurrent;   // If it is an directed Graph(We have to check where we're coming from)
	return hasCycleRec(0, isVisited, isCurrent);
}

void DFSRemoveVisited(size_t from, std::set<size_t>& vertices)
{
	vertices.erase(from);

	if (adjacencyLists.find(from) != adjacencyLists.end())
	{	
		for(size_t neighbour : adjacencyLists[from])
		{
			if (vertices.find(neighbour) != vertices.end())
			{
				DFSRemoveVisited(neighbour, vertices);
			}
		}
	}
}

size_t countComponents(const set<size_t>& vertices)
{
	size_t count = 0;

	set<size_t> verticesCopy;

	for (size_t vertex : vertices)
	{
		verticesCopy.insert(vertex);
	}

	while (!verticesCopy.empty())
	{
		++count;
		size_t currentVertex = *verticesCopy.begin();
		DFSRemoveVisited(currentVertex, verticesCopy);
	}

	return count;
}

int main()
{
	std::set<size_t> vertices;
	readVerticesAddEdges("Text.txt", vertices);

	cout << "vertices: ";
	for (const size_t& vertex : vertices)
	{
		cout << vertex << " ";
	}
	cout << endl;
	cout << vertices.size() << " vertices." << endl;

	for (const size_t& vertex : vertices)
	{
		if (adjacencyLists.find(vertex) == adjacencyLists.end())
		{
			cout << vertex << "not in ordered_map! ";
			cout << "Unexpected behavior!" << endl;
		}
	}

	printAdjacencyLists();

	cout << "Our graph has " << (hasCycle() ? "at least one cycle." : "NO cycles.") << endl;

	cout << "Our graph consists of " << countComponents(vertices) << " components." << endl;

	return 0;
}