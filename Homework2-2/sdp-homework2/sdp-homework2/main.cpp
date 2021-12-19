#include <iostream>

#include "interface.h"

using namespace std;

int main() {

	Hierarchy h = Hierarchy();
	h.addToHierachy("Uspeshnia", "Bogdan");
	h.find("Bogdan");
	h.addToHierachy("Uspeshnia", "Georgi");
	return 0;
}