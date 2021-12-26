#include <iostream>
#include <fstream>
#include <sstream>

#include "interface.h"

struct HierarchyFileReader { 

	static bool containsDelimiterOnce(const std::string& inputLine, const std::string& delimiter) {
		size_t counterOfDelimiterOccurances = 0;
		size_t delimiterPosition = inputLine.find(delimiter, 0);
		while (delimiterPosition != std::string::npos) {
			counterOfDelimiterOccurances++;
			if (counterOfDelimiterOccurances > 1) break;
			delimiterPosition = inputLine.find(delimiter, delimiterPosition + 1);
		}
		if (counterOfDelimiterOccurances > 1) return false;
		if (counterOfDelimiterOccurances == 1) return true;
	}

	static bool isValidInputFormat(const std::string& inputLine) {
		std::string expectedDelimiter = "-";
		if (inputLine.find(expectedDelimiter) == std::string::npos) return false;
		if (!containsDelimiterOnce(inputLine, expectedDelimiter)) return false;

		std::string firstToken = inputLine.substr(0, inputLine.find(expectedDelimiter));
		if (firstToken.size() == 0) return false;
		
		std::string secondToken = inputLine.substr(inputLine.find(expectedDelimiter) + expectedDelimiter.length());
		if (secondToken.size() == 0) return false;

		return true;
	}

	static Hierarchy readHierarchyFromFile(const std::string& _filename) {
		std::fstream inputFile(_filename);
		if (inputFile) {
			Hierarchy result = Hierarchy();
			std::string line;
			std::string delimiter = "-";
			while (std::getline(inputFile, line)) {

				if (isValidInputFormat(line)) {
					std::string managerName = line.substr(0, line.find(delimiter));
					std::string subordinateName = line.substr(line.find(delimiter) + delimiter.length());
					result.addToHierachy(managerName, subordinateName);
					// TODO: Check if first token already exists before calling addToHierarchy
					// it is already checking in addToHierarchy
					// maybe here is the place to print it out tho and stop reading lines further
				}
				else {
					std::cout << "Invalid input format!" << std::endl;
				}
			}
			inputFile.close();
			return result;
		}
		else {
			inputFile.close();
			std::cout << _filename << " could not be open."<< std::endl;
		}	
	}
};

int main() {
	Hierarchy h = HierarchyFileReader::readHierarchyFromFile("HierarchyA.txt");
	//std::cout << h.manager("Mariya") << std::endl;
	std::cout << h.print() << std::endl;
	std::cout << "Employees amount in hierarchy: " << h.num_employees() << std::endl;
	std::cout << "Overloaded amount: " << h.num_overloaded() << std::endl;
	std::cout << "Height: " << h.longest_chain() << std::endl;
	//Hierarchy h1 = h;
	//std::cout << h1.print() << std::endl;

	return 0;
}