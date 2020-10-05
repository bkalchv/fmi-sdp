#include <iostream>

// Сума на две матрици + валидация 
// Addition of two matrices with dynamic allocation with validation 
// (Matrices only with the same dimensions can be added to one another)

using namespace std;

void printMatrix(int** matrixToPrint, int rows, int columns) {
  cout << endl;
  for(int rowNr{0}; rowNr < rows; ++rowNr) {
    for(int colNr{0}; colNr < columns; ++colNr) {
      if(colNr==0) {
        cout << "|";
      }

      cout << matrixToPrint[rowNr][colNr];

      if(colNr+1 == columns) {
        cout << "|" << endl;
      } else {
        cout << " ";
      }
    }
  }
  cout << endl;
}

int** sumMatrices(int **m1, int m1rows, int m1cols, int **m2, int m2rows, int m2cols) {

  if(m1rows != m2rows || m1cols != m2cols) {
    throw std::runtime_error("Matrices must have equal dimensions, in order for them to be added!");
  } else {
    int** resultMatrix;
    int rows = m1rows;
    int cols = m1cols;

    resultMatrix = new int*[m1rows];
    for(int rowNr{0}; rowNr < rows; ++rowNr) {
      resultMatrix[rowNr] = new int[cols];
    }

    for(int rowNr{0}; rowNr < rows; ++rowNr) {
      for(int colNr{0}; colNr < cols; ++colNr) {
        resultMatrix[rowNr][colNr] = m1[rowNr][colNr] + m2[rowNr][colNr];
      }
    }

    return resultMatrix;
  }
  
}


int main() {
  int rows1, cols1, rows2, cols2;

  cout << "Input the amount of desired rows for the first matrix: ";
  while(!(cin >> rows1)) {
    cin.clear();
    cin.ignore(100, '\n');
    cout << "Invalid input! Try again! (Integers expected)" << endl;
  }
  cout << endl;

  cout << "Input the amount of desired cols for the first matrix: ";
  while(!(cin >> cols1)) {
    cin.clear();
    cin.ignore(100, '\n');
    cout << "Invalid input! Try again! (Integers expected)" << endl;
  }
  cout << endl;

  // Allocate space for the matrix
  int **matrix1 = new int*[rows1];
  for(int rowNr{0}; rowNr < rows1; ++rowNr) {
    matrix1[rowNr] = new int[cols1];
  }

  cout << "Input for first matrix(" << rows1 << "x" << cols1 << "): \n";  
  for(int rowNr{0}; rowNr < rows1; ++rowNr) {
    cout << rowNr+1 << ". row of 1st matrix: " << endl;
    for(int colNr{0}; colNr < cols1; ++colNr) {
      cout << " ";
      cin >> matrix1[rowNr][colNr];
      if(colNr+1 == cols1) {
        cout << endl;
      }
    }
    cout << endl;
  } 

  cout << "Input the amount of desired rows for the second matrix: ";
  while(!(cin >> rows2)) {
    cin.clear();
    cin.ignore(100, '\n');
    cout << "Invalid input! Try again! (Integers expected)" << endl;
  }
  cout << "\n";

  cout << "Input the amount of desired cols for the second matrix: ";
  while(!(cin >> cols2)) {
    cin.clear();
    cin.ignore(100, '\n');
    cout << "Invalid input! Try again! (Integers expected)" << endl;
  }
  cout << endl;
  

  //Allocate space for matrix2
  int** matrix2 = new int*[rows2];
  for(int rowNr{0}; rowNr < rows2; ++rowNr) {
    matrix2[rowNr] = new int[cols2];
  }

  cout << "Input for second matrix(" << rows2 << "x" << cols2 << "): \n";  
  for(int rowNr{0}; rowNr < rows2; ++rowNr) {
    cout << rowNr+1 << ". row of 2nd matrix: " << endl;
    for(int colNr{0}; colNr < cols2; ++colNr) {
      cout << " ";
      cin >> matrix2[rowNr][colNr]; 
      if(colNr+1 == cols2) {
        cout << endl;
      }
    }
  }
  
  try {
    int** sum = sumMatrices(matrix1, rows1, cols1, matrix2, rows2, cols2);

    printMatrix(matrix1, rows1, cols1);
    cout << " + " << endl;

    printMatrix(matrix2, rows2, cols2);
    cout << " = " << endl;

    printMatrix(sum, rows1, cols1);

    //Deallocationg memory
    for(int rowNr{0}; rowNr < rows1; ++rowNr) {
      delete[] matrix1[rowNr];
    }
    delete[] matrix1;

    for(int rowNr{0}; rowNr < rows2; ++rowNr) {
      delete[] matrix2[rowNr];
    }
    delete[] matrix2;

    for(int rowNr{0}; rowNr < rows1; ++rowNr) {
      delete[] sum[rowNr];
    }
    delete[] sum;
  } catch (std::runtime_error& e) {
    cerr << "Runtime_error caught!" << endl << e.what() << endl;
  }
  
  return 0;
}