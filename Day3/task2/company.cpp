#include "company.h"
#include <iostream>

using namespace std;

Company::Company(): companyName{"DEFAULT_NAME"}, employees{vector<std::unique_ptr<Employee>>()} {

}

Company::Company(const Company& other): companyName{other.companyName}, employees{vector<std::unique_ptr<Employee>>()} {
  for (const auto& e : other.employees) {
    this->employees.push_back(e->clone());
  }
}

Company::Company(std::string companyName, vector<std::unique_ptr<Employee>>& employees): companyName{companyName}, employees{vector<std::unique_ptr<Employee>>()} {
  for (const auto& e : employees) {
    this->employees.push_back(e->clone());
  }
}

Company& Company::operator=(Company other) {
  using std::swap;
  swap(this->companyName, other.companyName);
  swap(this->employees, other.employees);
  return *this;
}

std::string Company::getCompanyName() {
  return this->companyName;
}

std::string Company::getCompanyName() const {
  return this->companyName;
}

std::unique_ptr<Employee> Company::createEmployee() {
  std::string newEmployeeName;
  std::cout << "Enter new Employee's name: " << endl;
  cin >> newEmployeeName;

  int newEmployeeExperience;
  std::cout << "Enter new Employee's experience in months: " << endl;
  cin >> newEmployeeExperience;

  double newEmployeeSalary;
  std::cout << "Enter new Employee's salary: " << endl;
  cin >> newEmployeeSalary;

  char programmerOrManager;
  std::cout << "Choose new Employee's role- programmer or manager (p/m): ";
  cin >> programmerOrManager;

  switch (programmerOrManager) {
    case 'p': {
      char knowsCPPorNot, knowsCSorNot;
      bool knowsCPP, knowsCS = false;

      std::cout << " Does the programmer know C++? (y/n): ";
      cin >> knowsCPPorNot;
      if (knowsCPPorNot == 'y') {
        knowsCPP = true;
      }

      std::cout << " Does the programmer know C#? (y/n): ";
      cin >> knowsCSorNot;
      if (knowsCSorNot == 'y') {
        knowsCS = true;
      }

      return std::make_unique<Programmer>(newEmployeeName, newEmployeeExperience, newEmployeeSalary, knowsCPP, knowsCS);
    }
    case 'm' : {
      int amountOfPeopleManaging;
      std::cout << "How many people does the manager manage?" << endl;
      cin >> amountOfPeopleManaging;

      return std::make_unique<Manager>(newEmployeeName, newEmployeeExperience, newEmployeeSalary, amountOfPeopleManaging);
    } 
    default: 
    break;
  }
  return nullptr;
}

std::unique_ptr<Employee> Company::createEmployee(const Programmer& newProgrammer) {
  return std::make_unique<Programmer>(newProgrammer.getName(), newProgrammer.getMonthsExperience(), newProgrammer.getSalary(), newProgrammer.getKnowsCpp(), newProgrammer.getKnowsCS());
}

std::unique_ptr<Employee> Company::createEmployee(const Manager& newManager) {
  return std::make_unique<Manager>(newManager.getName(), newManager.getMonthsExperience(), newManager.getSalary(), newManager.getEmployeesAmount());
}

void Company::addNewEmployee() {
  employees.push_back(createEmployee()); 
  cout << "- "  << employees[employees.size()-1]->getName() << " has been added successfully! -" << endl;
}

void Company::addEmployee(const Programmer& newProgrammer) {
  employees.push_back(createEmployee(newProgrammer));
}

void Company::addEmployee(const Manager& newManager) {
  employees.push_back(createEmployee(newManager));
}

void Company::removeLastEmployee() {
  employees.pop_back();
}

double Company::getTotalSalaries() {
  if(employees.size() > 0) {
    double totalSalaries{0};
    
    for (size_t i{0}; i < employees.size(); ++i) {
      totalSalaries = totalSalaries + employees[i]->getSalary();
    }

    return totalSalaries;
  }

  return 0;
}

void Company::printTotalSalaries() {
  std::cout << "Current total salaries: " << this->getTotalSalaries() << "$" << endl;
  std::cout << "Distributed between " << this->employees.size() << " employees." << endl; 
}

void Company::printAllEmployees() {

  std::cout << "All employees of the company: " << endl;

  for (size_t i{0}; i < employees.size(); ++i) {
    employees[i]->print();
  }
}

void Company::removeEmployee() {
  cout << "Which employee shall be removed?" << endl;
  cout << " *Pick a number* " << endl;

  unsigned inputEmployeeIndex;

  for (size_t i{0}; i < employees.size(); ++i) {
    cout << "[" << i << "] " << employees[i]->getName() << endl; 
  }
  
  cin >> inputEmployeeIndex;

  while (inputEmployeeIndex > employees.size() || inputEmployeeIndex < 0) {
    cout << "  Invalid input. Try again!  " << endl;
    cout << " *Pick a number* " << endl;
    cin >> inputEmployeeIndex;
  }

  unique_ptr<Employee> employeeToBeRemoved = employees[inputEmployeeIndex]->clone();
  removeEmployee(*employeeToBeRemoved);

}

void Company::removeEmployee(const Employee& employeeToBeRemoved) {
  vector<unique_ptr<Employee>> buffer;

  for(auto& e : employees) {
    if (*e == employeeToBeRemoved) {
      e.reset();
      continue;
    }

    buffer.push_back(e->clone());
  }

  for (size_t i{employees.size()}; i > 0; --i) {
    employees.pop_back();
  }

  for (const auto& e : buffer) {
    employees.push_back(e->clone());
  }

  cout << "- " << employeeToBeRemoved.getName() << " successfully removed!" << " -" << endl;
}