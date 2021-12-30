#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

#include "interface.h"

struct HierarchyFileReader { 
private:
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

	static unsigned int countNonWhitespaceStrings(const std::string& str) {
		std::stringstream  stream(str);
		std::string        oneWord;
		unsigned int       count = 0;

		while (stream >> oneWord) { count++; }
		return count;
	}

	static std::string removeWhitespacesFromString(std::string str) {
		str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
		return str;
	}

	static bool isValidInputFormat(const std::string& inputLine) {
		std::string expectedDelimiter = "-";
		if (inputLine.find(expectedDelimiter) == std::string::npos) return false;
		if (!containsDelimiterOnce(inputLine, expectedDelimiter)) return false;

		std::string firstToken = inputLine.substr(0, inputLine.find(expectedDelimiter));
		if (firstToken.size() == 0) return false;

		if (countNonWhitespaceStrings(firstToken) != 1) return false;
		
		std::string secondToken = inputLine.substr(inputLine.find(expectedDelimiter) + expectedDelimiter.length());
		if (secondToken.size() == 0) return false;

		if (countNonWhitespaceStrings(secondToken) != 1) return false;

		return true;
	}

public:
	static Hierarchy readHierarchyFromFile(const std::string& _filename) {
		std::fstream inputFile(_filename);
		if (inputFile) {
			Hierarchy result = Hierarchy();
			std::string line;
			std::string delimiter = "-";
			while (std::getline(inputFile, line)) {

				if (isValidInputFormat(line)) {
					std::string stringUntilDelimiter = line.substr(0, line.find(delimiter));
					std::string managerName = removeWhitespacesFromString(stringUntilDelimiter);
					
					std::string stringAfterDelimiter = line.substr(line.find(delimiter) + delimiter.length());
					std::string subordinateName = removeWhitespacesFromString(stringAfterDelimiter);
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
	//Hierarchy h = HierarchyFileReader::readHierarchyFromFile("HierarchyA.txt");
	//std::cout << h.manager("Mariya") << std::endl;
	//std::cout << h.print() << std::endl;
	//std::cout << "Employees amount in hierarchy: " << h.num_employees() << std::endl;
	//std::cout << "Employees amount in hierarchy: " << h.num_employees_recursively() << std::endl; -> recursion debugging 
	//std::cout << "Overloaded amount: " << h.num_overloaded() << std::endl;
	//std::cout << "Height: " << h.longest_chain() << std::endl;
	//Hierarchy h1 = h;
	//std::cout << h1.print() << std::endl;

	//h.fire("Chocho");
	//std::cout << h.print() << std::endl;

	//h.fire("Bogdan");
	//std::cout << h.print() << std::endl;

	//h.hire("Chocho", "Uspeshnia");
	//std::cout << h.print() << std::endl;

	//h.hire("Mariya", "Uspeshnia");
	//std::cout << h.print() << std::endl;
	//h.incorporate();
	//std::cout << "After incorporation: " << std::endl;
	//std::cout << h.print() << std::endl;


	//Hierarchy a = HierarchyFileReader::readHierarchyFromFile("Hierarchy1.txt");
	//Hierarchy b = HierarchyFileReader::readHierarchyFromFile("Hierarchy2.txt");
	//Hierarchy c = a.join(b);

	//std::cout << c.print() << std::endl;

	//Hierarchy a1 = HierarchyFileReader::readHierarchyFromFile("HierarchyDiscordExample1.txt");
	//Hierarchy b1 = HierarchyFileReader::readHierarchyFromFile("HierarchyDiscordExample2.txt");
	//Hierarchy c1 = a1.join(b1);
	//Hierarchy c2 = b1.join(a1);
	//bidirectional join
	//(c1.print() == c2.print()) ? std::cout << "TRUE\n" : std::cout << "FALSE\n";

	//std::cout << c1.print() << std::endl;

	/*int UspeshniaLevel = a1.getLevelOfNodeByName("Uspeshnia");
	int a1BLevel = a1.getLevelOfNodeByName("B");
	int a1CLevel = a1.getLevelOfNodeByName("C");
	int a1DLevel = a1.getLevelOfNodeByName("D");
	int a1FLevel = a1.getLevelOfNodeByName("F");
	int a1QLevel = a1.getLevelOfNodeByName("Q");

	int b1UspeshniaLevel = b1.getLevelOfNodeByName("Uspeshnia");
	int b1BLevel = b1.getLevelOfNodeByName("B");
	int b1CLevel = b1.getLevelOfNodeByName("C");
	int b1DLevel = b1.getLevelOfNodeByName("D");
	int b1FLevel = b1.getLevelOfNodeByName("F");
	int b1QLevel = b1.getLevelOfNodeByName("Q");*/

	// join tests
	//Hierarchy testLozenecNew	= HierarchyFileReader::readHierarchyFromFile("lozenecNew.txt");
	//Hierarchy testLozenec		= HierarchyFileReader::readHierarchyFromFile("lozenec.txt");
	//Hierarchy testLarge			= HierarchyFileReader::readHierarchyFromFile("large.txt");
	//
	//Hierarchy l2 = testLozenec.join(testLozenec);
	//(l2.print() == testLozenec.print()) ? std::cout << "TRUE\n" : std::cout << "FALSE\n";

	//(testLarge.print() == testLarge.join(testLarge).print()) ? std::cout << "TRUE\n" : std::cout << "FALSE\n";

	//Hierarchy joined = testLozenecNew.join(testLozenec);
	//std::cout << joined.print() << std::endl;
	//std::cout << "Uspeshnia-Gosho\nUspeshnia-Misho\nUspeshnia-MishoPetrov\nUspeshnia-Slavi\nGosho-Dancho\nGosho-Pesho\nMishoPetrov-Slav\nSlavi-Slav1\nSlavi-Slav2\nDancho-Boris\nDancho-Kamen\nPesho-Alex\nSlav1-Mecho\nMecho-Q12Adl\n" << std::endl;
	//std::cout << (joined.print() == "Uspeshnia-Gosho\nUspeshnia-Misho\nUspeshnia-MishoPetrov\nUspeshnia-Slavi\nGosho-Dancho\nGosho-Pesho\nMishoPetrov-Slav\nSlavi-Slav1\nSlavi-Slav2\nDancho-Boris\nDancho-Kamen\nPesho-Alex\nSlav1-Mecho\nMecho-Q12Adl\n") << std::endl;
	//(joined.print().compare("Uspeshnia-Gosho\nUspeshnia-Misho\nUspeshnia-MishoPetrov\nUspeshnia-Slavi\nGosho-Dancho\nGosho-Pesho\nMishoPetrov-Slav\nSlavi-Slav1\nSlavi-Slav2\nDancho-Boris\nDancho-Kamen\nPesho-Alex\nSlav1-Mecho\nMecho-Q12Adl\n")) == 0 ? std::cout << "TRUE\n" : std::cout << "FALSE\n";
	//Hierarchy fromString("Uspeshnia-Gosho\nUspeshnia-Misho\nUspeshnia-MishoPetrov\nUspeshnia-Slavi\nGosho-Dancho\nGosho-Pesho\nMishoPetrov-Slav\nSlavi-Slav1\nSlavi-Slav2\nDancho-Boris\nDancho-Kamen\nPesho-Alex\nSlav1-Mecho\nMecho-Q12Adl\n");

	//std::string joinedStringRepresentation = joined.print();
	//std::string fromStringStringRepresentation	= fromString.print();
	//(joinedStringRepresentation.compare(fromStringStringRepresentation) == 0) ? std::cout << "FROM STRING: TRUE\n" : std::cout << "FROM STRING: FALSE\n";

	//(joined.num_employees() == 15) ? std::cout << "TRUE\n" : std::cout << "FALSE\n";
	//(joined.longest_chain() == 5) ? std::cout << "TRUE\n" : std::cout << "FALSE\n";
	//Hierarchy joined2 = testLarge.join(testLozenec);
	//(joined2.num_employees() == 13 + 24) ? std::cout << "TRUE\n" : std::cout << "FALSE\n";
	//(joined2.num_subordinates("Uspeshnia") == 4) ? std::cout << "TRUE\n" : std::cout << "FALSE\n";
	//(joined2.num_overloaded() == 2) ? std::cout << "TRUE\n" : std::cout << "FALSE\n";
	
	Hierarchy lozenec = HierarchyFileReader::readHierarchyFromFile("lozenec.txt");
	lozenec.modernize();
	std::cout << lozenec.print() << std::endl;

	return 0;
}