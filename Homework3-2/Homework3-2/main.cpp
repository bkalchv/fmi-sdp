#include "interface.h"

#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <string>

const std::string HORIZONTAL_TAB_AS_STRING = std::string("") + '\\' + 't';
const std::string NEWLINE_AS_STRING = std::string("") + '\\' + 'n';
const std::string VERTICALTAB_AS_STRING = std::string("") + '\\' + 'v';
const std::string CARRIAGE_RETURN_AS_STRING = std::string("") + '\\' + 'r';

void eraseAllOccurancesOfSubstring(std::string& str, const std::string& substrToErase)
{
	size_t pos;
	while ((pos = str.find(substrToErase)) != std::string::npos)
	{
		str.erase(pos, substrToErase.length());
	}
}

void formatLine(std::string& str)
{
	eraseAllOccurancesOfSubstring(str, HORIZONTAL_TAB_AS_STRING);
	eraseAllOccurancesOfSubstring(str, NEWLINE_AS_STRING);
	eraseAllOccurancesOfSubstring(str, VERTICALTAB_AS_STRING);
	eraseAllOccurancesOfSubstring(str, CARRIAGE_RETURN_AS_STRING);
}


std::stringstream sstreamOfFile(const std::string& _filename) {
	std::stringstream result;

	std::fstream inputFile(_filename);
	if (inputFile) 
	{
		std::string line;
		while (std::getline(inputFile, line)) {
			formatLine(line); // necessarry, because stringstream(string)
			std::stringstream  stream(line);
			
			std::string        oneWord;
			while (stream >> oneWord) {
				result << oneWord << " ";
			}
		}
		inputFile.close();
		std::cout << "Words from " << _filename << " succesfully streamed!" << std::endl;
		return result;
	}
	else {
		inputFile.close();
	}
}


int main() 
{

	//WordsMultiset ms("text.txt");

	std::string filename1 = "text.txt";
	std::ifstream str1;
	str1.open(filename1);
	//::stringstream buf = sstreamOfFile(filename1);


	std::string filename2 = "text1.txt";
	std::ifstream str2;
	str2.open(filename2);

	//std::stringstream buf1 = sstreamOfFile(filename2);

	Comparator c;

	ComparisonReport cr = c.compare(str1, str2);
	cr.commonWords.print();
	cr.uniqueWords[0].print();
	cr.uniqueWords[1].print();

	return 0;
}