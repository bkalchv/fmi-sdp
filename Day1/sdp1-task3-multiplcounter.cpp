#include <iostream>
#include <math.h>  

using namespace std;

//Намерете броя на цифрите на произведение

int findDigitsAmountOfMultiplication(double num1, double num2) {
  int digitsAmount = static_cast<int>(log10(num1) + log10(num2) + 1);
  return digitsAmount;
}

int main() {
  double multiplier1, multiplier2;

  cout << "Let's find out how many digits will the number that comes out as a result of the multiplication of two numbers will be!" << endl;
  cout << "Enter the first number: " << endl;
  cin >> multiplier1;
  cout << "Enter the second number: " << endl;
  cin >> multiplier2;

  int digits = findDigitsAmountOfMultiplication(multiplier1, multiplier2);
  cout << "Multiplying " << multiplier1 << " and " << multiplier2 << " will result in a number that has " << digits << " digits." << endl;
  return 1;
}