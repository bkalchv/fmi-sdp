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

void Instrument::display(std::vector<const MenuItem*>& menu)
{
	for (const MenuItem* mItem : menu)
	{
		std::cout << mItem->getNumber() << ". " << mItem->getAction() << std::endl;
	}
}

const MenuItem* Instrument::getChoice(const MenuItem*& choice, std::vector<const MenuItem*>& menu)
{
	const MenuItem* result_getChoice = _getChoice(menu);

	while (!result_getChoice)
	{
		system("CLS");
		std::cout << "Invalid choice. Choose again!" << std::endl;
		display(menu);
		result_getChoice = _getChoice(menu);
	}

	choice = result_getChoice;
	return choice;
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