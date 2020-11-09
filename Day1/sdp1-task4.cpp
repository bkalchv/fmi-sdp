#include <iostream>

using namespace std;

double quickPower(const double& base, int power) {

  if (power == 0) {
    return 1;
  }

  if (power == 1) {
    return base;
  }

  if (power & 1) { // Checks if power is odd, using binary operator &. Means that in binary it ends on 1
    return base * quickPower(base, power-1);
  } else {
    double almostResult = quickPower(base, power/2);
    return almostResult * almostResult;
  }

}

template<typename T>
T quickPower(const T& base, int power) {

  if (power == 0) {
    return 1;
  }

  if (power == 1) {
    return base;
  }

  if (power & 1) { // Checks if power is odd, using binary operator &. Means that in binary it ends on 1
    return base * quickPower(base, power - 1);
  } else {
    T almostResult = quickPower(base, power / 2);
    return almostResult * almostResult;
  }

}

int main() {
  double base;
  int power;

  cout << "Insert a base: " << endl << "base = ";
  cin >> base;

  cout << "Insert power amount: " << endl << "power = ";
  cin >> power;

  cout << base << "^" << power << " = " << quickPower(base, power) << endl;

  return 0;
}