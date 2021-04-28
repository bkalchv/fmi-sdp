#pragma once
#include "MenuItem.h"
#include "FileManager.h"
#include <vector>

class Instrument
{
private:
	FileManager fileManager;

public:
	std::vector<const MenuItem*> objectMenu;
	std::vector<const MenuItem*> mainMenu;

	Instrument(const FileManager&);

	void display(std::vector<const MenuItem*>&);
	const MenuItem* getChoice(std::vector<const MenuItem*>&);
	void addStation();
	void removeStation();
	void addRoute();
	void removeRoute();
	void addLine();
	void removeLine();

	~Instrument();
private:;
	static const MenuItem* _getChoice(std::vector<const MenuItem*>&);
	bool isValidLineNumber(const std::string&);
	bool isValidStationFormat(const std::string&);
	bool isValidYesNoAnswer(char c);
	bool isNumber(const std::string&);
	std::string getLineToAddToFile();
};

