
#ifndef _DYN_VECTOR_
#define _DYN_VECTOR_
#include <stdexcept>
#include <iostream>

using namespace std;

namespace constants {
  const unsigned MIN_SIZE{6};
}

namespace sdp {
  template <class T>
  class DynamicVector {

  private:
  unsigned capacity;
  unsigned size;
  T* values;

  public:
  DynamicVector();
  DynamicVector(const unsigned& desiredSize);
  DynamicVector(const DynamicVector<T>& other);
  DynamicVector& operator=(const DynamicVector<T>&);
  T& operator[](size_t idx);
  const T& operator[](size_t idx) const;
  unsigned getCapacity() const;
  unsigned getSize() const;
  T* getValues() const;
  bool isEmpty() const;
  bool isFull() const;
  void push_back(const T& element);
  void pop();
  void clear();
  void print();
  ~DynamicVector();

  private:
  void reserveCapacity(size_t);
  void shrinkToFitCapacity();
  };

  #include "dynamicVector.inl"
}




#endif