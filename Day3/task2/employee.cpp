#include "employee.h"
#include <string>
#include <stdexcept>
#include <iostream>

Employee::Employee(): name{"DEFAULT_NAME"}, monthsExperience{-1}, salary{800} {
}

Employee::Employee(string name, int months, double salary) {
  setName(name);
  setMonthsExperience(months);
  setSalary(salary);
}

Employee::Employee(const Employee& other): name{other.name}, monthsExperience{other.monthsExperience}, salary{other.salary} {

}

bool Employee::operator==(const Employee& other) const {
  if ( this->getName() == other.getName()
  && this->getMonthsExperience() == other.getMonthsExperience()
  && this->getSalary() == other.getSalary() ) {
    return true;
  }
  return false;
}

void Employee::setName(string name) {
  if (name.length() < 3) {
    throw std::invalid_argument("Employee's name has to be longer than 3 symbols!");
  }

  this->name = name;
}

void Employee::setMonthsExperience(int months) {
  if (months < 0) {
    throw std::invalid_argument("Months can't be smaller than 0!");
  }

  this->monthsExperience = months;
}

void Employee::setSalary(double salary) {
  if (salary <= 0) {
    throw std::invalid_argument("Salary has to be bigger than 0!");
  }

  this->salary = salary;
}

void Employee::swap(Employee& first, Employee& second) {
  using std::swap;
  swap(first.name, second.name);
  swap(first.monthsExperience, second.monthsExperience);
  swap(first.salary, second.salary);
}

void Employee::print() const {
  std::cout << "========================" << endl;
  std::cout << "[NAME]: " << name << endl;
  std::cout << "[EXPERIENCE]: " << monthsExperience << " months" << endl;
  std::cout << "[SALARY]: " << salary << "$" << endl; 
}

std::unique_ptr<Employee> Employee::clone() const {
  return std::unique_ptr<Employee>(clone_impl());
}

Employee::~Employee() {
  
}
