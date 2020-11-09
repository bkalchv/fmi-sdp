#ifndef _PROGRAMMER_H_
#define _PROGRAMMER_H_
#include "employee.h"
#include <string>

using namespace std;

class Programmer : public Employee {
  private:
    bool knowsCPP;
    bool knowsCS;

  public:
    Programmer();
    Programmer(string, int, double, bool, bool);
    Programmer(const Programmer& other);
    Programmer& operator=(Programmer other);
    inline bool getKnowsCpp() { return knowsCPP; }
    inline bool getKnowsCpp() const { return knowsCPP; }
    inline bool getKnowsCS() { return knowsCS; }
    inline bool getKnowsCS() const { return knowsCS; }
    void swap(Programmer&, Programmer&);
    virtual void print() const override;
    virtual Programmer* clone_impl() const override { return new Programmer(*this); };
    ~Programmer();
};

#endif