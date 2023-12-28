//-------------------------------------------------
// grid template class
//-------------------------------------------------
// author: aiden kiefer
// cs211
// professor reckinger
// nov 19 2023
//-------------------------------------------------
// this template class represents a 2-dimensional grid,
// where each element can be of type T. it provides basic
// functionalities such as both default and parameterized
// construction, destruction, both copy construction and 
// assignment, and access to individual elements in the grid       
//-------------------------------------------------

#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>

using namespace std;

template<typename T>
class Grid {
private:
  struct ROW {
    T*  Cols;     // array of column elements
    size_t NumCols;  // total # of columns 
  };

  ROW* Rows;     // array of ROWs
  size_t  NumRows;  // total # of rows 
    
public:
  //
  // default constructor:
  //
  // called automatically by to construct a 4x4 grid 
  // all elements initialized to the default value of T
  //
  Grid() {
    Rows = new ROW[4];  // 4 rows
    NumRows = 4;

    // initialize each row to have 4 columns
    for (size_t r = 0; r < NumRows; ++r) {
      Rows[r].Cols = new T[4];
      Rows[r].NumCols = 4;

      // initialize the elements to their default value
      for (size_t c = 0; c < Rows[r].NumCols; ++c) {
        Rows[r].Cols[c] = T();  // default value for type T
      }
    }
  }

  //
  // parameterized constructor:
  //
  // called automatically to construct a grid 
  // with R rows where each row has C columns 
  // all elements initialized to the default value of T
  //
  Grid(size_t R, size_t C) {
    //allocate memory for rows
    Rows = new ROW[R];
    NumRows = R;
    //initialize each row to have C columns
    for (size_t r = 0; r < NumRows; ++r) {
      Rows[r].Cols = new T[C];
      Rows[r].NumCols = C;
      //initialize the elements to their default value
      for (size_t c = 0; c < Rows[r].NumCols; ++c) {
        Rows[r].Cols[c] = T();  // default value for type T
      }
    }
  }
    
  //
  // destructor:
  //
  // automatically called to free memory for this grid
  //
  virtual ~Grid() {
    //deallocate memory for each row and its columns
    for (size_t r = 0; r < NumRows; ++r) {
      delete[] Rows[r].Cols;
    }
    //deallocate memory for rows
    delete[] Rows;
  }

  //
  // copy constructor:
  //
  // automatically called to construct a grid that contains a
  // copy of an existing grid
  //
  Grid(const Grid<T>& other) {
    // copy the number of rows
    NumRows = other.NumRows;
    // allocate memory for rows
    Rows = new ROW[NumRows];
    // copy each row and its columns
    for (size_t r = 0; r < NumRows; ++r) {
      Rows[r].NumCols = other.Rows[r].NumCols;
      Rows[r].Cols = new T[Rows[r].NumCols];
      // copy elements from the other grid
      for (size_t c = 0; c < Rows[r].NumCols; ++c) {
        Rows[r].Cols[c] = other.Rows[r].Cols[c];
      }
    }
  }
    
  //
  // copy operator=
  //
  // called when one grid is assigned into another
  //
  Grid& operator=(const Grid& other) {
    //check for self-assignment
    if (this != &other) {
      //deallocate existing memory
      for (size_t r = 0; r < NumRows; ++r) {
        delete[] Rows[r].Cols;
      }
      delete[] Rows;
      //copy the number of rows
      NumRows = other.NumRows;
      //allocate memory for rows
      Rows = new ROW[NumRows];
      //copy each row and its columns
      for (size_t r = 0; r < NumRows; ++r) {
        Rows[r].NumCols = other.Rows[r].NumCols;
        Rows[r].Cols = new T[Rows[r].NumCols];
        //copy elements from the other grid
        for (size_t c = 0; c < Rows[r].NumCols; ++c) {
          Rows[r].Cols[c] = other.Rows[r].Cols[c];
        }
      }
    }

    return *this;
  }

  //
  // numrows
  //
  // returns the # of rows in the grid  
  //
  size_t numrows() const {
    return NumRows;
  }
  

  //
  // numcols
  //
  // returns the # of columns in row r  
  //
  size_t numcols(size_t r) const {
    if (r < NumRows) {
      return Rows[r].NumCols;
    } else {
      throw out_of_range("Invalid row index");
    }
  }

  //
  // size
  //
  // returns the total # of elements in the grid
  //
  size_t size() const {
    size_t totalSize = 0;
    for (size_t r = 0; r < NumRows; ++r) {
      totalSize += Rows[r].NumCols;
    }
    return totalSize;
  }

  //
  // ()
  //
  // returns a reference to the element at location (r, c)
  //
  T& operator()(size_t r, size_t c) {
    if (r < NumRows && c < Rows[r].NumCols) {
      return Rows[r].Cols[c];
    } else {
      throw out_of_range("Invalid row or column index");
    }
  }

  //
  // _output
  //
  // outputs the contents of the grid used for debugging purposes
  //
  void _output() {
    for (size_t r = 0; r < NumRows; ++r) {
      for (size_t c = 0; c < Rows[r].NumCols; ++c) {
        cout << Rows[r].Cols[c] << " ";
      }
      cout << endl;
    }
  }

};
