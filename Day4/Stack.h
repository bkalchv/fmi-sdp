//push, pop, isEmpty, top
#ifndef __STACK__H__BY__DYNAMIC__VECTOR__H__
#define __STACK__H__BY__DYNAMIC__VECTOR__H__
#include <stddef.h>

template <class T>
class Stack {
  private:
    size_t capacity;
    size_t size;
    T* values;

  public:
    Stack();
    Stack(const Stack<T>&);
    Stack& operator=(Stack<T>);
    ~Stack();
    void push(const T&);
    void pop();
    const T& top();
    bool isEmpty();

  private:
    bool resize();
    void swap(Stack<T>&);
    void clean();

};

#endif