#include <iostream>
#include <vector>

using namespace std;

//function to check if a number is a prime
bool isPrime(int number) {
    if (number < 2)
        return false;
    for (int i = 2; i * i <= number; i++) {
        if (number % i == 0)
            return false;
    }
    return true;
}

//function to get all primes up until the number
vector<int> getAllPrimesUntilNumber(int number) { //Eratosten's better
  vector<int> result;

  for(size_t i{0}; i < number; ++i) {
    if(isPrime(i)) {
      result.push_back(i);
    }
  }

  if(isPrime(number)) {
    result.push_back(number);
  } 

  return result;
}

vector<int> getPrimeDividersOfNumber(int number) {
  vector<int> result, primesUntilNumber{getAllPrimesUntilNumber(number)};

  for(const int& prime : primesUntilNumber) {
    if (number % prime == 0) {
      result.push_back(prime);
    }
  }

  return result;
}

void printDecompositionOfNumber(int number) {
  vector<int> primeDividersOfNumber{getPrimeDividersOfNumber(number)};
  cout << number << " = "; 
  int numberToIterate{number};
  for (size_t i{0}; i < primeDividersOfNumber.size();) {
    int currentPrimeDivider = primeDividersOfNumber[i];
    if((numberToIterate/currentPrimeDivider)%currentPrimeDivider == 0) {
      cout << currentPrimeDivider;
      if (i != primeDividersOfNumber.size()) {
        cout << " x ";
      }
      numberToIterate = numberToIterate/currentPrimeDivider;
      continue;
    } else {
      ++i;
      numberToIterate = numberToIterate/currentPrimeDivider;
      cout << currentPrimeDivider;
      if (i != primeDividersOfNumber.size()) {
        cout << " x ";
      }
    }
  }
  cout << endl;
}


void printPrimeDeviders(const int& number) {
  vector<int> result {getPrimeDividersOfNumber(number)};
  cout << "Prime deviders of the number " <<  number << " are [ ";
  for(const int& n : result) {
    cout << n << " ";
  }
  cout << "]" << endl;
}

int main() {
  cout << "Lets decompose a number to its the prime deviders!" << endl;
  cout << "Input a number: ";
  int number;
  cin >> number;
  printPrimeDeviders(number);
  printDecompositionOfNumber(number);
}