#include "interface.h"
#include "hashTable.h"
#include <sstream>
#include <iostream>

static unsigned int countNonWhitespaceStrings(const std::string& str) {
	std::stringstream  stream(str);
	std::string        oneWord;
	unsigned int       count = 0;

	while (stream >> oneWord) { 
		count++; 
		std::cout << oneWord << std::endl;
	}
	return count;
}


static void insertNonWhitespaceStringsInHashTable(const std::string& str, MyHashTable& h) {
	std::stringstream  stream(str);
	std::string        oneWord;
	//unsigned int       count = 0;

	while (stream >> oneWord) {
		std::cout << oneWord << std::endl;
		h.insert(oneWord);
	}
}

int main() {

	//countNonWhitespaceStrings("1234 \t\t (*abc*(  \r  \n abd \r\n");

	MyHashTable h = MyHashTable();
	insertNonWhitespaceStringsInHashTable("1234 1234 1234\t\t (*abc*(  \r  \n abd \r\n 1 2 3 4 5 6 7 8 9 10", h);
	h.print();

	return 0;
}