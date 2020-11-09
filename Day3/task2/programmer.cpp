#include "programmer.h"
#include <stdexcept>
#include <iostream>

Programmer::Programmer(): knowsCPP{false}, knowsCS{false} {

}

Programmer::Programmer(string name, int months, double salary,bool knowsCPP, bool knowsCS): Employee(name, months, salary) {
  this->knowsCPP = knowsCPP;
  this->knowsCS = knowsCS;
}

Programmer::Programmer(const Programmer& other) : Employee(other) {
  this->knowsCPP = other.knowsCPP;
  this->knowsCS = other.knowsCS;
}

Programmer& Programmer::operator=(Programmer other) {
  swap(*this, other);

  return *this;
}

void Programmer::swap(Programmer& first, Programmer& second) {
  using std::swap;
  swap(first.name, second.name);
  swap(first.monthsExperience, second.monthsExperience);
  swap(first.salary, second.salary);
  swap(first.knowsCPP, second.knowsCPP);
  swap(first.knowsCS, second.knowsCS);
}

void Programmer::print() const {
  Employee::print();
  std::cout << "[ROLE]: Programmer" << endl;
  if (this->knowsCPP || this->knowsCS) {
    std::cout << "[LANGUAGES]: ";
    if (knowsCPP) {
      std::cout << "C++ ";
    }
    if (knowsCS) {
      std::cout << "C# "; 
    }
    std::cout << endl;
  }
  std::cout << "========================" << endl;
}

Programmer::~Programmer() {
  
}