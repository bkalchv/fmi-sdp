// Is there a cycle in the graph?
// BFS/DFS Vertices iteration
// using Adjacency Lists

#include <iostream>
#include <map>
#include <forward_list>
#include <set>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

map<size_t, forward_list<size_t>> adjacencyLists;

bool hasEdge(size_t from, size_t to)
{
	if (!adjacencyLists[from].empty())
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
	if (!hasEdge(from, to))
	{
		if (adjacencyLists[from].empty())
		{
			adjacencyLists[from] = forward_list<size_t>();
		}
		adjacencyLists[from].push_front(to);
	}
}

// DFS hasCycleRec() for a directed graph
//bool hasCycleRec(size_t vertexIndex, vector<bool>& isVisited, vector<bool>& isCurrent)
//{
//	if (isCurrent[vertexIndex])
//	{
//		return true;
//	}
//
//	isCurrent[vertexIndex] = true;
//
//	for (size_t neighbour : adjacencyLists[vertexIndex])
//	{
//		if (!isVisited[neighbour])
//		{
//			if (hasCycleRec(neighbour, isVisited, isCurrent))
//				return true;
//		}
//	}
//
//	isVisited[vertexIndex] = true;
//	return false;
//}
//
//// DFS hasCycle() for a directed graph
//bool hasCycle()
//{
//	vector<bool> isVisited;
//	vector<bool> isCurrent; // If it is an directed Graph (We have to check where we're coming from)
//	isVisited.resize(adjacencyLists.size(), false);
//	isCurrent.resize(adjacencyLists.size(), false);
//	return hasCycleRec(0, isVisited, isCurrent);
//}

//void DFSRemoveVisited(size_t from, std::set<size_t>& vertices)
//{
//	vertices.erase(from);
//
//	for (size_t neighbour : adjacencyLists[from])
//	{
//		if (vertices.find(neighbour) != vertices.end())
//		{
//			DFSRemoveVisited(neighbour, vertices);
//		}
//	}
//}

//size_t countComponents()
//{
//	size_t count = 0;
//	std::set<size_t> vertices;
//
//	for (size_t i = 0; i < adjacencyLists.size(); ++i)
//	{
//		vertices.insert(i);
//	}
//
//	while (!vertices.empty())
//	{
//		++count;
//		size_t currentVertex = *vertices.begin();
//		DFSRemoveVisited(currentVertex, vertices);
//	}
//
//	return count;
//}

void readVerticesAddEdges(std::string filename, std::set<size_t>& vertices)
{
	ifstream inMyfile;
	inMyfile.open(filename.c_str());
	std::string line;

	if (inMyfile)
	{
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
				}

				if (adjacencyLists.find(number2) == adjacencyLists.end())
				{
					adjacencyLists[number2];
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

	for (const size_t& vertex : vertices)
	{
		if (adjacencyLists.find(vertex) == adjacencyLists.end())
		{
			cout << vertex << "not in ordered_map! ";
			cout << "Unexpected behavior!" << endl;
		}
	}

	printAdjacencyLists();

	return 0;
}