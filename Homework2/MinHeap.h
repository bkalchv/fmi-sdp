#pragma once
#include <iostream>

class MinHeap
{
public:
	size_t* indicesArray;
	size_t	capacity;
	size_t	size;

	MinHeap(size_t capacity);
	size_t*		getIndicesArray();
	void		heapify(size_t);
	size_t		getParentIndex(size_t);
	size_t		getLeftIndex(size_t);
	size_t		getRightIndex(size_t);
	size_t		exctractRoot(); // Min
	size_t		getRoot();
	//void		deleteKey(size_t);
	size_t		insertKey(size_t);

	static void swap(size_t*, size_t*);
};

