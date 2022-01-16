#include "interface.h"

#include <sstream>
#include <fstream>
#include <iostream>

const std::string HORIZONTAL_TAB_AS_STRING = std::string("") + '\\' + 't';
const std::string NEWLINE_AS_STRING = std::string("") + '\\' + 'n';
const std::string VERTICALTAB_AS_STRING = std::string("") + '\\' + 'v';
const std::string CARRIAGE_RETURN_AS_STRING = std::string("") + '\\' + 'r';

struct MyHashTable::HashTableReader {

private:
	static void eraseAllOccurancesOfSubstring(std::string& str, const std::string& substrToErase)
	{
		size_t pos;
		while ((pos = str.find(substrToErase)) != std::string::npos)
		{
			str.erase(pos, substrToErase.length());
		}
	}

	static void formatLine(std::string& str)
	{
		eraseAllOccurancesOfSubstring(str, HORIZONTAL_TAB_AS_STRING);
		eraseAllOccurancesOfSubstring(str, NEWLINE_AS_STRING);
		eraseAllOccurancesOfSubstring(str, VERTICALTAB_AS_STRING);
		eraseAllOccurancesOfSubstring(str, CARRIAGE_RETURN_AS_STRING);
	}

	static void insertNonWhitespaceStringsInHashTable(const std::string& line, MyHashTable& h) {
		std::stringstream  stream(line);
		std::string        oneWord;

		while (stream >> oneWord) {
			std::cout << "word: " << oneWord << " added to multiset." << std::endl;
			h.insert(oneWord);
		}
	}

public:

	static MyHashTable readHashTableFromFile(const std::string& _filename)
	{
		std::fstream inputFile(_filename);
		if (inputFile) {
			MyHashTable hashTableResult = MyHashTable();

			std::string line;
			while (std::getline(inputFile, line)) {
				HashTableReader::formatLine(line); // necessarry, because stringstream(string)
				std::cout << "Line after formatting: " << line << std::endl;
				insertNonWhitespaceStringsInHashTable(line, hashTableResult);
			}
			inputFile.close();
			std::cout << "Words from " << _filename << " succesfully loaded!" << std::endl;
			return hashTableResult;
		}
		else {
			inputFile.close();
		}
	}

	// TODO: fix that shit inputStream is empty
	static MyHashTable readHashTableFromIstream(std::istream& inputStream) 
	{
		if (inputStream) 
		{
			MyHashTable hashTableResult = MyHashTable();
			std::string line;
			while (std::getline(inputStream, line)) {
				HashTableReader::formatLine(line); // necessarry, because stringstream(string)
				std::cout << "Line after formatting: " << line << std::endl;
				insertNonWhitespaceStringsInHashTable(line, hashTableResult);
			}
			std::cout << "Words from istream succesfully loaded!" << std::endl;
			return hashTableResult;
		}
	}
};

void WordsMultiset::add(const std::string& word, size_t times) {
	this->hashTable.insert(word, times);
}

bool WordsMultiset::contains(const std::string& word) const
{
	return this->hashTable.contains(word);
}

size_t WordsMultiset::countOf(const std::string& word) const
{
	if (this->contains(word)) return this->hashTable.at(word);

	return 0;
}

size_t WordsMultiset::countOfUniqueWords() const
{
	return this->hashTable.getSize();
}

std::multiset<std::string> WordsMultiset::words() const
{
	std::multiset<std::string> result;

	using row = std::forward_list<MyHashTable::Element>;
	for (size_t i{0}; i < this->hashTable.capacity(); i++)
	{
		for (const MyHashTable::Element& element: this->hashTable.getConstRefRowAt(i))
		{
			for (size_t wordCount{element.wordCount}; wordCount > 0; wordCount--)
			{
				result.insert(element.keyWord);
			}
		}
	}

	return result;
}

WordsMultiset::WordsMultiset()
{
	this->hashTable = MyHashTable();
}

WordsMultiset::WordsMultiset(const std::string& filename)
{
	this->hashTable = MyHashTable::HashTableReader::readHashTableFromFile(filename);
}

WordsMultiset::WordsMultiset(std::istream& stream)
{
	this->hashTable = MyHashTable::HashTableReader::readHashTableFromIstream(stream);
}

void WordsMultiset::print()
{
	this->hashTable.print();
}

WordsMultiset WordsMultiset::substract(const WordsMultiset& substractorObject)
{
	WordsMultiset result = WordsMultiset();

	MyHashTable hashTableUnderneath = this->hashTable.substract(substractorObject.hashTable);

	result.hashTable = hashTableUnderneath;

	return result;
}

ComparisonReport Comparator::compare(std::istream& a, std::istream& b)
{
	ComparisonReport result = ComparisonReport();

	if (a.peek() == EOF && b.peek() == EOF) 
	{
		return result;
	}

	WordsMultiset m1 = WordsMultiset();
		std::string word;
	while (a >> word) 
	{
		m1.add(word);
	}

	WordsMultiset m2 = WordsMultiset();
	while (b >> word) 
	{
		m2.add(word);
	}

	result.commonWords = WordsMultiset();

	// Todo: find another way of iterating through the WordsMultiset Object
	for (const std::string& word : m1.words())
	{
		if (!result.commonWords.contains(word) && keyExistsInBoth(word, m1, m2)) 
		{
			size_t wordOccuranceInM1 = m1.countOf(word);
			size_t wordOccuranceInM2 = m2.countOf(word);

			size_t wordOccuranceInReport = std::min(wordOccuranceInM1, wordOccuranceInM2);

			result.commonWords.add(word, wordOccuranceInReport);
		}
	}

	result.uniqueWords[0] = m1.substract(result.commonWords);
	result.uniqueWords[1] = m2.substract(result.commonWords);

	return result;
}

bool Comparator::keyExistsInBoth(const std::string& keyWord, const WordsMultiset& wm1, const WordsMultiset& wm2)
{
	return (wm1.contains(keyWord) && wm2.contains(keyWord));
}
