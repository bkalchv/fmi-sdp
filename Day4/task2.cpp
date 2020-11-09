// Check if a word is a palindrome using stack
#include <iostream>
#include <string>
#include "Stack.h"
#include "Stack.cpp"

using namespace std;

static bool isPalindrome(string word) {
  Stack<char> wordInStack = Stack<char>();
  for (size_t i{0}; i < word.size(); ++i) {
    char currentCharacter = word.at(i);
    wordInStack.push(currentCharacter);
  }

  for (size_t i{0}; i < word.size(); ++i) {
    if (word[i] == wordInStack.top()) {
      wordInStack.pop();
      continue;
    } else {
      return false;
    }
  }

  return true;
}

static void printWhetherPalindrome(string word) {
  if(isPalindrome(word)) {
    cout << "The word '"<< word << "' is a palindrome!" << endl;
  } else {
    cout << "The word '"<< word << "' is NOT a palindrome!" << endl;
  }
}

int main() {
  bool again{true};
  while (again) {
    cout << "Let's check wether a word is a palindrome!" << endl;
    cout << "Your input word: ";
    string inputWord;
    cin >> inputWord;

    printWhetherPalindrome(inputWord);

    string answer;
    cout << "Would you like to check wether another word is a palindrome? (y/n)" << endl;
    cin >> answer;

    while (answer != 'y' && answer != 'n' && answer.length() != 1) {
      cout << "Wrong input! Try again!" << endl;
      cout << "*Expected input: y or n (yes or no)*" << endl;
      cin >> answer;
    }

    if (answer == 'y') {
      again = true;
    } else if (answer == 'n') {
      again = false;
    }
  }

  return 0;
}