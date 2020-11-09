#ifndef _COMPANY_H_
#define _COMPANY_H_

#include "employee.h"
#include "manager.h"
#include "programmer.h"
#include <vector>
#include <memory>

class Company {
  private:
  std::string companyName;
  vector<std::unique_ptr<Employee>> employees;

  public:
  Company();
  Company(std::string, vector<std::unique_ptr<Employee>>&);
  Company(const Company&);
  std::string getCompanyName();
  std::string getCompanyName() const;
  Company& operator=(Company);
  void addNewEmployee();
  void addEmployee(const Programmer&);
  void addEmployee(const Manager&);
  std::unique_ptr<Employee> createEmployee();
  std::unique_ptr<Employee> createEmployee(const Programmer&);
  std::unique_ptr<Employee> createEmployee(const Manager&);
  void removeLastEmployee();
  void removeEmployee();
  void removeEmployee(const Employee&);
  double getTotalSalaries();
  void printTotalSalaries();
  void printAllEmployees();
};

#endif