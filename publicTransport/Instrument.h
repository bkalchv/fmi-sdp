#pragma once
#include "MenuItem.h"
#include <vector>

class Instrument
{
public:
	static std::vector<const MenuItem*> mainMenu;
	static std::vector<const MenuItem*> objectMenu;
	//static FileManager fileManager;
	static void display(std::vector<const MenuItem*>&);
	static const MenuItem* getChoice(const MenuItem*&, std::vector<const MenuItem*>&);

	~Instrument();
private:
	static const MenuItem* _getChoice(std::vector<const MenuItem*>&);
};

