#include <iostream>

#include "company.h"
#include "../dynamicVector.h"

using namespace std;

static void printOptions() {
  cout << "-------------------------------------------------------------------------------------" << endl;
  cout << "| What would you like to do?                                                        |" << endl;
  cout << "| Press [1] to add a new employee!                                                  |" << endl;
  cout << "| Press [2] to delete an employee!                                                  |" << endl;
  cout << "| Press [3] to display all employees, currently added in the list of the employees! |" << endl;
  cout << "| Press [4] to display the current expense of the company for monthly wages!        |" << endl;
  cout << "-------------------------------------------------------------------------------------" << endl;
  cout << "Your choice: ";
}

static void printWelcomeMessage(const Company& company) {
  cout << "-------------------------------------------------------------------------------------" << endl;
  cout << "| Welcome to the control panel of " << company.getCompanyName() << "!                                        |" << endl;
  cout << "-------------------------------------------------------------------------------------" << endl;
}

static bool validateOperation(int inputOperation) {
  if (inputOperation != 1 && inputOperation != 2 && inputOperation != 3) return false;
  return true;
}

int main() {
  Programmer defaultProgrammer = Programmer();
  Programmer antosho = Programmer("Antosho", 20, 1000, true, false);
  Manager peter = Manager("Peter", 28, 1500, 1);

  vector<std::unique_ptr<Employee>> testvec;
  testvec.push_back(std::make_unique<Programmer>(antosho));
  testvec.push_back(std::make_unique<Manager>(peter));

  Company testCompany = Company("MyCompany", testvec);

  bool wantToTryAgain{false};
  printWelcomeMessage(testCompany);
  do {
    printOptions();
    int inputOperation;
    cin >> inputOperation;
    while (!validateOperation(inputOperation)) {
      cout << "Invalid input! Please try again! (1/2/3)" << endl;

      char optionsAgainInput;
      cout << "*** Would you like to see the options again? (y/n) ***" << endl;
      cin >> optionsAgainInput;
      if (optionsAgainInput == 'y') {
        printOptions();
      }

      cout << "Your choice: ";
      cin >> inputOperation;
    }

    switch(inputOperation) {
      case 1 : {
        testCompany.addNewEmployee();
        break;
      }
      case 2 : {
        testCompany.removeEmployee();
        break;
      }
      case 3 : {
        testCompany.printAllEmployees();
        break;
      }
      case 4 : {
        testCompany.printTotalSalaries();
        break;
      }
      default : {
        throw new invalid_argument("Options input invalid. Validation poorly done!");
        break;
      }
    }

    char againAnswer;
    cout << "*** Would you like to do something else? (y/n) ***" << endl;
    cin >> againAnswer;
    
    while (againAnswer != 'y' && againAnswer != 'n') {
      cout << "Invalid answer. Try again! (y/n)" << endl;
      cin >> againAnswer;
    }

    if (againAnswer == 'y') {
      wantToTryAgain = true;
    } else if (againAnswer == 'n') {
      wantToTryAgain = false;
    }

  } while (wantToTryAgain);
}