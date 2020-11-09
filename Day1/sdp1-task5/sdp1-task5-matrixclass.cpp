#include "sdp1-task5-matrixclass.h"

#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

Matrix::Matrix(): rowsAmount{3}, colsAmount{3} {
  
  matrix = new int*[rowsAmount];

  for (size_t rowNr{0}; rowNr < rowsAmount; ++rowNr) { // Allocates memory for an 3x3 matrix
    matrix[rowNr] = new (nothrow) int[colsAmount];
    if (!matrix[rowNr]) {

    }
  }

  for (size_t rowNr = 0; rowNr < rowsAmount; rowNr++) {
      for (size_t colNr = 0; colNr < colsAmount; colNr++) {
        matrix[rowNr][colNr] = 0;
      }
  }
}

Matrix::Matrix(const Matrix& otherMatrix): rowsAmount{otherMatrix.rowsAmount}, colsAmount{otherMatrix.colsAmount}{ //Copy-constructor
  //copy()
  matrix = new int*[rowsAmount];

  for (size_t rowNr{0}; rowNr < rowsAmount; ++rowNr) { // Allocates memory for an 3x3 matrix
    matrix[rowNr] = new (nothrow) int[colsAmount];
  }

  for (size_t rowNr = 0; rowNr < rowsAmount; rowNr++) {
      for (size_t colNr = 0; colNr < colsAmount; colNr++) {
        matrix[rowNr][colNr] = otherMatrix.matrix[rowNr][colNr];
      }
  }

}

Matrix::Matrix(const int& member11, const int& member12, const int& member13, const int& member21, const int& member22, const int& member23, const int& member31, const int& member32, const int& member33): rowsAmount{3}, colsAmount{3} {

  matrix = new int*[3];

  for (size_t rowNr{0}; rowNr < rowsAmount; ++rowNr) { // Allocates memory for an 3x3 matrix
    switch(rowNr) {
      case 0 :
        matrix[0] = new int[colsAmount];
        matrix[0][0] = member11;
        matrix[0][1] = member12;
        matrix[0][2] = member13;
        break;
      case 1 : 
        matrix[1] = new int[colsAmount];
        matrix[1][0] = member21;
        matrix[1][1] = member22;
        matrix[1][2] = member23;
        break;
      case 2 :
        matrix[2] = new int[colsAmount];
        matrix[2][0] = member31;
        matrix[2][1] = member32;
        matrix[2][2] = member33;
        break;
      default :
        throw new std::runtime_error("The matrix's row exceeds 3.");
    }
  }
}

Matrix& Matrix::operator=(const Matrix& otherMatrix) {
  if(this->matrix != otherMatrix.matrix) {
    cout << "operator= called" << endl;
    rowsAmount = otherMatrix.rowsAmount;
    colsAmount = otherMatrix.colsAmount;

    //copy()
    //delete()

    // Deallocate first! (If working in other array)
    /*for (size_t rowNr{0}; rowNr < rowsAmount; ++rowNr) { // Allocates memory for an 3x3 matrix
      matrix[rowNr] = new int[colsAmount];
    }
    */

    for (size_t rowNr = 0; rowNr < rowsAmount; rowNr++) {
        for (size_t colNr = 0; colNr < colsAmount; colNr++) {
          matrix[rowNr][colNr] = otherMatrix.matrix[rowNr][colNr];
        }
    }

  return *this;
  }

  return *this;
}

size_t Matrix::getRowsAmount() const {
  return this->rowsAmount;
}

size_t Matrix::getColsAmount() const {
  return this->colsAmount;
}

int** Matrix::getMatrix() const {
  return this->matrix;
}

Matrix::~Matrix() {
  if(this->matrix != nullptr) {
    for(size_t rowNr{0}; rowNr < rowsAmount; ++rowNr) {
        delete[] matrix[rowNr];
    }
    delete[] matrix;
    matrix = nullptr;
  }
}

Matrix Matrix::operator*(const Matrix& otherMatrix) const {
   if(colsAmount != otherMatrix.getColsAmount()) {
    throw std::runtime_error("Matrices must have domino-like dimension, in order for them to be multiplied with one another!");
  } else {
    Matrix resultMatrix = Matrix();

    for(size_t rowNr{0}; rowNr < rowsAmount; ++rowNr) {
      for(size_t colNrSecondMatrix{0}; colNrSecondMatrix < otherMatrix.getColsAmount(); ++colNrSecondMatrix) {
        for(size_t colNrFirstMatrix{0}; colNrFirstMatrix < colsAmount; ++colNrFirstMatrix) {
          resultMatrix.matrix[rowNr][colNrSecondMatrix] +=  matrix[rowNr][colNrFirstMatrix] * otherMatrix.matrix[colNrFirstMatrix][colNrSecondMatrix]; 
        }
      }
    }

    return resultMatrix;
  }
}

void Matrix::inputRowByRow() {
  string line;
  cout << "Input for the matrix(" << rowsAmount << "x" << colsAmount << "): \n";  
  for(size_t rowNr{0}; rowNr < rowsAmount;) {
    cout << rowNr+1 << ". row of the matrix(separate input by spaces): ";

    getline(cin, line);
    istringstream lineStream(line);
    int value;
    vector<int> vectorOfInputValues;
    while(lineStream >> value) {
      vectorOfInputValues.push_back(value);
    }

    if (vectorOfInputValues.size() != colsAmount) {
      cout << "Amount of input elements for the row different than " << colsAmount << "! Try again!" << endl;
      continue;
    }

    for(size_t colNr{0}; colNr < colsAmount; ++colNr) {
      matrix[rowNr][colNr] = vectorOfInputValues[colNr];
    }

    ++rowNr;
  } 
}

void Matrix::print() {
  cout << endl;

  for(size_t rowNr{0}; rowNr < rowsAmount; ++rowNr) {
    for(size_t colNr{0}; colNr < colsAmount; ++colNr) {
      if(colNr==0) {
        cout << "|";
      }

      cout << this->matrix[rowNr][colNr];

      if(colNr+1 == colsAmount) {
        cout << "|" << endl;
      } else {
        cout << " ";
      }
    }
  }
  cout << endl;
}
