#include <iostream>
#include <vector>

using namespace std;

//#define MAXN 30000
//bool sieve[MAXN]; // допълнителна памет! не е много добре това :/
vector<unsigned> getPrimesByEratostensSieve( unsigned n ) {
    unsigned j, i = 2;
    bool sieve[n];
    vector<unsigned> result;

    for ( unsigned i = 0; i < n; i++ ) {
    sieve[i] = false;
    }

    while (i <= n) {
        if (!sieve[i]) {
        
            if(n % i == 0) {
              result.push_back(i);
            }

            j = i * i;

            while (j <= n) {
                sieve[j] = true;
                j += i;
            }
        }
        i++;
    }
    return result;
}

void printPrimeDeviders(const unsigned& number) {
  vector<unsigned> result {getPrimesByEratostensSieve(number)};
  cout << "Prime deviders of the number " <<  number << " are [ ";

  for (const int& n : result) {
    cout << n << " ";
  }

  cout << "]" << endl;
}

void printDecompositionOfNumber(const unsigned& number) {
  vector<unsigned> primeDividersOfNumber{getPrimesByEratostensSieve(number)};
  unsigned numberToIterate{number};

  cout << number << " = ";   
  for (size_t i{0}; i < primeDividersOfNumber.size();) {
    unsigned currentPrimeDivider = primeDividersOfNumber[i];

    if ((numberToIterate / currentPrimeDivider) % currentPrimeDivider == 0) {
      cout << currentPrimeDivider;unsigned numberToIterate{number};

      if (i + 1 != primeDividersOfNumber.size()) {
        cout << " x ";
      }

      numberToIterate = numberToIterate/currentPrimeDivider;
      continue;
    } else {
      numberToIterate = numberToIterate/currentPrimeDivider;
      cout << currentPrimeDivider;

      if (i + 1 != primeDividersOfNumber.size()) {
        cout << " x ";
      }

      ++i;
    }
  }
  cout << endl;
}


int main() {
  unsigned number;

  cout << "Lets decompose a number to its the prime deviders!" << endl;
  cout << "Insert a number: ";
  cin >> number;

  printPrimeDeviders(number);
  printDecompositionOfNumber(number);

  return 0;
}