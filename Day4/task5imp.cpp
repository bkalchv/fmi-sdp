/* Задача 5: Напишете програма, която да преобразува израз в инфиксен запис до израз в постфиксен запис.
 Например:
  postfix: a + b * c = a + bc* = abc*+
  postfix: (a * b) + (c * d)- e = (ab*) + (cd*) - e = ab*cd*+ - e = ab*cd*+e
В израза могат да бъдат срещнаите следните оператори: +, −, ∗, /,(,) и операнди a−z, A−Z. */

#include <iostream>
#include <string>
#include <stack>

using namespace std;

const string operators{"+-*/()"};

bool isOperrand(char c) {
  if (((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c >= 48 && c <= 57))) { // Using ASCII 
    return true;
  }
  return false;
}

int getOperatorsWeight(char c) {
  if (c == '*' || c == '/') {
    return 2;
  } else if (c == '+' || c == '-') {
    return 1;
  }
  return 0;
}

string toPostfixExpression(const string& infixExpression) {
  string result;
  stack<char> operatorsStack;

  for (const char& c : infixExpression) {
    if (c == '(') {
      operatorsStack.push(c);
    } else if ( c == ')' ) {

      while(!operatorsStack.empty() && operatorsStack.top() != '(') {
        result.push_back(operatorsStack.top());
        operatorsStack.pop();
      }
      operatorsStack.pop(); // pops the '('
    } else if (isOperrand(c)) {
      result.push_back(c);
    } else {
      while(!operatorsStack.empty() && getOperatorsWeight(operatorsStack.top()) >= getOperatorsWeight(c)) {
        result.push_back(operatorsStack.top());
        operatorsStack.pop();
      }
      operatorsStack.push(c);
    }
  }

  while(!operatorsStack.empty()) {
    result.push_back(operatorsStack.top());
    operatorsStack.pop();
  }

  return result;
}

int main() {
  string infixExpression = "a+b*c";
  string postfixExpression = toPostfixExpression(infixExpression);
  cout << postfixExpression << endl;
}