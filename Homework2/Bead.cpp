#include "Bead.h"
#include <stdexcept>

Bead::Bead(char c) {
	validateColor(c);
	color = c;
}