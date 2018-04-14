#ifndef PUZZLE_H
#define PUZZLE_H

#include <iostream>

#include "compare.h"
#include "cell.h"

///////////////////////////////////////////////////
//TODO: interface fxns for unfilled vector/////////
///////////////////////////////////////////////////

class Puzzle {

public:
  // default ctor
  Puzzle();
  
  
  
  // copy ctor
  Puzzle( const Puzzle &puzzle );
  
  
  // copy assignment operator
  Puzzle &operator=( const Puzzle &puzzle );
  
  
  // dxr
  ~Puzzle();
  
  // print
  void print() const;
  
  // construct a new puzzle with a single solution
  void buildPuzzle();
  
  
  
  // getters
  ushort getValue( ushort x, ushort y ) const                     { return this->data[y][x]->getValue(); }
  
  const Cell *getCell( ushort x, ushort y ) const                 { return this->data[y][x]; }
  
  bool hasCandidate( ushort x, ushort y, ushort cand ) const      { return this->data[y][x]->hasCandidate( cand ); }
  
  
  
  // setters
  void setValue( ushort x, ushort y, ushort val )                 { this->data[y][x]->setValue( val ); }
  
  void unsetValue( ushort x, ushort y )                           { this->data[y][x]->unsetValue(); }
  
  void setCandidate( ushort x, ushort y, ushort cand )            { this->data[y][x]->setCandidate( cand ); }
  
  void unsetCandidate( ushort x, ushort y, ushort cand )          { this->data[y][x]->unsetCandidate( cand ); }
  
  // return true iff the value is valid at the cell in data[x_coord][y_coord]
  bool validAt( ushort x_coord, ushort y_coord, ushort value ) const;
  
  
private:
  
  // helper for copy ctor and copy assignment operator
  void makeCopy( const Puzzle &puzzle );
  
  
  // helper function for buildPuzzle():
  // build a complete (solved) and legal puzzle
  void fillGrid();
  
  
  
  // backtrack helper function for fillGrid()
  // if checkValid == True, then it will terminate after any solution is found
  // if checkValid == False, then it will terminate (return false) if 2 solutions are found,
  //                                and otherwise return true if exactly 1 solution is found
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public: // TODO remove this line/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  template<class StateFunctor>
  void backtrack_fillGrid(  std::vector<Cell const*> &unfilled,       std::vector<Cell const*> &filled,
                            ushort &matches,                          StateFunctor &compare              );
  
private: //TODO remove this line
  // helper for buildPuzzle() function
  // return true if there is exactly one solution
  bool isUnique( const std::vector<Cell const*> &filled );
  
  // helper functions for validAt()
  bool rowContains( ushort y_coord, ushort value ) const;
  
  bool colContains( ushort x_coord, ushort value ) const;
  
  bool subgridContains( ushort x_coord, ushort y_coord, ushort value) const;
  
  
  
private:
  std::vector<std::vector<Cell *>> data;
  
  std::vector<Cell const*> unfilled;
  
}; // class Puzzle


#endif