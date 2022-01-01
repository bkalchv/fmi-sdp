#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <list>
#include <vector>

#include "interface.h"

static std::vector<string> commands =
{
	"help",
	"load -your_object_name- -your_filename_-(optional)",
	"save -your_object_name- -your_filename_-(optional)",
	"find",
	"num_subordinates",
	"manager",
	"num_employees",
	"overloaded",
	"join",
	"fire",
	"hire",
	"salary",
	"incorporate",
	"modernize",
	"exit"
};

std::vector<std::pair<std::string, Hierarchy>> hierarchies;

bool hierarchiesContainObjectWithName(const string& _objectName) {
	for (const std::pair<std::string, Hierarchy>& p : hierarchies) {
		if (p.first == _objectName) return true;
	}
	return false;
}

void printHierarchyWithObjectName(const string& _objectName) {
	for (const std::pair<std::string, Hierarchy>& p : hierarchies) {
		if (p.first == _objectName) std::cout << p.first << ":\n" << p.second.print() << std::endl;
	}
}

void saveHierarchyWithObjectNameInFilewWithName(const string& _objectName, const string& _filename) {
	std::ofstream fileToWriteTo;
	fileToWriteTo.open(_filename);
	for (const std::pair<std::string, Hierarchy>& p : hierarchies) {
		if (p.first == _objectName) fileToWriteTo << p.second.print();
	}
	fileToWriteTo.close();
}

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
	static std::pair<std::string, Hierarchy> readHierarchyFromFile(const std::string& _hierarchyName, const std::string& _filename) {
		std::fstream inputFile(_filename);
		if (inputFile) {
			Hierarchy	hierarchy		= Hierarchy();
			std::string line;
			std::string delimiter = "-";
			while (std::getline(inputFile, line)) {

				if (isValidInputFormat(line)) {
					std::string stringUntilDelimiter = line.substr(0, line.find(delimiter));
					std::string managerName = removeWhitespacesFromString(stringUntilDelimiter);
					
					std::string stringAfterDelimiter = line.substr(line.find(delimiter) + delimiter.length());
					std::string subordinateName = removeWhitespacesFromString(stringAfterDelimiter);
					hierarchy.addToHierachy(managerName, subordinateName);
					// TODO: Check if first token already exists before calling addToHierarchy
					// it is already checking in addToHierarchy
					// maybe here is the place to print it out tho and stop reading lines further
				}
				else {
					std::cout << "Invalid input format!" << std::endl;
				}
			}
			inputFile.close();
			std::cout << _hierarchyName << " succesfully loaded!" << std::endl;
			return std::pair<std::string, Hierarchy>(_hierarchyName, hierarchy);
		}
		else {
			inputFile.close();
			std::cout << _filename << " could not be open."<< std::endl;
			std::cout << _filename << " possibly not in the same directory or non-existent." << std::endl;
		}	
	}
};

struct MenuItem {
	string commandName;

	MenuItem(const string& _name) {
		
	};
};

struct ConsoleInputReader {

private:

	static unsigned int countNonWhitespaceStrings(const string& str) {
		std::stringstream  stream(str);
		std::string        oneWord;
		unsigned int       count = 0;

		while (stream >> oneWord) { count++; }
		return count;
	}
	
