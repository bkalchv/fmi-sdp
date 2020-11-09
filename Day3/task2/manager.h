#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "employee.h"
#include <string>

using namespace std;

class Manager : public Employee {

  public:
    Manager();
    Manager(string, int, double, int);
    Manager(const Manager& other);
    Manager& operator=(Manager other);
    inline int getEmployeesAmount() { return employeesAmount; }
    inline int getEmployeesAmount() const { return employeesAmount; }
    void swap(Manager&, Manager&);
    virtual void print() const override;
    ~Manager();

  protected:
    int employeesAmount;
    virtual Manager* clone_impl() const override { return new Manager(*this); }
};

#endif