#ifndef HASHTABLE_HEADER
#define HASHTABLE_HEADER

#include <iostream>
#include <vector>
#include <forward_list>
#include <string>

class MyHashTable {
	
public:
	struct Element {
		std::string keyWord;
		size_t		wordCount;

		Element(const std::string& word, size_t wordCount = 0) : keyWord(word), wordCount(wordCount) {}
	};

public:
	MyHashTable();
	MyHashTable(size_t);
	MyHashTable(const MyHashTable&) = default;
	~MyHashTable() = default;

	bool contains(const std::string&) const;

	bool insert(const std::string&, size_t);
	bool insert(const Element&);
	void clear();

	size_t& at(const std::string&);
	const size_t at(const std::string&) const;

	//Returns the amount of elements in the HashTable
	size_t	getSize() const;
	size_t	capacity() const;
	float	calculateLoadFactor() const;
	void	rehash(size_t);
	void	reserve(size_t);
	void	print();
	MyHashTable substract(const MyHashTable&) const;

	struct HashTableReader;

	//Returns the amount of words in the HashTable
	size_t countOfWords() const;

private:
	using row = std::forward_list<Element>;
	std::vector<row> hashTableVector;
	size_t size;
	float  maxLoadFactor;

private:
	size_t  calculateHashIndex(const std::string&);
	size_t  calculateHashIndex(const std::string&) const;
	Element* findElement(const std::string&);
	const Element* findElement(const std::string&) const;
	bool wouldAdditionRequireRehashing() const;
	Element* addElement(const std::string&, size_t);
	Element* addElement(const Element&);

public:
	const row& getConstRefRowAt(size_t) const;
};

#endif