	static bool isValidObjectName(const string& _objectNameCandidate) {
		if (_objectNameCandidate.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_") != std::string::npos) return false;

		return true;
	}

	static bool isValidFilename(const string& _filename) {
		size_t fileExtensionDelimiter = _filename.find(".");
		if (fileExtensionDelimiter == string::npos) return false;
		string extension = _filename.substr(fileExtensionDelimiter);
		if (extension.empty()) return false;
		if (extension != ".txt") return false;

		return true;
	}

	static bool isValidUserInputLoad(const std::vector<string>& tokenizedInput) {
		if (tokenizedInput.size() != 2 && tokenizedInput.size() != 3) return false;
		else {
			if (!isValidObjectName(tokenizedInput[1])) {
				std::cout << "Unsupported object name format!\n(Object names may contain characters from A-Z, numbers from 0-9 and '_')\n";
				return false;
			}
			
			if (tokenizedInput.size() == 3 && !isValidFilename(tokenizedInput[2])) {
				std::cout << "Invalid filename input!\n(Only .txt files allowed!)\n";
				return false;
			}
		}

		return true;
	}

	static bool isValidUserInputSave(const std::vector<string>& tokenizedInput) {
		if (tokenizedInput.size() != 2 && tokenizedInput.size() != 3) return false;
		else {
			if (!isValidObjectName(tokenizedInput[1])) {
				std::cout << "Unsupported object name format!\n(Object names may contain characters from A-Z, numbers from 0-9 and '_')\n";
				return false;
			}

			if (!isValidFilename(tokenizedInput[2])) {
				std::cout << "Invalid filename input!\n(Only .txt files allowed!)\n";
				return false;
			}
		}

		return true;
	}

	static bool isValidUserInputFind(const std::string& input) {
		unsigned int nonWhiteSpacesStringsInInput = countNonWhitespaceStrings(input);
		if (nonWhiteSpacesStringsInInput != 3) return false;

		return true;
	}

	static bool isValidUserInputNumSubordinates(const std::string& input) {
		unsigned int nonWhiteSpacesStringsInInput = countNonWhitespaceStrings(input);
		if (nonWhiteSpacesStringsInInput != 3) return false;

		return true;
	}

	static bool isValidUserInputManager(const std::string& input) {
		unsigned int nonWhiteSpacesStringsInInput = countNonWhitespaceStrings(input);
		if (nonWhiteSpacesStringsInInput != 3) return false;

		return true;
	}

	static bool isValidUserInputNumEmployees(const std::string& input) {
		unsigned int nonWhiteSpacesStringsInInput = countNonWhitespaceStrings(input);
		if (nonWhiteSpacesStringsInInput != 2) return false;

		return true;
	}

	static bool isValidUserInputOverloaded(const std::string& input) {
		unsigned int nonWhiteSpacesStringsInInput = countNonWhitespaceStrings(input);
		if (nonWhiteSpacesStringsInInput != 2) return false;

		return true;
	}

	static bool isValidUserInputJoin(const std::string& input) {
		unsigned int nonWhiteSpacesStringsInInput = countNonWhitespaceStrings(input);
		if (nonWhiteSpacesStringsInInput != 3) return false;

		return true;
	}

	static bool isValidUserInputFire(const std::string& input) {
		unsigned int nonWhiteSpacesStringsInInput = countNonWhitespaceStrings(input);
		if (nonWhiteSpacesStringsInInput != 3) return false;

		return true;
	}

	static bool isValidUserInputHire(const std::string& input) {
		unsigned int nonWhiteSpacesStringsInInput = countNonWhitespaceStrings(input);
		if (nonWhiteSpacesStringsInInput != 4) return false;

		return true;
	}

	static bool isValidUserInputSalary(const std::string& input) {
		unsigned int nonWhiteSpacesStringsInInput = countNonWhitespaceStrings(input);
		if (nonWhiteSpacesStringsInInput != 3) return false;

		return true;
	}

	static bool isValidUserInputIncorporate(const std::string& input) {
		unsigned int nonWhiteSpacesStringsInInput = countNonWhitespaceStrings(input);
		if (nonWhiteSpacesStringsInInput != 2) return false;

		return true;
	}

	static bool isValidUserInputModernize(const std::string& input) {
		unsigned int nonWhiteSpacesStringsInInput = countNonWhitespaceStrings(input);
		if (nonWhiteSpacesStringsInInput != 2) return false;

		return true;
	}

	static bool isValidUserInputExit(const std::string& input) {

		return input == "exit";
	}

	static std::vector<string> tokenizeInput(const std::string& input) {
		std::vector<string> tokenizedInput;

		std::stringstream inputStringStream(input);
		std::string token;
		while (getline(inputStringStream, token, ' ')) {
			tokenizedInput.push_back(token);
		}

		return tokenizedInput;
	}

public:

	static void readInput() {
		string input;
		
		while (std::getline(std::cin, input)) {
			std::vector<string> tokenizedInput = tokenizeInput(input);
			if (!tokenizedInput.empty()) {
				std::string command = tokenizedInput[0];

				if (tokenizedInput[0] == "help") {
					for (const string& command : commands) std::cout << "- " << command << std::endl;
				}
				else if (tokenizedInput[0] == "load" && tokenizedInput.size() == 3) {
					if (isValidUserInputLoad(tokenizedInput)) {
						string	objectName = tokenizedInput[1];
						string	filename = tokenizedInput[2];
						hierarchies.push_back(HierarchyFileReader::readHierarchyFromFile(objectName, filename));
					}
					else {
						std::cout << "Invalid input for load command! Try again!" << std::endl;
					}
				}
				else if (tokenizedInput[0] == "load" && tokenizedInput.size() == 2) { // TODO:
					// read until ctrl+z or ctrl+d
				}
				else if (tokenizedInput[0] == "save" && tokenizedInput.size() == 3) {
					if (isValidUserInputLoad(tokenizedInput)) {
						string	objectName = tokenizedInput[1];
						string	filename = tokenizedInput[2];
						if (hierarchiesContainObjectWithName(objectName)) {
							saveHierarchyWithObjectNameInFilewWithName(objectName, filename);

						}
						else {
							std::cout << "No hierarchy with " << objectName << " object name found!" << std::endl;
						}
					}
					else {
						std::cout << "Invalid input for save command! Try again!" << std::endl;
					}
				}
				else if (tokenizedInput[0] == "save" && tokenizedInput.size() == 2) {
					if (isValidUserInputLoad(tokenizedInput)) {
						string	objectName = tokenizedInput[1];
						if (hierarchiesContainObjectWithName(objectName)) {
							printHierarchyWithObjectName(objectName);
						}
						else {
							std::cout << "No hierarchy with " << objectName << " object name found!" << std::endl;
						}
					}
					else {
						std::cout << "Invalid input for save command! Try again!" << std::endl;
					}
				}
				else if (tokenizedInput[0] == "find") { // TODO:

				}
				else if (tokenizedInput[0] == "num_subordinates") { // TODO:

				}
				else if (tokenizedInput[0] == "manager") { // TODO:

				}
				else if (tokenizedInput[0] == "num_employees") { // TODO:

				}
				else if (tokenizedInput[0] == "overloaded") { // TODO:

				}
				else if (tokenizedInput[0] == "join") { // TODO:

				}
				else if (tokenizedInput[0] == "fire") { // TODO:

				}
				else if (tokenizedInput[0] == "hire") { // TODO:

				}
				else if (tokenizedInput[0] == "salary") { // TODO:

				} 
				else if (tokenizedInput[0] == "incorporate") { // TODO:

				}
				else if (tokenizedInput[0] == "modernize") { // TODO:

				}
				else if (tokenizedInput[0] == "exit") {
					exit(1);
				}
				else {
					std::cout << "Such command doesnt exist! (Type help to see the list of commands available)" << std::endl;
				}
			}
		}


		// tokenize input
		// first token - command keyword
		// call isValidUserInput_COMMAND_ based on the first token
		// repeat until there's a valid input or exit command was called

		
		//while (!isValidUserInput(input)) {
		//	system("CLS");
		//	std::cout << "Invalid input. Try again!" << std::endl;
		//	// std::getline(std::cin, input);
		//}
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
	
	//Hierarchy lozenec = HierarchyFileReader::readHierarchyFromFile("lozenec.txt");
	//lozenec.modernize();
	//std::cout << lozenec.print() << std::endl;

	std::cout << "Welcome to " << THE_BOSS_NAME << "'s HierarchyManager!" << std::endl;
	std::cout << "status: waiting for input" << std::endl;
	std::cout << "*In case you don't know what to do: type 'help'" << std::endl;

	ConsoleInputReader::readInput();

	return 0;
}