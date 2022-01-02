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
	"find -your_object_name- -your_employee_name-",
	"num_subordinates -your_object_name- -your_employee_name-",
	"manager -your_object_name- -your_employee_name-",
	"num_employees -your_object_name-",
	"num_overloaded -your_object_name-",
	"join -your_first_object_name- -your_second_object_name- -your_result_object_name-",
	"fire -your_object_name- -your_employee_name-",
	"hire -your_object_name- -your_employee_name- -employee's_manager_name-",
	"salary -your_object_name- -your_employee_name-",
	"incorporate -your_object_name-",
	"modernize -your_object_name-",
	"exit"
};

std::vector<std::pair<std::string, Hierarchy>>	hierarchies;
std::vector<bool>								isHierarchyLoadedAndUnsavedByIndex;

size_t getIndexOfHierarchyByName(const string& _objectName) {
	for (size_t hierarchyIndex{ 0 }; hierarchyIndex < hierarchies.size(); hierarchyIndex++) {
		if (hierarchies[hierarchyIndex].first == _objectName) return hierarchyIndex;
	}
}

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
		if (p.first == _objectName) { 
			fileToWriteTo << p.second.print(); 
			size_t currentObjectWithNameIndex = getIndexOfHierarchyByName(_objectName);
			isHierarchyLoadedAndUnsavedByIndex[currentObjectWithNameIndex] = true;
		}
		
	}
	fileToWriteTo.close();
}

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
					// Check if first token already exists before calling addToHierarchy
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
		} else {
			inputFile.close();
		}	
	}
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

	static bool isFileWithFilenameAccessible(const string& _filename) {
		std::fstream inputFile(_filename);
		if (inputFile) {
			return true;
			inputFile.close();
		}
		else {
			return false;
			inputFile.close();
		}
	}

	static bool isValidUserInputLoad(const std::vector<string>& tokenizedInput) {
		if (tokenizedInput.size() != 2 && tokenizedInput.size() != 3) 
			return false;
		else if (!isValidObjectName(tokenizedInput[1])) {
			std::cout << "Unsupported object name format!\n(Object names may contain characters from A-Z, numbers from 0-9 and '_')\n";
			return false;
		} else if (hierarchiesContainObjectWithName(tokenizedInput[1])) {
			std::cout << tokenizedInput[1] << " already loaded." << std::endl;
		} else if (tokenizedInput.size() == 3 && !isValidFilename(tokenizedInput[2])) {
				std::cout << "Invalid filename input!\n(Only .txt files allowed!)\n";
				return false;
		} else if (tokenizedInput.size() == 3 && !isFileWithFilenameAccessible(tokenizedInput[2])) {
				std::cout << tokenizedInput[2] << " could not be open." << std::endl;
				std::cout << tokenizedInput[2] << " possibly not in the same directory or non - existent." << std::endl;
			return false;
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

			if (tokenizedInput.size() == 3 && !isValidFilename(tokenizedInput[2])) {
				std::cout << "Invalid filename input!\n(Only .txt files allowed!)\n";
				return false;
			}
		}

		return true;
	}

	static bool isValidUserInputJoin(const std::vector<string>& tokenizedInput) {
		if (!isValidObjectName(tokenizedInput[1])) {
			std::cout << tokenizedInput[1] << " is invalid object name." << std::endl;
			return false;
		} 
		else if (!isValidObjectName(tokenizedInput[2])) {
			std::cout << tokenizedInput[2] << " is invalid object name." << std::endl;
			return false;
		} else if (!isValidObjectName(tokenizedInput[3])) {
			std::cout << tokenizedInput[3] << " is invalid object name." << std::endl;
			return false;
		} else if (hierarchiesContainObjectWithName(tokenizedInput[3])) {
			std::cout << tokenizedInput[3] << " already loaded." << std::endl;
			return false;
		} else if (!hierarchiesContainObjectWithName(tokenizedInput[1])) {
			std::cout << "No hierarchy with " << tokenizedInput[1] << " object name found! Maybe this hierarchy hasn't been loaded yet." << std::endl;
			return false;
		} else if (!hierarchiesContainObjectWithName(tokenizedInput[2])) {
			std::cout << "No hierarchy with " << tokenizedInput[2] << " object name found! Maybe this hierarchy hasn't been loaded yet." << std::endl;
			return false;
		}

		return true;
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
						isHierarchyLoadedAndUnsavedByIndex.push_back(false);
					}
					else {
						std::cout << "Invalid input for load command! Try again!" << std::endl;
					}
				}
				else if (tokenizedInput[0] == "load" && tokenizedInput.size() == 2) {
					// read until ctrl+z
					if (!isValidObjectName(tokenizedInput[1])) {
						std::cout << "Unsupported object name format!\n(Object names may contain characters from A-Z, numbers from 0-9 and '_')\n";
					}
					else {
						std::string hierarchyString;
						std::string delimiter = "-";
						std::string line;
						while (std::getline(std::cin, line))
						{
							if (!line.empty()) {
								// checks if input is good and if it could be added 
								if (HierarchyFileReader::isValidInputFormat(line)) {
									std::string stringUntilDelimiter = line.substr(0, line.find(delimiter));
									std::string managerName = HierarchyFileReader::removeWhitespacesFromString(stringUntilDelimiter);

									std::string stringAfterDelimiter = line.substr(line.find(delimiter) + delimiter.length());
									std::string subordinateName = HierarchyFileReader::removeWhitespacesFromString(stringAfterDelimiter);

									std::string lineInFile = managerName.append(delimiter).append(subordinateName).append("\n");
									hierarchyString.append(lineInFile);
									// Check if first token already exists before calling addToHierarchy
									// it is already checking in addToHierarchy
									// maybe here is the place to print it out tho and stop reading lines further
								}
								else {
									std::cout << "Invalid input format!" << std::endl;
									// throw exception, instead of creating object?
								}
							}
						}
						try {
							string objectName			= tokenizedInput[1];
							Hierarchy hierarchyToAdd = Hierarchy(hierarchyString);

							hierarchies.push_back(std::pair<std::string, Hierarchy>(objectName, hierarchyToAdd));
							isHierarchyLoadedAndUnsavedByIndex.push_back(true);

							std::cout << objectName << " succesfully loaded!" << std::endl;
							std::cin.clear();

						} catch (const std::invalid_argument& ia) {
							std::cerr << ia.what() << std::endl;
							std::cin.clear();
						}
					}


				}
				else if (tokenizedInput[0] == "save" && tokenizedInput.size() == 3) {
					if (isValidUserInputSave(tokenizedInput)) {
						string	objectName = tokenizedInput[1];
						string	filename = tokenizedInput[2];
						if (hierarchiesContainObjectWithName(objectName)) {
							saveHierarchyWithObjectNameInFilewWithName(objectName, filename);
							std::cout << objectName << " saved." << std::endl;
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
					if (isValidUserInputSave(tokenizedInput)) {
						string	objectName = tokenizedInput[1];
						if (hierarchiesContainObjectWithName(objectName)) {
							printHierarchyWithObjectName(objectName);
						}
						else {
							std::cout << "No hierarchy with " << objectName << " object name found! Maybe this hierarchy hasn't been loaded yet." << std::endl;
						}
					}
					else {
						std::cout << "Invalid input for save command! Try again!" << std::endl;
					}
				}
				else if (tokenizedInput[0] == "find" && tokenizedInput.size() == 3) {
					string objectName	= tokenizedInput[1];
					string employeeName = tokenizedInput[2];
					if (hierarchiesContainObjectWithName(objectName)) {
						for (const std::pair<std::string, Hierarchy>& p : hierarchies) {
							if (p.first == objectName) {
								bool employeeFound = p.second.find(employeeName);
								if (employeeFound) {
									std::cout << employeeName << " is employed in " << objectName << "." << std::endl;
								}
								else
								{
									std::cout << "There's no employee with name " << employeeName << " in " << objectName << "." << std::endl;
								}
								break;
							}
						}
					}
					else {
						std::cout << "No hierarchy with " << objectName << " object name found! Maybe this hierarchy hasn't been loaded yet." << std::endl;
					}
				}
				else if (tokenizedInput[0] == "num_subordinates" && tokenizedInput.size() == 3) {
					string objectName = tokenizedInput[1];
					string employeeName = tokenizedInput[2];
					if (hierarchiesContainObjectWithName(objectName)) {
						for (const std::pair<std::string, Hierarchy>& p : hierarchies) {
							if (p.first == objectName) {
								bool employeeFound = p.second.find(employeeName);
								if (employeeFound) {
									int employeeSubordinates = p.second.num_subordinates(employeeName);
									std::cout << employeeName << " in " << objectName << " has " << employeeSubordinates << " subordinates." << std::endl;
								}
								else
								{
									std::cout << "There's no employee with name " << employeeName << " in " << objectName << "." << std::endl;
								}
								break;
							}
						}
					}
					else {
						std::cout << "No hierarchy with " << objectName << " object name found! Maybe this hierarchy hasn't been loaded yet." << std::endl;
					}
				}
				else if (tokenizedInput[0] == "manager" && tokenizedInput.size() == 3) {
					string objectName = tokenizedInput[1];
					string employeeName = tokenizedInput[2];
					if (hierarchiesContainObjectWithName(objectName)) {
						for (const std::pair<std::string, Hierarchy>& p : hierarchies) {
							if (p.first == objectName) {
								bool employeeFound = p.second.find(employeeName);
								if (employeeFound) {
									string employeeManagerName = p.second.manager(employeeName);
									if (employeeName == THE_BOSS_NAME) {
										std::cout << employeeName << " is on top of the food chain. Nobody can be his/her/their manager." << std::endl;
									}
									else {
										std::cout << "The manager of " << employeeName << " in " << objectName << " is " << employeeManagerName << "." << std::endl;
									}
								}
								else
								{
									std::cout << "There's no employee with name " << employeeName << " in " << objectName << "." << std::endl;
								}
								break;
							}
						}
					}
					else {
						std::cout << "No hierarchy with " << objectName << " object name found! Maybe this hierarchy hasn't been loaded yet." << std::endl;
					}
				}
				else if (tokenizedInput[0] == "num_employees" && tokenizedInput.size() == 2) {
					string objectName = tokenizedInput[1];
					if (hierarchiesContainObjectWithName(objectName)) {
						for (const std::pair<std::string, Hierarchy>& p : hierarchies) {
							if (p.first == objectName) {
								int employeesAmount = p.second.num_employees();
								std::cout << "There are " << employeesAmount << " employees in " << objectName << "." << std::endl;
								break;
							}
						}
					}
					else {
						std::cout << "No hierarchy with " << objectName << " object name found! Maybe this hierarchy hasn't been loaded yet." << std::endl;
					}
				}
				else if (tokenizedInput[0] == "overloaded" && tokenizedInput.size() == 2) {
					string objectName = tokenizedInput[1];
					if (hierarchiesContainObjectWithName(objectName)) {
						for (const std::pair<std::string, Hierarchy>& p : hierarchies) {
							if (p.first == objectName) {
								int overloadedEmployeesAmount = p.second.num_overloaded();
								if (overloadedEmployeesAmount == 0) {
									std::cout << "There are no overloaded employees in " << objectName << "." << std::endl;
								}
								else {
									std::cout << "There are " << overloadedEmployeesAmount << " overloaded employees in " << objectName << "." << std::endl;
								}
								break;
							}
						}
					}
					else {
						std::cout << "No hierarchy with " << objectName << " object name found! Maybe this hierarchy hasn't been loaded yet." << std::endl;
					}
				}
				else if (tokenizedInput[0] == "join" && tokenizedInput.size() == 4) { 
					if (isValidUserInputJoin(tokenizedInput)) {
						string firstObjectName	= tokenizedInput[1];
						size_t firstObjectIndex = getIndexOfHierarchyByName(firstObjectName);
						string secondObjectName = tokenizedInput[2];
						size_t secondObjectIndex = getIndexOfHierarchyByName(secondObjectName);
						string resultObjectOfJoinName = tokenizedInput[3];
						
						Hierarchy firstObjectHierarchy	= hierarchies[firstObjectIndex].second;
						Hierarchy secondObjectHierarchy = hierarchies[secondObjectIndex].second;

						// TODO: check if it'll be a valid join
						try {
							Hierarchy resultObjectHierarchy = firstObjectHierarchy.join(secondObjectHierarchy);
							hierarchies.push_back(std::pair<string, Hierarchy>(resultObjectOfJoinName, resultObjectHierarchy));
							isHierarchyLoadedAndUnsavedByIndex.push_back(false);
						} catch (const std::exception& e) {
							std::cerr << e.what() << std::endl;
							continue;
						}

					}
					else {
						std::cout << "Invalid input for Join command! Try again!" << std::endl;
					}
				}
				else if (tokenizedInput[0] == "fire" && tokenizedInput.size() == 3) {
					string objectName = tokenizedInput[1];
					string employeeName = tokenizedInput[2];
					if (hierarchiesContainObjectWithName(objectName)) {
						for (std::pair<std::string, Hierarchy>& p : hierarchies) {
							if (p.first == objectName) {
								bool wasEmployeeFiredSuccessfully = p.second.fire(employeeName);
								if (wasEmployeeFiredSuccessfully) {
									std::cout << employeeName << " was fired from " << objectName << "." << std::endl;
									size_t objectIndex = getIndexOfHierarchyByName(objectName);
									isHierarchyLoadedAndUnsavedByIndex[objectIndex] = true;
								}
								else {
									if (employeeName == THE_BOSS_NAME) std::cout << THE_BOSS_NAME << " can't be fired! The whole food chain will fall apart without him in the end." << std::endl;
									else
										std::cout << "There's no employee with name " << employeeName << " in " << objectName << "." << std::endl;
								}
								break;
							}
						}
					}
					else {
						std::cout << "No hierarchy with " << objectName << " object name found! Maybe this hierarchy hasn't been loaded yet." << std::endl;
					}
				}
				else if (tokenizedInput[0] == "hire" && tokenizedInput.size() == 4) {
					string objectName = tokenizedInput[1];
					string employeeToHireName = tokenizedInput[2];
					string managerName = tokenizedInput[3];
					if (hierarchiesContainObjectWithName(objectName)) {
						for (std::pair<std::string, Hierarchy>& p : hierarchies) {
							if (p.first == objectName) {
								bool wasEmployeeHiredSuccessfully = p.second.hire(employeeToHireName, managerName);
								if (wasEmployeeHiredSuccessfully) {
									std::cout << employeeToHireName << " was hired in " << objectName << "." << std::endl;
									size_t objectIndex = getIndexOfHierarchyByName(objectName);
									isHierarchyLoadedAndUnsavedByIndex[objectIndex] = true;
								}
								else {
										std::cout << "There's no employee with name " << managerName << " in " << objectName << "." << std::endl;
								}
							}
							break;
						}
					}
					else {
						std::cout << "No hierarchy with " << objectName << " object name found! Maybe this hierarchy hasn't been loaded yet." << std::endl;
					}
				}
				else if (tokenizedInput[0] == "salary" && tokenizedInput.size() == 3) {
					string objectName = tokenizedInput[1];
					string employeeName = tokenizedInput[2];
					if (hierarchiesContainObjectWithName(objectName)) {
						for (const std::pair<std::string, Hierarchy>& p : hierarchies) {
							if (p.first == objectName) {
								bool employeeFound = p.second.find(employeeName);
								if (employeeFound) {
									unsigned int employeeSalary = p.second.getSalary(employeeName);
									std::cout << "The salary of " << employeeName << " in " << objectName << " is " << employeeSalary << " BGN." << std::endl;
								}
								else
								{
									std::cout << "There's no employee with name " << employeeName << " in " << objectName << "." << std::endl;
								}
								break;
							}
						}
					}
					else {
						std::cout << "No hierarchy with " << objectName << " object name found! Maybe this hierarchy hasn't been loaded yet." << std::endl;
					}
				} 
				else if (tokenizedInput[0] == "incorporate" && tokenizedInput.size() == 2) { 
					string objectName = tokenizedInput[1];
					if (hierarchiesContainObjectWithName(objectName)) {
						for (std::pair<std::string, Hierarchy>& p : hierarchies) {
							if (p.first == objectName) {
								p.second.incorporate();
								std::cout << objectName << " incorporated." << std::endl;
								size_t objectIndex = getIndexOfHierarchyByName(objectName);
								isHierarchyLoadedAndUnsavedByIndex[objectIndex] = true;
								break;
							}
						}
					}
					else {
						std::cout << "No hierarchy with " << objectName << " object name found! Maybe this hierarchy hasn't been loaded yet." << std::endl;
					}
				}
				else if (tokenizedInput[0] == "modernize" && tokenizedInput.size() == 2) {
					string objectName = tokenizedInput[1];
					if (hierarchiesContainObjectWithName(objectName)) {
						for (std::pair<std::string, Hierarchy>& p : hierarchies) {
							if (p.first == objectName) {
								p.second.modernize();
								std::cout << objectName << " modernized." << std::endl;
								size_t objectIndex = getIndexOfHierarchyByName(objectName);
								isHierarchyLoadedAndUnsavedByIndex[objectIndex] = true;
								break;
							}
						}
					}
					else {
						std::cout << "No hierarchy with " << objectName << " object name found! Maybe this hierarchy hasn't been loaded yet." << std::endl;
					}
				}
				else if (tokenizedInput[0] == "exit") {
					// check if a certain hierarchy has been modified; if so ask where to save the modified hierarchy
					// done

					for (size_t i{ 0 }; i < isHierarchyLoadedAndUnsavedByIndex.size(); i++) {
						if (isHierarchyLoadedAndUnsavedByIndex[i]) {
							const std::pair<string, Hierarchy>& currentUnsavedHierarchy = hierarchies[i];
							std::cout << currentUnsavedHierarchy.first << " is modified, but not saved." << std::endl;
							std::cout << "Please enter a filename to save it to:" << std::endl;


								string filenameInput;
								do {
									std::cin >> filenameInput;
									std::cin.clear();
								} while (!isValidFilename(filenameInput));

								saveHierarchyWithObjectNameInFilewWithName(currentUnsavedHierarchy.first, filenameInput);
								std::cout << currentUnsavedHierarchy.first << " saved." << std::endl;
						}
					}

					exit(1);
				}
				else {
					std::cout << "Wrong command or command format! (Type help to see the list of commands available)" << std::endl;
				}
			}
		}


		// tokenize input
		// first token - command keyword
		// call isValidUserInput_COMMAND_ based on the first token
		// repeat until there's a valid input or exit command was called
	}
};

int main() {

	std::cout << "Welcome to " << THE_BOSS_NAME << "'s HierarchyManager!" << std::endl;
	std::cout << "status: waiting for input" << std::endl;
	std::cout << "*In case you don't know what to do: type 'help'" << std::endl;

	ConsoleInputReader::readInput();

	return 0;
}