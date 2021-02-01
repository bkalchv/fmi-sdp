#include "Instrument.h"

#include <iostream>
#include <string>
#include <stdexcept>

Instrument::Instrument(const FileManager& filemanager)
{
	this->fileManager = FileManager(fileManager.getFilename());

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
		system("CLS");
		std::cout << "Invalid choice. Choose again!" << std::endl;
		display(menu);
		result_getChoice = _getChoice(menu);
	}

	return result_getChoice;
}

void Instrument::addLine()
{
	std::string lineToAddToFile		= getLineToAddToFile();
	//std::string scheduleToAddToFule = getScheduleToAdd();

	this->fileManager.addLine(lineToAddToFile);
}

void Instrument::removeLine()
{
	std::string lineNrToRemove;

	std::cout << "Please input the line number you'd like to remove: ";
	std::cin >> lineNrToRemove;
	while (!isValidLineNumber(lineNrToRemove))
	{
		system("CLS");
		std::cout << "Invalid input. Try again!" << std::endl;
		std::cout << "Please input the line number you'd like to remove: ";
	}

	this->fileManager.removeLine(lineNrToRemove);
}

void Instrument::setFileManager(const std::string& filename)
{
	FileManager result = FileManager(filename);
	this->fileManager = result;
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
	std::cin >> choice;

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

std::string Instrument::getLineToAddToFile()
{
	std::string				lineNrToAdd, lineToAddToTextFile;
	std::vector<Station>	stationsToAdd;

	std::cout << "Please input the line number you'd like to remove: ";
	std::cin >> lineNrToAdd;
	while (!isValidLineNumber(lineNrToAdd))
	{
		system("CLS");
		std::cout << "Invalid input. Try again!" << std::endl;
		std::cout << "Please input the line number you'd like to remove: ";
	}

	lineToAddToTextFile = lineNrToAdd;
	lineToAddToTextFile.append(" : ");

	bool addMoreStations = false;
	std::cout << "Please input the line's stations in order:\n*(station format: stationNumber-stationName)*\n";
	do
	{
		std::string stationToAdd;
		std::cin >> stationToAdd;
		if (isValidStationFormat(stationToAdd))
		{
			size_t delimiterIndex = stationToAdd.find('-');
			std::string inputStationNumber = stationToAdd.substr(0, delimiterIndex);
			std::string inputStationName = stationToAdd.substr(delimiterIndex + 1);
			lineToAddToTextFile.append(inputStationNumber).append("-").append(inputStationName);
			stationsToAdd.push_back(Station(stoi(inputStationNumber), inputStationNumber));

			std::cout << "Would you like to add another one? (y/n)" << std::endl;
			char answer;
			std::cin >> answer;
			while (!isValidYesNoAnswer(answer))
			{
				std::cout << "Invalid answer! Try again (y/n)" << '\r';
				std::cout << "Would you like to add another one? (y/n)" << '\n';
				std::cin >> answer;
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
