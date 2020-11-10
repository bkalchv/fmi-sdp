/* Задача 3: Проверете дали даден израз е с правилно поставени скоби. Израз е с правилно
поставени скоби ако за всяка отваряща скоба има съответна затваряща, няма незатворени
или неотворени скоби и областите на действие на скобите не се пресичат. Считайте че
имате 3-те вида скоби: (), [], {} */

#include <iostream>
#include <stack>

using namespace std;

string expressionToBrackets(const string& expression) {
  string result;
  for (const char& c : expression) {
    if(c == '{' || c == '}' || c == '(' || c == ')' || c == '[' || c == ']') {
      result.push_back(c);
    }
  }

  return result;
}

bool isOpeningBracket(const char& c) {
  if (c == '(' || c == '[' || c == '{') {
    return true;
  }
  return false;
}

bool isClosingBracket(const char& c) {
  if (c == ')' || c == ']' || c == '}') {
    return true;
  }
  return false;
}

char getOpeningBracket(const char& closingBracket) {
  if (isClosingBracket(closingBracket)) {
    switch (closingBracket) {
    case ')' : {
      return '(';
      break;
    }
    case ']' : {
      return '[';
      break;
    } 
    case '}' :  {
      return '{';
      break;
    }
    default : 
      break;
    }
  }
    return '-';
}

char getClosingBracket(const char& openingBracket) {
  if (isOpeningBracket(openingBracket)) {
    switch (openingBracket) {
    case '(' : {
      return ')';
      break;
    }
    case '[' : {
      return ']';
      break;
    } 
    case '{' :  {
      return '}';
      break;
    }
    default : 
      break;
    }
  }
    return '-';
}

bool isValidExpression (string expressionToBeChecked) {
  string bracketsExpression(expressionToBrackets(expressionToBeChecked));
  stack<char> stackOfOpenningBrackets;

  for (size_t i{0}; i < bracketsExpression.size(); ++i) {
    if ( isOpeningBracket(bracketsExpression[i]) ) {
      stackOfOpenningBrackets.push(bracketsExpression[i]);
      continue;
    }

    if ( !stackOfOpenningBrackets.empty() && getOpeningBracket(bracketsExpression[i]) != '-' && bracketsExpression[i] == getClosingBracket(stackOfOpenningBrackets.top()) ) {
      stackOfOpenningBrackets.pop();
    } else if (!isClosingBracket(bracketsExpression[i])) {
      continue;
    } else {
      return false;
    }
  }

  return stackOfOpenningBrackets.empty();
}

void printWhetherValid(string expression) {
  cout << "---------------------------------------------------------------------------" << endl;
  cout << "The expression '" << expression << "' is being broken down to " << expressionToBrackets(expression) << endl;
  if (isValidExpression(expression)) {
    cout << "which is a VALID expression!" << endl;
  } else {
    cout << "which is an INVALID expression!" << endl;
  }
  cout << "---------------------------------------------------------------------------" << endl;
}

int main() {
  bool again{true};
  while (again) {
    cout << "Let's check wether an expression is a valid one!" << endl;
    cout << " ------------------------------------------------------------------" << endl;
    cout << " |*NB: We call valid expression,                                  |" << endl;
    cout << " |- an expression that has a closing bracket for each opening one |" << endl;
    cout << " |- and there's no intersection of different types of brackets!   |" << endl;
    cout << " ------------------------------------------------------------------" << endl;

    cout << "Your input expression is: ";
    string inputExpression;
    cin >> inputExpression;

    printWhetherValid(inputExpression);

    string answer;
    cout << "Would you like to check wether another expression is valid? (y/n)" << endl;
    cin >> answer;

    while (answer != "y" && answer != "n" && answer.length() != 1) {
      cout << "Wrong input! Try again!" << endl;
      cout << "*Expected input: y or n (yes or no)*" << endl;
      cin >> answer;
    }

    if (answer == "y") {
      again = true;
    } else if (answer == "n") {
      again = false;
    }
  }

  return 0;
} 