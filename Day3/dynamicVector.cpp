#include "dynamicVector.h"
#include <iostream>
#include <cassert>

using namespace std;
using namespace sdp;

int main()
{
// test the constructors
    DynamicVector<int> intArray;
    DynamicVector<int> intArray1{intArray};

// test addition
    for (size_t i = 0; i < 10; ++i) {
        intArray.push_back(i);
    }
    
    intArray.print();
//test the operator[]
    assert(intArray[1] == 1);

    for (size_t i = 0; i < 3; ++i) {
        intArray.push_back(i);
    }

    assert(intArray.getCapacity() == 24);

    intArray.pop();
    intArray.print(); 

// test the clear method
    intArray.clear();
    assert(intArray.isEmpty());

    return 0;
}