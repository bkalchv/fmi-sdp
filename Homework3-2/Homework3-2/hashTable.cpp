#include "hashTable.h"

const size_t    DEFAULT_INITIAL_SIZE = 8;
const float     DEFAULT_MAX_LOAD_FACTOR = 0.75;

MyHashTable::MyHashTable() : size(0), maxLoadFactor(DEFAULT_MAX_LOAD_FACTOR)
{
	this->rehash(DEFAULT_INITIAL_SIZE);
}

MyHashTable::MyHashTable(size_t capacity) : hashTableVector(capacity), size(0), maxLoadFactor(DEFAULT_MAX_LOAD_FACTOR)
{}

bool MyHashTable::contains(const std::string& keyWord) const
{
	return this->findElement(keyWord) != nullptr;
}

bool MyHashTable::insert(const std::string& word, size_t wordCount)
{
	if (this->contains(word))
	{
		MyHashTable::Element* element = this->findElement(word);
		element->wordCount += wordCount;
	}
	else 
	{
		this->addElement(MyHashTable::Element(word, wordCount));
	}

	return true;
}

bool MyHashTable::insert(const Element& elementToInsert)
{
	std::string elementToInsertKeyWord = elementToInsert.keyWord;

	if (this->contains(elementToInsertKeyWord))
	{
		MyHashTable::Element* element = this->findElement(elementToInsertKeyWord);
		element->wordCount = element->wordCount + 1;
	}
	else
	{
		this->addElement(MyHashTable::Element(elementToInsertKeyWord, 1));
	}

	return true;
}

void MyHashTable::clear()
{
	this->hashTableVector.clear();
	this->size = 0;
}

size_t& MyHashTable::at(const std::string& keyWord)
{
	size_t hashIndex = this->calculateHashIndex(keyWord);
	for (MyHashTable::Element& element : this->hashTableVector[hashIndex]) 
	{
		if (element.keyWord == keyWord) return element.wordCount;
	}
}

const size_t MyHashTable::at(const std::string& keyWord) const
{
	size_t hashIndex = this->calculateHashIndex(keyWord);
	for (const MyHashTable::Element& element : this->hashTableVector[hashIndex])
	{
		if (element.keyWord == keyWord) return element.wordCount;
	}
}

size_t MyHashTable::calculateHashIndex(const std::string& keyWord)
{
	return std::hash<std::string>{}(keyWord) % this->capacity();
}

size_t MyHashTable::calculateHashIndex(const std::string& keyWord) const
{
	return std::hash<std::string>{}(keyWord) % this->capacity();
}

MyHashTable::Element* MyHashTable::findElement(const std::string& keyWord)
{
	size_t indexToLookAt = this->calculateHashIndex(keyWord);

	for (MyHashTable::Element& element : this->hashTableVector[indexToLookAt])
	{
		if (element.keyWord == keyWord) return &element;
	}

	return nullptr;
}

const MyHashTable::Element* MyHashTable::findElement(const std::string& keyWord) const
{
	size_t indexToLookAt = this->calculateHashIndex(keyWord);

	for (const MyHashTable::Element& element : this->hashTableVector[indexToLookAt])
	{
		if (element.keyWord == keyWord) return &element;
	}

	return nullptr;
}

bool MyHashTable::wouldAdditionRequireRehashing() const
{
	float loadedFactorAfterAddition = (float)(this->size + 1) / this->capacity();
	return loadedFactorAfterAddition > this->maxLoadFactor;
}

MyHashTable::Element* MyHashTable::addElement(const std::string&keyWord, size_t wordCount)
{
	return this->addElement(MyHashTable::Element(keyWord, wordCount));
}

MyHashTable::Element* MyHashTable::addElement(const MyHashTable::Element& elementToAdd)
{
	size_t indexToAddTo = this->calculateHashIndex(elementToAdd.keyWord);

	if (this->wouldAdditionRequireRehashing()) this->rehash(this->capacity() * 2);

	this->hashTableVector[indexToAddTo].push_front(elementToAdd);
	++this->size;
	return &this->hashTableVector[indexToAddTo].front();
}

using row = std::forward_list<MyHashTable::Element>;
const row& MyHashTable::getConstRefRowAt(size_t atIndex) const
{
	return this->hashTableVector[atIndex];
}

size_t MyHashTable::getSize() const
{
	return this->size;
}

size_t MyHashTable::capacity() const
{
	return this->hashTableVector.size();
}

float MyHashTable::calculateLoadFactor() const
{
	return (float) this->size / this->capacity();
}

void MyHashTable::rehash(size_t newDesiredCapacity)
{
	size_t smallestCapacityForCurrentHashTableRequired = (size_t) std::ceil(this->size / this->maxLoadFactor);

	size_t newHashTableVectorSize = std::max(std::max(newDesiredCapacity, smallestCapacityForCurrentHashTableRequired), DEFAULT_INITIAL_SIZE);

	std::vector<row> newHashTableVector(newHashTableVectorSize);
	std::swap(this->hashTableVector, newHashTableVector);
	this->size = 0;

	for (const row& fwdList : newHashTableVector) 
	{
		for (const MyHashTable::Element& element : fwdList) 
		{
			this->insert(element);
		}
	}
}

void MyHashTable::reserve(size_t requiredCapacity)
{
	std::cout << "Reserve called with argument: " << requiredCapacity << std::endl;

	if (requiredCapacity > (this->size * maxLoadFactor)) 
	{
		size_t newTableCapacity = this->size;

		do {
			newTableCapacity = newTableCapacity*2;
		} while (requiredCapacity > (newTableCapacity * maxLoadFactor));
		
		this->rehash(newTableCapacity);
	}
	else 
	{
		std::cout << "Required capacity: " << requiredCapacity << std::endl;
		std::cout << "Current capacity: " << this->capacity() << std::endl;
		std::cout << "Current load factor: " << this->calculateLoadFactor() << std::endl;
		std::cout << "Max load factor: " << maxLoadFactor<< std::endl;
	}
}

void MyHashTable::print()
{
	std::cout << "Current capacity: " << this->capacity() << std::endl;
	std::cout << "Current load factor: " << this->calculateLoadFactor() << std::endl;
	std::cout << "Max load factor: " << maxLoadFactor << std::endl;

	for (size_t i{0}; i < hashTableVector.size(); i++)
	{
		std::cout << "[" << i << "]: ";
		for (const Element& elem : hashTableVector[i]) 
		{
			std::cout << "(\"" << elem.keyWord << "\", " << elem.wordCount << ") ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

}

MyHashTable MyHashTable::substract(const MyHashTable& substractorObject) const
{
	if (substractorObject.getSize() == 0) return *this;

	MyHashTable result = MyHashTable();
	for (const row& fwdList : this->hashTableVector)
	{
		for (const MyHashTable::Element& element : fwdList)
		{
			std::string currentWord			= element.keyWord;
			size_t		currentWordCount	= element.wordCount;
			if (substractorObject.contains(currentWord)) 
			{
				size_t currentWordCountInSubtractor = substractorObject.at(currentWord);
				if (currentWordCount > currentWordCountInSubtractor) 
					result.insert(currentWord, currentWordCount - currentWordCountInSubtractor);
			}
			else
				result.addElement(currentWord, currentWordCount);
		}
	}

	return result;
}

size_t MyHashTable::countOfWords() const
{
	size_t wordsInHashTable = 0;

	for (const row& fwdList : this->hashTableVector)
	{
		for (const MyHashTable::Element& element : fwdList)
		{
			size_t		currentWordCount = element.wordCount;
			wordsInHashTable += currentWordCount;
		}
	}

	return wordsInHashTable;
}
