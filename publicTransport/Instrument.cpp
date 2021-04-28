#include "Instrument.h"

#include <iostream>
#include <string>
#include <stdexcept>

const std::string BLANK_STATION_INPUT = "-";

Instrument::Instrument(const FileManager& filemanager)
{
	this->fileManager = FileManager(filemanager.getFilenameStations(), filemanager.getFilenameTimetable());

	this->mainMenu =
	{
		new MenuItem("1", "Add"),
		new MenuItem("2", "Remove"),
		new MenuItem("3", "Quit")
	};

	this->objectMenu =
	{
			new MenuItem("1","Bus Station"),
			new MenuItem("2", "Route"),
			new MenuItem("3", "Line"),
			new MenuItem("4", "Back")
	};
}

void Instrument::display(std::vector<const MenuItem*>& menu)
{
	for (const MenuItem* mItem : menu)
	{
		std::cout << mItem->getNumber() << ". " << mItem->getAction() << std::endl;
	}
}

const MenuItem* Instrument::getChoice(std::vector<const MenuItem*>& menu)
{
	const MenuItem* result_getChoice = _getChoice(menu);

	while (!result_getChoice)
	{
		//system("CLS");
		std::cout << "Invalid choice. Choose again!" << std::endl;
		display(menu);
		result_getChoice = _getChoice(menu);
	}

	return result_getChoice;
}

void Instrument::addStation()
{
	std::string stationToAddNumber, stationToAddName, stationToAddLineNumber, stationToAddPrevious, stationToAdd;
	std::cout << "Please enter the number of the station you'd like to add: ";
	std::getline(std::cin, stationToAddNumber);

	while (!isNumber(stationToAddNumber))
	{
		std::cout << "Wrong input! Try again! (a number expected)" << std::endl;
		std::cout << "Please enter the number of the station you'd like to add: ";
		std::getline(std::cin, stationToAddNumber);
	}

	std::cout << "Please enter the name of the station you'd like to add:";
	std::getline(std::cin, stationToAddName);

	stationToAdd = stationToAddNumber.append("-").append(stationToAddName);

	std::cout << "To which line would you like to add a station?" << std::endl;
	std::getline(std::cin, stationToAddLineNumber);
	while (!isValidLineNumber(stationToAddLineNumber))
	{
		std::cout << "Wrong input! Try again! (a positive number expected)" << std::endl;
		std::cout << "Please enter the number of the station you'd like to add: ";
		std::getline(std::cin, stationToAddLineNumber);
	}

	std::cout << "After which station would you like to add the station?" << std:: endl;
	std::cout << "('-' in case you're adding a first stop)" << std::endl;
	std::getline(std::cin, stationToAddPrevious);
	if (stationToAddPrevious != BLANK_STATION_INPUT)
	{
		while (!isValidStationFormat(stationToAddPrevious))
		{
			std::cout << "Invalid station input! (*stationNumber-stationName* expected)" << std::endl;
			std::cout << "After which station would you like to add the station?" << std::endl;
			std::getline(std::cin, stationToAddPrevious);
		}
	}

	this->fileManager.addStation(stationToAdd, stationToAddLineNumber, stationToAddPrevious);
}

void Instrument::removeStation()
{
	std::cout << "Please input the station you'd like to remove: ";
	std::string stationToRemove;
	std::getline(std::cin, stationToRemove);

	while (!isValidStationFormat(stationToRemove))
	{
		std::cout << "Invalid input! Try again!" << std::endl;
		std::cout << "Please input the station you'd like to remove: ";
		std::getline(std::cin, stationToRemove);
	}

	this->fileManager.removeStation(stationToRemove);
}

