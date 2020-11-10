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

stack<char> getStackOfOperators(string expression) {
  stack<char> result;

  for (const char& c : expression) {
    if (operators.find(c) != string::npos) {
      result.push(c);
    }
  }

  if (result.empty()) {
    cout << "There's no operators in your expression!" << endl;
  }

  return result;
}

stack<string> getStackOfOperrands(string expression) {
  stack<string> result;
  string charToString;
  for (const char& c : expression) {
    charToString.push_back(c);
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) ) { // Using ASCII alphabet value ranges
      result.push(charToString);
    } else if (c >= 48 && c <= 57) { // Using ASCII digits value ranges
      result.push(charToString);
    }
    charToString.pop_back();
  }

  if (result.empty()) {
    cout << "There's no operrands in your expression!" << endl;
  }

  return result;
}

string getPostfixExpression(string operrand1, char currentOperator, string operrand2) {
  string result;
  result.append(operrand2).append(operrand1).push_back(currentOperator);
  return result;
}

string toPostfixExpression(string infixExpression) {
  stack<char> operatorsStack {getStackOfOperators(infixExpression)};
  stack<string> operrandsStack {getStackOfOperrands(infixExpression)};

  while (!operatorsStack.empty()) {
    char currentOperator = operatorsStack.top();
    string operrand1 = operrandsStack.top();
    operrandsStack.pop();
    string operrand2 = operrandsStack.top();
    operrandsStack.pop();

    string postfixForCurrentOperator = getPostfixExpression(operrand1, currentOperator, operrand2);

    operrandsStack.push(postfixForCurrentOperator);

    operatorsStack.pop();
  }

  return operrandsStack.top();
}

int main() {
  string infixExpression = "a+a+b*c";
  string postfixExpression = toPostfixExpression(infixExpression);
  cout << postfixExpression << endl;
}