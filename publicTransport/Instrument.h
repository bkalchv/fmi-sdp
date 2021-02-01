#pragma once
#include "MenuItem.h"
#include "FileManager.h"
#include <vector>

class Instrument
{
private:
	FileManager fileManager;

public:

	Instrument(const std::string&);
	static std::vector<const MenuItem*> mainMenu;
	static std::vector<const MenuItem*> objectMenu;
	static void display(std::vector<const MenuItem*>&);
	static const MenuItem* getChoice(std::vector<const MenuItem*>&);
	void removeLine();
	
	void setFileManager(const std::string&);
	~Instrument();
private:
	static const MenuItem* _getChoice(std::vector<const MenuItem*>&);
	bool isValidLineNumber(const std::string&);
};