void Instrument::addRoute()
{
	std::string lineNumber, fromStation, toStation, routeLength;
	std::vector<Station> stationsToAdd;

	std::cout << "To which line would you like to add a station?" << std::endl;
	std::getline(std::cin, lineNumber);
	while (!isValidLineNumber(lineNumber))
	{
		std::cout << "Wrong input! Try again! (a positive number expected)" << std::endl;
		std::cout << "Please enter the number of the station you'd like to add: ";
		std::getline(std::cin, lineNumber);
	}

	std::cout << "From which station on would you like to add a route?" << std::endl;
	std::getline(std::cin, fromStation);
	while (!isValidStationFormat(fromStation))
	{
		std::cout << "Invalid input! Try again." << std::endl;
		std::cout << "From which station on would you like to add a route?" << std::endl;
		std::getline(std::cin, fromStation);
	}

	std::cout << "To which station on would you like to add a route?" << std::endl;
	std::getline(std::cin, toStation);
	while (!isValidStationFormat(toStation))
	{
		std::cout << "Invalid input! Try again!" << std::endl;
		std::cout << "To which station on would you like to add a route?" << std::endl;
		std::getline(std::cin, toStation);
	}

	std::cout << "How many stations does this route consist of?";
	std::getline(std::cin, routeLength);
	while (!isValidLineNumber(routeLength))
	{
		std::cout << "Wrong input! Try again. (positive whole number expected)";
		std::cout << "How many stations does this route consist of?";
		std::getline(std::cin, routeLength);
	}

	stationsToAdd.resize(stoi(routeLength));

	bool addMoreStations = false;
	std::cout << "Please input the line's stations in order:\n*(station format: stationNumber-stationName)*\n";
	for (int i{0}; i < stoi(routeLength); ++i)
	{
		std::string stationToAdd;
		std::getline(std::cin, stationToAdd);
		if (isValidStationFormat(stationToAdd))
		{
			size_t delimiterIndex = stationToAdd.find('-');
			std::string inputStationNumber = stationToAdd.substr(0, delimiterIndex);
			std::string inputStationName = stationToAdd.substr(delimiterIndex + 1);
			stationsToAdd[i] = Station(stoi(inputStationNumber), inputStationName);
		}
		else
		{
			std::cout << "Invalid input! Try again." << std::endl;
			std::cout << "Station format incompatible! (expected: *stationNr - stationName*)";
			--i;
		}
	}

	this->fileManager.addRoute(lineNumber, fromStation, toStation, stationsToAdd);
}

void Instrument::removeRoute()
{
	std::string lineNumber, fromStation, toStation;

	std::cout << "To which line would you like to remove a station?" << std::endl;
	std::getline(std::cin, lineNumber);
	while (!isValidLineNumber(lineNumber))
	{
		std::cout << "Wrong input! Try again! (a positive number expected)" << std::endl;
		std::cout << "Please enter the number of the station you'd like to add: ";
		std::getline(std::cin, lineNumber);
	}

	std::cout << "From which station on would you like to remove a route?" << std::endl;
	std::getline(std::cin, fromStation);
	while (!isValidStationFormat(fromStation))
	{
		std::cout << "Invalid input! Try again!" << std::endl;
		std::cout << "Please input the station you'd like to remove: ";
		std::getline(std::cin, fromStation);
	}

	std::cout << "To which station on would you like to remove a route?" << std::endl;
	std::getline(std::cin, toStation);
	while (!isValidStationFormat(toStation))
	{
		std::cout << "Invalid input! Try again!" << std::endl;
		std::cout << "Please input the station you'd like to remove: ";
		std::getline(std::cin, fromStation);
	}

	this->fileManager.removeRoute(lineNumber, fromStation, toStation);
}

void Instrument::addLine()
{
	std::string lineToAddToFile		= getLineToAddToFile();
	//std::string scheduleToAddToFile = getScheduleToAdd();

	this->fileManager.addLine(lineToAddToFile);
}

void Instrument::removeLine()
{
	std::string lineNrToRemove;

	std::cout << "Please input the line number you'd like to remove: ";
	std::getline(std::cin, lineNrToRemove);
	while (!isValidLineNumber(lineNrToRemove))
	{
		system("CLS");
		std::cout << "Invalid input. Try again!" << std::endl;
		std::cout << "Please input the line number you'd like to remove: ";
	}

	this->fileManager.removeLine(lineNrToRemove);
}

