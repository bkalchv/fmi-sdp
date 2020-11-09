#ifndef _DYN_VECTOR_IMPLEMENTATION_INCLUDED__
#define _DYN_VECTOR_IMPLEMENTATION_INCLUDED__

#include <cassert>


  template<class T>
  inline DynamicVector<T>::DynamicVector(): DynamicVector(constants::MIN_SIZE) {

  }

  template<class T>
  inline DynamicVector<T>::DynamicVector(const unsigned& givenCapacity): capacity{givenCapacity < constants::MIN_SIZE ? constants::MIN_SIZE : givenCapacity}, size{0}, values{new T[capacity]} {

  }

  template<class T>
  inline DynamicVector<T>::DynamicVector(const DynamicVector<T>& other) { // Copy constructor

      capacity = other.getCapacity();
      values = new T[capacity];

      try {
  
        for(size_t a{0}; a < other.getSize(); ++a) {
          values[a] = other.getValues()[a];
          ++size;
        }

      } catch (...) {
        std::cout << "Exception in copy constructor caught!" << std::endl;
        delete[] values;
        throw;
      }
  }

  template<class T>
  inline DynamicVector<T>& DynamicVector<T>::operator=(const DynamicVector<T>& other) {
    if (this != other) {

      if(capacity < other.getSize) {
        this->clear();
        reserveCapacity(other.getCapacity());
      }

      try {
        values = new T[other.getCapacity()];
        capacity = other.getCapacity();

        for(size_t a{0}; a < other.getSize(); ++a) {
          values[a] = other.getValues()[a];		
          ++size;
        }

      } catch (...) {
        std::cout << "Exception in assignment operator caught!" << std::endl;
        throw;
      }
		return *this;
    }
  }

  template<class T>
  inline T& DynamicVector<T>::operator[](size_t idx) {
    if (idx < size) {
      return values[idx];
    } else {
      throw std::out_of_range("Index out of range!");
    }
  }

  template<class T>
  inline const T&  DynamicVector<T>::operator[](size_t idx) const {
    if (idx < size) {
      return values[idx];
    } else {
      throw std::out_of_range("Index out of range!");
    }
  }
  
  template<class T>
  inline unsigned DynamicVector<T>::getSize() const {
    return size;
  }

  template<class T>
  inline unsigned DynamicVector<T>::getCapacity() const {
    return capacity;
  }

  template<class T>
  inline T* DynamicVector<T>::getValues() const {
    return values;
  }

  template<class T>
  inline bool DynamicVector<T>::isEmpty() const {
    return size == 0;
  }

  template<class T>
  inline bool DynamicVector<T>::isFull() const {
    return capacity == size;
  }
  
  template<class T>
  inline void DynamicVector<T>::reserveCapacity(size_t desiredCapacity) {

    if (capacity < desiredCapacity) {

      unsigned newCapacity = values ? capacity : 1;

      while(newCapacity < desiredCapacity) {
        newCapacity *= 2;
      }

      T* buffer{new T[newCapacity]};

      try {

        for(unsigned i{0}; i < size; ++i) {
          buffer[i] = values[i];
        }

      } catch (...) {
        delete[] buffer;
        throw;
      }
      
      delete[] values;
      values = buffer;
      capacity = newCapacity;
    } else {
      return;
    }
  }

  template<class T>
  inline DynamicVector<T>::~DynamicVector() {
   this->clear();
  }


  template<class T>
  inline void DynamicVector<T>::push_back(const T& element) {
    reserveCapacity(size + 1);
    values[size] = element;
    ++size; 
  }

  template<class T>
  inline void DynamicVector<T>::pop() {
    --size;
  }

  template<class T>
  inline void DynamicVector<T>::clear() {
    delete[] values;
    values = nullptr;
    capacity = 0;
    size = 0;
  }

  template<class T>
  inline void DynamicVector<T>::print() {
    std::cout << "SIZE: " << size << " CAPACITY:" << capacity << endl;
    for (size_t i{0}; i < size; ++i) {
      std::cout <<  "[" << values[i] << "]";
      if (i + 1 != size) {
        std::cout <<  " -> ";
      }
    }
    std::cout << endl;
  }

#endif