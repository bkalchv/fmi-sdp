#include "Graph.h"

Graph::Graph(size_t _verticesAmount)
{
	this->verticesAmount = _verticesAmount;
	adjacencyMatrix = new int* [_verticesAmount];
	for (size_t i{ 0 }; i < this->verticesAmount; ++i)
	{
		adjacencyMatrix[i] = new int[verticesAmount];
		for (size_t j{ 0 }; j < this->verticesAmount; ++j)
		{
			adjacencyMatrix[i][j] = INF;
		}
	}
}

int** Graph::getAdjacencyMatrix()
{
	return this->adjacencyMatrix;
}

size_t Graph::getVerticesAmount()
{
	return this->verticesAmount;
}

void Graph::addEdge(size_t fromIndex, size_t toIndex, int cost)
{
	adjacencyMatrix[fromIndex][toIndex] = cost;
}

void Graph::removeEdge(size_t fromIndex, size_t toIndex)
{
	adjacencyMatrix[fromIndex][toIndex] = INF;
}

void Graph::print() const
{
	std::cout << "Adjacency Graph: " << std::endl;

	for (size_t i = 0; i < verticesAmount; i++)
	{
		if (i == 0)
		{
			std::cout << "   ";
		}
		std::cout << "[" << i << "]";
	}
	std::cout << std::endl;

	for (size_t i = 0; i < verticesAmount; i++) {
		std::cout << "[" << i << "]";
		for (size_t j = 0; j < verticesAmount; j++)
		{
			if (adjacencyMatrix[i][j] == INF)
			{
				std::cout << " - ";
			}
			else
			{
				std::cout << " " <<  adjacencyMatrix[i][j] << " ";
			}
			
		}
		std::cout << std::endl;
	}
}

int findNearestCommonVertex(size_t, size_t);

Graph::~Graph()
{
	for (size_t i = 0; i < verticesAmount; ++i)
	{
		delete[] adjacencyMatrix[i];
	}
	delete[] adjacencyMatrix;
}

int Graph::initBFSWave(size_t indexNecklace, std::vector<bool>& visited, std::queue<size_t>& queue)
{	
	visited[indexNecklace] = true;
	
	if (!queue.empty())
	{
		queue.pop();
	}

	for (size_t i{ 0 }; i < this->verticesAmount; ++i)
	{
		if (this->adjacencyMatrix[indexNecklace][i] != INF && visited[i] == false)
		{
			queue.push(i);
			visited[i] = true;
			//std::cout << "visited " << i << std::endl;
			continue;
		}
		else if (this->adjacencyMatrix[indexNecklace][i] != INF && visited[i] == true)
		{
			//std::cout << "already visited " << i << std::endl;
			return i;
		}
	}

	return -1;
}

int Graph::findNearestCommonVertex(size_t indexNecklace1, size_t indexNecklace2)
{
	int nearestCommonVertexIndex{-1};
	
	if (this->getVerticesAmount() > 2)
	{
		std::vector<bool> visited(this->verticesAmount, false);
		std::queue<size_t> queueNecklace1;
		queueNecklace1.push(indexNecklace1);
		std::queue<size_t> queueNecklace2;
		queueNecklace2.push(indexNecklace2);

		while (true)
		{
			int initBFSReturnValueNecklace1 = initBFSWave(queueNecklace1.front(), visited, queueNecklace1);
			//std::cout << "returnValue1: " << initBFSReturnValueNecklace1 << std::endl;
			if (initBFSReturnValueNecklace1 == -1)
			{
				int initBFSReturnValueNecklace2 = initBFSWave(queueNecklace2.front(), visited, queueNecklace2);
				//std::cout << "returnValue2: " << initBFSReturnValueNecklace2 << std::endl;
				if (initBFSReturnValueNecklace2 != -1)
				{
					nearestCommonVertexIndex = initBFSReturnValueNecklace2;
					break;
				}

				if (queueNecklace1.empty() && queueNecklace2.empty())
				{
					break;
				}
			}
			else
			{				
				nearestCommonVertexIndex = initBFSReturnValueNecklace1;
				break;
			}
		}
	}
	else if (this->getVerticesAmount() == 2)
	{
		return -2;
	}
	
	return nearestCommonVertexIndex;
}

int** Graph::shortestPathsMatrix()
{
	// Floyd-Warshall
	size_t verticesAmount = this->getVerticesAmount();
	int** costs;

	costs = new int*[verticesAmount];

	for (size_t i{ 0 }; i < verticesAmount; ++i)
	{
		costs[i] = new int[verticesAmount];
	}

	for (size_t i{ 0 }; i < verticesAmount; ++i)
	{
		for (size_t j{ 0 }; j < verticesAmount; ++j)
		{
			costs[i][j] = this->adjacencyMatrix[i][j];
		}
	}

	for (size_t k{ 0 }; k < verticesAmount; ++k)
	{
		// Pick all vertices as source one by one 
		for (size_t i{ 0 }; i < verticesAmount; ++i)
		{
			// Pick all vertices as destination for the 
			// above picked source 
			for (size_t j{ 0 }; j < verticesAmount; ++j)
			{
				// If vertex k is on the shortest path from 
				// i to j, then update the value of dist[i][j] 
				if (costs[i][k] + costs[k][j] < costs[i][j])
					costs[i][j] = costs[i][k] + costs[k][j];
			}
		}
	}

	return costs;
}

void Graph::clearShortestPathsMatrix(int** costs)
{
	for (size_t i{ 0 }; i < this->verticesAmount; ++i)
	{
		delete[] costs[i];
	}

	delete costs;
}

int Graph::getShortestPathCostFromTo(int** costs, size_t fromIndex, size_t toIndex)
{
	if (costs[fromIndex][toIndex] == INF)
	{
		throw std::exception("No path between the two given nodes!");
	}

	return costs[fromIndex][toIndex];
}