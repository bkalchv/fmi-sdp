#include "manager.h"
#include <stdexcept>
#include <iostream>

Manager::Manager(): employeesAmount{0} {

}

Manager::Manager(string name, int months, double salary, int employeesAmount): Employee(name, months, salary) {

  if (employeesAmount < 0) {
    throw new std::invalid_argument("Employees amount have to be bigger than 0");
  }

  this->employeesAmount = employeesAmount;
}

Manager::Manager(const Manager& other) : Employee(other) {
  this->employeesAmount = other.employeesAmount;
}

Manager& Manager::operator=(Manager other) {
  swap(*this, other);

  return *this;
}

void Manager::swap(Manager& first, Manager& second) {
  using std::swap;
  swap(first.name, second.name);
  swap(first.monthsExperience, second.monthsExperience);
  swap(first.salary, second.salary);
  swap(first.employeesAmount, second.employeesAmount);
}

void Manager::print() const {
  Employee::print();
  std::cout << "[ROLE]: Manager" << endl;
  std::cout << "[MANAGER OF]: " << employeesAmount << " people" << endl;
  std::cout << "========================" << endl;
}

Manager::~Manager() {
  
}