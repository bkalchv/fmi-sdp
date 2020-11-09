/******************************************************************
*  Sample code to the course "Data Structures And Algorithms", FMI
*
* A template class, representing dynamic array. 
* The goal is to illustrate the idea of separating the class
* interface from the implementation details in case of template usage.
* Also inner template classes are introduced and the concept of Iterator.
*
* The template type imply the following members:
* Default and copy constructors, assignment operator
* and equality operator.
*
* (cc) Petar Armyanov, FMI, 2020
*******************************************************************/

#pragma once

#ifndef _DYN_ARRAY_INCLUDED__
#define _DYN_ARRAY_INCLUDED__

#include <stdexcept>
#include <assert.h>
#include <stddef.h>

namespace fmi
{
namespace sdp
{
    template <class DataType>
    class DynArray
    {
    public:
////
// Class life-cycle members

        // Ctor that sets the initial capacity
        explicit DynArray(int initialCapacity = 16);

        // dtor
        ~DynArray();

        // copy ctor
        DynArray(const DynArray& other);

        // assignment operator
        DynArray& operator= (const DynArray& other);

        // A constructor, that creates an array as a copy.
        // The source array can have different type of elements!
        // In that case implicit cast operator between two types is expected.
        template <class OtherType>
        DynArray(const DynArray<OtherType>& other);

        // Assignment operator. 
        // The source array can have different type of elements!
        // In that case implicit cast operator between two types is expected.
        template <class OtherType>
        DynArray<DataType>& operator= (const DynArray<OtherType>& other);


////
// Addition, remove and search of elements

        // Appends an array to the current.
        // The source array can have different type of elements!
        // In that case implicit cast operator between two types is expected.
        template <class OtherType>
        DynArray<DataType>& operator+= (const DynArray<OtherType>& other);
        
        // Appends an element at the end of the array.
        DynArray<DataType>& operator+= (const DataType& data);
        void append(const DataType& elem);

        // Removes an element at given position
        void remove(size_t index);
        
        // Checks if an element exists in the array.
        // Requires comparison operator for the data type.
        bool contains(const DataType& data) const;

        // Returns a copy of the element at a given position.
        DataType get(size_t index) const;

        // Removes all elements in the array.
        void clear();

////
// Information methods
        // the number of elements in the array
        size_t getSize() const;

        // the actual allocated size of the array
        size_t getCapacity() const;

        // checks if there are elements in the array
        bool isEmpty() const;


////
// Access operators and iterators
        
        // An indexing operators
        const DataType& operator[](size_t index) const;
        DataType& operator[](size_t index);

    protected:
        DataType* data;
        size_t size, capacity;

    private:
////
// helper methods
        
        // ensures there are enough space for given number of elements
        void ensureSize(size_t size);

        // changes the actual allocated space to the given number of elements
        void resize(size_t newCapacity);
    };

#include "DynArray.inl"

} //namespace sdp
} //namespace fmi

#endif //_DYN_ARRAY_INCLUDED__