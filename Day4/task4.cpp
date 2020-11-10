/* Задача 4: Обърнете стек използвайки само стекови операции.
Например S = {a,b,c} -> S={c,b,a}. */

#include <iostream>
#include <stack>

using namespace std;

stack<char> reverseStack(stack<char> stackToReverse) {
  stack<char> result;

  while (!stackToReverse.empty()) {
    result.push(stackToReverse.top());
    stackToReverse.pop();
  }

  return result;
}

ostream& operator<<(ostream& os, stack<char> stackToPrint) {
  os << "-----" << endl;
  while (!stackToPrint.empty()) {
    os << "| "<< stackToPrint.top() << " |" << endl;
    os << "-----" << endl;
    stackToPrint.pop();
  }

  return os;
}

int main() {
  // s = |c| -> |b| -> |a|;
  stack<char> s;
  s.push('a');
  s.push('b');
  s.push('c');

  // s1 = |a| -> |b| -> |c|;
  stack<char> s1 = reverseStack(s);
  cout << s1;
  
}