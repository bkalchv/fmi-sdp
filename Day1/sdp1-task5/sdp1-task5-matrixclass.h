#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

using namespace std;

// A matrix class, defining a 3x3 matrix
class Matrix {
  private:
    size_t rowsAmount;
    size_t colsAmount;
    int** matrix;
  public:
    Matrix();
    Matrix(const Matrix&);
    Matrix(const int&,const int&,const int&,const int&,const int&,const int&,const int&,const int&, const int&);
    ~Matrix();
    Matrix operator*(Matrix&);
    Matrix& operator=(const Matrix&);
    void print();
    size_t getRowsAmount();
    size_t getColsAmount();
    int** getMatrix();
    void inputRowByRow();
};

#endif 