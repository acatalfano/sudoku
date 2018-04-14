#ifndef SOLUTION_H
#define SOLUTION_H

#include "puzzle.h"

class Solution {
public:
  // default ctor
  Solution();
  
  void print() const    { this->puzzle.print(); }
  
  // setters
  void setCell( ushort val );
  
  void unsetCell();
  
  // getters
  Cell const *getNextOpenCell() const {
    if( this->openCells.empty() )   return nullptr;
    
    return this->openCells.back();
  }
  
  bool validateWithBacktrack();
  
private:
  void backtrackChecker( ushort &count );
  
  void setCandidatesAt( ushort x_coord, ushort y_coord );
  
  bool uniqueSolution() const;
  
  void updateSolution();
  
private:
  std::vector<std::vector<ushort>> solutionValues;
  std::vector<Cell *> openCells;
  std::vector<Cell *> assignedCells;
  Puzzle puzzle;
  
}; // class Solution

#endif