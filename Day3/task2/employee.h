#ifndef _EMPLOYEE_H_
#define _EMPLOYEE_H_

#include <string>
#include <memory>

using namespace std;

class Employee {
  public:
    Employee();
    Employee(string, int, double);
    Employee(const Employee& other);
    bool operator==(const Employee&) const;
    inline string getName() { return this->name; }
    inline string getName() const { return this->name; }
    void setName(string);
    inline int getMonthsExperience() { return this->monthsExperience; }
    inline int getMonthsExperience() const { return this->monthsExperience; }
    void setMonthsExperience(int);
    inline double getSalary() { return this->salary; }  
    inline double getSalary() const { return this->salary; }
    void setSalary(double);
    void swap(Employee&, Employee&);
    virtual void print() const;
    std::unique_ptr<Employee> clone() const;
    virtual ~Employee() = 0;

  protected:
    string name;
    int monthsExperience;
    double salary;
    virtual Employee* clone_impl() const = 0;
};

#endif