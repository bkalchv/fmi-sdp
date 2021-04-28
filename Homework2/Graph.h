#pragma once
#include "Necklace.h"
#include <iostream>
#include <queue>
#include <vector>
#include <stdexcept>

const int INF = 0x3f3f3f3f;

class Graph
{
private:
	int**	adjacencyMatrix;
	size_t	verticesAmount;
public:
	Graph(size_t);
	int** getAdjacencyMatrix();
	size_t getVerticesAmount();
	void addEdge(size_t, size_t, int);
	void removeEdge(size_t, size_t);
	void print() const;
	~Graph();
	int initBFSWave(size_t, std::vector<bool>&, std::queue<size_t>&);
	int findNearestCommonVertex(size_t, size_t);
	int** shortestPathsMatrix();
	void clearShortestPathsMatrix(int**);
	int getShortestPathCostFromTo(int**, size_t, size_t);
};

