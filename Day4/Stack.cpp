#include "Stack.h"
#include <iostream>

namespace constant {
  const size_t MIN_SIZE{6};
}

template <class T>
Stack<T>::Stack(): capacity{0}, size{0}, values{nullptr} {

}

template <class T>
Stack<T>::Stack(const Stack<T>& other): Stack<T>() {
  values = new T[other.capacity];
  capacity = other.capacity;
  
  try {
    for (size_t i{0}; i < other.size; ++i) {
      values[i] = other.values[i];
      ++size;
    }
  } catch (...) {
    std::cout << "Exception in copy constructor caught!" << std::endl;
    delete[] values;
    throw;
  }

}

template <class T>
Stack<T>& Stack<T>::operator=(Stack<T> other) {
  this->swap(other);
  return *this;
}

template <class T>
Stack<T>::~Stack() {
  this->clean();
}

template <class T>
void Stack<T>::push(const T& element) {
  if (size >= capacity) {
    resize();
  }

  values[size] = element;
  ++size;
}

template <class T>
void Stack<T>::pop() {
  if (size > 0) {
    --size;
  }
}

template <class T>
const T& Stack<T>::top() {
  if (!isEmpty()) {
    return values[size - 1];
  } else {
    throw new std::logic_error("Values should not be empty!");
  }
}

template <class T>
bool Stack<T>::isEmpty() {
  return (size == 0);
}

template <class T>
void Stack<T>::swap(Stack<T>& other) {
  using namespace std;
  swap(this->capacity, other.capacity);
  swap(this->size, other.size);
  swap(this->values, other.values);
}

template <class T>
void Stack<T>::clean() {
  delete[] values;
  values = nullptr;
  capacity = 0;
  size = 0;
}

template <class T>
bool Stack<T>::resize() {
  size_t  newCapacity;

    if (capacity == 0) {
        newCapacity = constant::MIN_SIZE;    // Default capacity
    } else {
        newCapacity = 2 * capacity;    // Default scaling factor
    }

    if (!(newCapacity > capacity)) {
        return false;
    }

    T* newValues = new (std::nothrow) T[newCapacity];
    if (!newValues) {
        return false;
    }

    for (size_t i = 0; i < capacity; ++i) {
        std::swap(newValues[i], values[i]);
    }

    delete[] values;
    values = newValues;
    capacity = newCapacity;

    return true;
}