#include "sdp1-task5-matrixclass.h"
#include "sdp1-task5-matrixclass.cpp"
#include <iostream>

using namespace std;

int main() {
  
  auto matrix1 = Matrix();
  auto matrix2 = Matrix();

  try {
    cout << "Matrix1: ";
    matrix1.inputRowByRow();
    
    cout << "Matrix2: ";
    matrix2.inputRowByRow();

    Matrix result{matrix1*matrix2};

    cout << "Matrix1: ";
    matrix1.print();
    cout << "    *    " << endl;
    cout << "Matrix2: ";
    matrix2.print();
    cout << "    =    " << endl;
    cout << "Result: " << endl;
    result.print();

    Matrix result1;
    result1 = matrix1*matrix2;
    result.print();

  } catch (std::runtime_error& e) {
    cerr << "Runtime_error caught!" << endl << e.what() << endl;
  }
 
  return 0;
}