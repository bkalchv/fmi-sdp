#include "MinHeap.h"

MinHeap::MinHeap(size_t capacity)
{
	size			= 0;
	this->capacity	= capacity;
	indicesArray	= new size_t[capacity];
}

void MinHeap::heapify(size_t index)
{
	size_t l = getLeftIndex(index);
	size_t r = getRightIndex(index);
	size_t smallest = index;
	if (l < this->size && this->indicesArray[l] < this->indicesArray[index])
	{
		smallest = l;
	}
		
	if (r < this->size && this->indicesArray[r] < this->indicesArray[smallest])
	{
		smallest = r;
	}
		
	if (smallest != index)
	{
		swap(&this->indicesArray[index], &this->indicesArray[smallest]);
		heapify(smallest);
	}
}

size_t MinHeap::getParentIndex(size_t index)
{
	return (index-  1) / 2;
}

size_t MinHeap::getLeftIndex(size_t index)
{
	return (2 * index + 1);
}

size_t MinHeap::getRightIndex(size_t index)
{
	return (2 * index + 2);
}

void MinHeap::swap(size_t* left, size_t* right)
{
	int temp = *left;
	*left = *right;
	*right = temp;
}

size_t MinHeap::exctractRoot()
{
	if (size <= 0)
	{
		std::cout << "IMPLEMENT it!" << std::endl;
		// return INF
	}

	if (size == 1)
	{
		--size;
		return this->indicesArray[0];
	}

	size_t rootValue		= this->indicesArray[0];
	this->indicesArray[0]	= this->indicesArray[size-1];
	--size;
	heapify(0);

	return rootValue;
}

size_t MinHeap::getRoot()
{
	return this->indicesArray[0];
}

size_t MinHeap::insertKey(size_t idx)
{
	if (this->size == this->capacity)
	{
		std::cout << "Overflow!" << std::endl;
		//return;
	}

	size++;
	size_t lastIndex = size - 1;
	this->indicesArray[lastIndex] = idx;
	
	while (lastIndex != 0 && this->indicesArray[getParentIndex(lastIndex)] > this->indicesArray[lastIndex])
	{
		swap(&(this->indicesArray[lastIndex]), &(this->indicesArray[lastIndex]));
		lastIndex = getParentIndex(lastIndex);
	}
}