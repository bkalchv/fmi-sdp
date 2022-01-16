#include "interface.h"

#include <sstream>
#include <fstream>
#include <iostream>

//const std::string HORIZONTAL_TAB_AS_STRING = std::string("") + '\\' + 't';
//const std::string NEWLINE_AS_STRING = std::string("") + '\\' + 'n';
//const std::string VERTICALTAB_AS_STRING = std::string("") + '\\' + 'v';
//const std::string CARRIAGE_RETURN_AS_STRING = std::string("") + '\\' + 'r';

struct MyHashTable::HashTableReader {

private:
	//static void eraseAllOccurancesOfSubstring(std::string& str, const std::string& substrToErase)
	//{
	//	size_t pos;
	//	while ((pos = str.find(substrToErase)) != std::string::npos)
	//	{
	//		str.erase(pos, substrToErase.length());
	//	}
	//}

	//static void formatLine(std::string& str)
	//{
	//	eraseAllOccurancesOfSubstring(str, HORIZONTAL_TAB_AS_STRING);
	//	eraseAllOccurancesOfSubstring(str, NEWLINE_AS_STRING);
	//	eraseAllOccurancesOfSubstring(str, VERTICALTAB_AS_STRING);
	//	eraseAllOccurancesOfSubstring(str, CARRIAGE_RETURN_AS_STRING);
	//}

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

	static MyHashTable readHashTableFromIstream(std::istream& inputStream) 
	{
		if (inputStream) 
		{
			MyHashTable hashTableResult = MyHashTable();
			std::string line;
			while (std::getline(inputStream, line)) {
				insertNonWhitespaceStringsInHashTable(line, hashTableResult);
			}
			std::cout << "Words from istream succesfully loaded!" << std::endl;
			return hashTableResult;
		}
	}
};

void WordsMultiset::add(const std::string& word, size_t times) {
	this->hashTable.insert(word, times);
	this->wordsAmount += times;
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
	this->wordsAmount = 0;
}

WordsMultiset::WordsMultiset(const std::string& filename)
{
	this->hashTable = MyHashTable::HashTableReader::readHashTableFromFile(filename);
	this->wordsAmount = 0;
}

WordsMultiset::WordsMultiset(std::istream& stream)
{
	this->hashTable = MyHashTable::HashTableReader::readHashTableFromIstream(stream);
	this->wordsAmount = 0;
}

void WordsMultiset::print()
{
	this->hashTable.print();
}

WordsMultiset WordsMultiset::substract(const WordsMultiset& substractorObject) const
{
	WordsMultiset result = WordsMultiset();

	MyHashTable hashTableUnderneath = this->hashTable.substract(substractorObject.hashTable);

	result.hashTable = hashTableUnderneath;
	result.wordsAmount = result.hashTable.countOfWords();

	return result;
}

size_t WordsMultiset::getHashTableCapacity() const
{
	return this->hashTable.capacity();
}

using hashTableRow = std::forward_list<MyHashTable::Element>;
hashTableRow WordsMultiset::getHashTableRowAt(size_t index) const
{
	return this->hashTable.getConstRefRowAt(index);
}

ComparisonReport Comparator::compare(std::istream& a, std::istream& b)
{
	ComparisonReport result = ComparisonReport();

	if (a.peek() == EOF && b.peek() == EOF)
	{
		std::cout << "Comparator: EMPTY ISTREAM input" << std::endl;
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

	using hashTableRow = std::forward_list<MyHashTable::Element>;
	for (size_t index{0}; index < m1.getHashTableCapacity(); ++index)
	{
		hashTableRow currentRow = m1.getHashTableRowAt(index);
		for (const MyHashTable::Element& element : currentRow) 
		{
			std::string currentWord = element.keyWord;
			if (!result.commonWords.contains(currentWord) && keyExistsInBoth(currentWord, m1, m2))
			{
				size_t wordOccuranceInM1 = m1.countOf(currentWord);
				size_t wordOccuranceInM2 = m2.countOf(currentWord);

				size_t wordOccuranceInReport = std::min(wordOccuranceInM1, wordOccuranceInM2);

				result.commonWords.add(currentWord, wordOccuranceInReport);
			}
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

void ComparisonReport::printReport()
{
	size_t commonWordsAmount = this->commonWords.wordsAmount;
	size_t firstFileWordsAmount = commonWordsAmount + this->uniqueWords[0].wordsAmount;
	size_t secondFileWordsAmount = commonWordsAmount + this->uniqueWords[1].wordsAmount;

	size_t wholeNumberPercentageOfOverlayFile1toFile2 = (size_t) ((((float)commonWordsAmount / (float)firstFileWordsAmount)) * 100.00f);
	size_t wholeNumberPercentageOfOverlayFile2toFile1 = (size_t) ((((float)commonWordsAmount / (float)secondFileWordsAmount)) * 100.00f);

	std::cout << "File1 contains " << firstFileWordsAmount << " words. " << commonWordsAmount << " of them can be found in File2 (" << wholeNumberPercentageOfOverlayFile1toFile2 << "%)" << std::endl;
	std::cout << "File2 contains " << secondFileWordsAmount << " words. " << commonWordsAmount << " of them can be found in File1 (" << wholeNumberPercentageOfOverlayFile2toFile1 << "%)" << std::endl << std::endl;
}