Instrument::~Instrument()
{
	for (const MenuItem* item : mainMenu)
	{
		item = nullptr;
		delete item;
	}

	for (const MenuItem* item : objectMenu)
	{
		item = nullptr;
		delete item;
	}

	// TODO : FileManager destr.?
}

const MenuItem* Instrument::_getChoice(std::vector<const MenuItem*>& menu)
{
	std::string choice;
	std::getline(std::cin, choice);

	for (const MenuItem* mItem : menu)
	{
		if (mItem->match(choice)) return mItem;
	}

	return nullptr;
}

bool Instrument::isValidLineNumber(const std::string& lineNumber)
{
	for (const char& c : lineNumber)
	{
		if (c == lineNumber.front())
		{
			if (c < 49 || c > 58)
				return false;
		}
		else
		{
			if (c < 48 || c > 58)
				return false;
		}

	}

	return true;
}

bool Instrument::isValidStationFormat(const std::string& stationInput)
{
	size_t delimiterIndex = stationInput.find('-');
	if (delimiterIndex == -1)
	{
		return false;
	}
	else
	{
		std::string inputStationNumber	= stationInput.substr(0, delimiterIndex);
		std::string inputStationName	= stationInput.substr(delimiterIndex + 1);

		if (inputStationNumber.empty() || inputStationName.empty())
			return false;

		if (!isValidLineNumber(inputStationNumber))
			return false;
	}

	return true;
}

bool Instrument::isValidYesNoAnswer(char c)
{
	return c == 'y' || c == 'n';
}

bool Instrument::isNumber(const std::string& stringToCheck)
{
	for (const char& c : stringToCheck)
	{
		if (!isdigit(c))
		{
			return false;
		}
	}
	return true;
}

std::string Instrument::getLineToAddToFile()
{
	std::string				lineNrToAdd, lineToAddToTextFile;
	std::vector<Station>	stationsToAdd;

	std::cout << "Please input the line number you'd like to add ";
	std::getline(std::cin, lineNrToAdd);
	while (!isValidLineNumber(lineNrToAdd))
	{
		system("CLS");
		std::cout << "Invalid input. Try again!" << std::endl;
		std::cout << "Please input the line number you'd like to add: ";
	}

	lineToAddToTextFile = lineNrToAdd;
	lineToAddToTextFile.append(" : ");

	bool addMoreStations = false;
	std::cout << "Please input the line's stations in order:\n*(station format: stationNumber-stationName)*\n";
	do
	{
		std::string stationToAdd;
		std::getline(std::cin, stationToAdd);
		if (isValidStationFormat(stationToAdd))
		{
			size_t delimiterIndex = stationToAdd.find('-');
			std::string inputStationNumber = stationToAdd.substr(0, delimiterIndex);
			std::string inputStationName = stationToAdd.substr(delimiterIndex + 1);
			lineToAddToTextFile.append(inputStationNumber).append("-").append(inputStationName);
			stationsToAdd.push_back(Station(stoi(inputStationNumber), inputStationName));

			std::cout << "Would you like to add another one? (y/n)" << std::endl;
			char answer;
			std::cin.get(answer);
			while (!isValidYesNoAnswer(answer))
			{
				std::cout << "Invalid answer! Try again (y/n)" << '\r';
				std::cout << "Would you like to add another one? (y/n)" << '\n';
				std::cin.get(answer);
			}

			if (answer == 'y')
			{
				addMoreStations = true;
				lineToAddToTextFile.append(", ");
			}
			else if (answer == 'n' && stationsToAdd.size() < 2)
			{
				std::cout << "You should add another station! Lines shorter than two stations are considered invalid!" << std::endl;
				addMoreStations = true;
				lineToAddToTextFile.append(", ");
			}
			else if (answer == 'n' && stationsToAdd.size() >= 2)
			{
				addMoreStations = false;
			}
		}
	} while (addMoreStations);

	return lineToAddToTextFile;
}
