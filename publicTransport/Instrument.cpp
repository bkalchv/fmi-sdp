#include "Instrument.h"

#include <iostream>
#include <string>
#include <stdexcept>

std::vector<const MenuItem*> Instrument::mainMenu =
{
	new MenuItem("1", "Add"),
	new MenuItem("2", "Remove"),
	new MenuItem("3", "Quit")
};

std::vector<const MenuItem*> Instrument::objectMenu =
{
		new MenuItem("1","Bus Station"),
		new MenuItem("2", "Route"),
		new MenuItem("3", "Line"),
		new MenuItem("4", "Back")
};

Instrument::Instrument(const std::string& filename)
{
	this->setFileManager(filename);
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
