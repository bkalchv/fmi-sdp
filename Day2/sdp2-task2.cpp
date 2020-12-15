// Smith's number
// - Sum of its' numbers is equal to the sum of its' decomposed numbers

#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

vector<unsigned> getDecomposedNumbersEratosten(unsigned n)
{
  unsigned j, i = 2;
  bool sieve[n];
  vector<unsigned> result;

  for (unsigned i = 0; i < n; i++)
  {
    sieve[i] = false;
  }

  while (i <= n)
  {

    if (!sieve[i])
    {

      if (n % i == 0)
      {
        result.push_back(i);
      }

      j = i * i;

      while (j <= n)
      {
        sieve[j] = true;
        j += i;
      }
    }

    i++;
  }

  cout << "RESULT: ";

  for (auto r : result)
  {
    cout << "r: " << r << endl;
  }

  return result;
}

unsigned getTenths(const unsigned &number)
{
  unsigned digitsAmount = log10(number) + 1;
  unsigned tenths = 0;

  if (digitsAmount > 1)
  {
    tenths = 1;
    for (size_t z{1}; z < digitsAmount - 1; ++z)
    {
      tenths *= 10;
    }
  }

  return tenths;
}

vector<unsigned> getDecomposedNumbersDigitsVector(const vector<unsigned> &decomposedNumbers)
{
  vector<unsigned> result;
  for (const auto &num : decomposedNumbers)
  {
    unsigned numsLength = log10(num) + 1;
    if (numsLength > 1)
    {
      unsigned copyNum = num;
      for (size_t i{0}; i < numsLength; ++i)
      {
        if (numsLength - 1 - i == 0)
        {
          result.push_back(copyNum);
          break;
        }
        else
        {
          result.push_back(copyNum / (unsigned)pow(10, numsLength - 1));
        }

        copyNum = copyNum % ((unsigned)pow(10, numsLength - 1 - i));
      }
    }
    else
    {
      result.push_back(num);
    }
  }

  for (auto &r : result)
  {
    cout << r << " ";
  }
  cout << endl;

  return result;
}

unsigned sumDecomposedNumbersDigits(const vector<unsigned> &decomposedNumbersDigits)
{
  unsigned result = 0;

  for (const unsigned &number : decomposedNumbersDigits)
  {
    result += number;
  }

  return result;
}

unsigned sumDigits(unsigned number)
{
  unsigned result = 0;
  unsigned digitsAmount = log10(number) + 1;
  unsigned tenths = getTenths(number);

  if (tenths == 0)
  {
    return number;
  }

  for (size_t i{0}; i < digitsAmount; ++i)
  {
    if (tenths == 0)
    {
      result += number;
      break;
    }
    unsigned digit = number / (tenths * 10);
    result += digit;
    number = number % (tenths * 10);
    tenths = tenths / 10;
  }

  return result;
}

bool isSmithsNumber(const unsigned &number)
{
  unsigned sumOfDecomposedDigits = sumDecomposedNumbersDigits(getDecomposedNumbersDigitsVector(getDecomposedNumbersEratosten(number)));
  unsigned sumOfDigits = sumDigits(number);

  cout << "sumOfDecomposedDigits: " << sumOfDecomposedDigits << endl;
  cout << "sumOfDigits: " << sumOfDigits << endl;

  if (sumOfDecomposedDigits == sumOfDigits)
  {
    return true;
  }
  return false;
}

int main()
{

  unsigned input;
  cout << "Let's check if a number is a Smith's number" << endl;
  cout << "Your input: ";
  cin >> input;

  cout << "The number " << input;
  if (isSmithsNumber(input))
  {
    cout << " is a smith's number." << endl;
  }
  else
  {
    cout << " is NOT a smith's number" << endl;
  }

  return 0;
}