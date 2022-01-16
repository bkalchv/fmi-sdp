#include "interface.h"

#include <iostream>
#include <istream>
#include <fstream>
#include <string>

//	!NB! After building the project,
//  Put the text files that are expected to be read in the same folder as the executable file. (Homework3-2.exe)
//	
//	If you're running the project throughout Local Windows Debugger, 
//	keep in mind that VS usually has no pre-set command-line arguments.
//	This means that to test it properly you'd need to assign filename1 and filename2
//  and comment line 30 and 36
// 
//  If you want to see the ComparisonReport's output in the console, 
//	use its public method printReport

int main(int argc, char* argv[]) 
{

	if (argc != 3) 
	{
		std::cout << "Wrong command-line input" << std::endl;
		std::cout << "Expected input: ./<your_executable_name>.exe <your_first_textfile_name>.txt  <your_second_textfile_name>.txt" << std::endl;
		return -1;
	}
 	
	std::string filename1;
	filename1 = argv[1];

	std::ifstream str1;
	str1.open(filename1);

	std::string filename2;
	filename2 = argv[2];

	std::ifstream str2;
	str2.open(filename2);

	Comparator c;
	ComparisonReport cr = c.compare(str1, str2);
	
	str1.close();
	str2.close();

	cr.printReport();

	return 0;
}