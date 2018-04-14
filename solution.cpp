#include "solution.h"


Solution::
Solution() : solutionValues(9, std::vector<ushort>(9, 0)) {
  
  this->openCells.reserve(81);
  this->assignedCells.reserve(81);
  this->puzzle.buildPuzzle();
  
  Cell *tempCell = nullptr;
  
  for(ushort i = 0; i < 9; i++) {
    for(ushort j = 0; j < 9; j++) {
      
      tempCell = const_cast<Cell *>( this->puzzle.getCell(i, j) );
      if(tempCell->getValue() == 0) {
        this->openCells.push_back( tempCell );
        tempCell->clearCandidates();
        this->setCandidatesAt(i, j);
      } // if
    } // for j
  } // for i
} // ctor




void Solution::
setCell( ushort val ) {
  if( this->openCells.empty() )   return;
  
  Cell *cell = this->openCells.back();
  this->openCells.pop_back();
  
  cell->setValue(val);
  this->assignedCells.push_back(cell);
  
  ushort x_coord = cell->getX();
  ushort y_coord = cell->getY();
  
  // update candidates in same row
  for( ushort i = 0; i < 9; i++ ) {
    if( this->puzzle.getValue( x_coord, i ) == 0 ) {
      this->puzzle.unsetCandidate( x_coord, i, val );
    } // if
  } // for
  
  // update candidates in same column
  for( ushort i = 0; i < 9; i++ ) {
    if( this->puzzle.getValue( i, y_coord ) == 0 ) {
      this->puzzle.unsetCandidate( i, y_coord, val );
    } // if
  } // for
  
  // update candidates in same subgrid
  ushort x_start = ( x_coord / 3 ) * 3;
  ushort y_start = ( y_coord / 3 ) * 3;
  
  for( ushort i = x_start; i < x_start + 3; i++ ) {
    for( ushort j = y_start; j < y_start + 3; j++ ) {
      if( this->puzzle.getValue( i, j ) == 0 ) {
        this->puzzle.unsetCandidate( i, j, val );
      } // if
    } // for j
  } // for i
  
} // Solution::setCell()




void Solution::
unsetCell() {
  if( this->assignedCells.empty() )   return;
  
  Cell *cell = this->assignedCells.back();
  
  // store the cell's value before it is unset (to be used for updating candidate values)
  ushort value = cell->getValue();
  
  this->assignedCells.pop_back();
  
  cell->unsetValue();
  this->openCells.push_back(cell);
  
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // TODO: make sure the rest of this function (i.e. updating the candidates) works properly ///////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  // clear and recalculate all candidates at the popped cell
  cell->clearCandidates();
  this->setCandidatesAt( cell->getX(), cell->getY() );
  
  
  // add candidate value for the affected and unset cells
  // that intersect the unset cell by row, column, or subgrid
  for( ushort i = 0; i < 9; i++ ) {
    
    // check and update by row
    if( this->puzzle.validAt(cell->getX(), i, value) ) {
      this->puzzle.setCandidate(cell->getX(), i, value);
    } // if
    
    // check and update by column
    if( this->puzzle.validAt(i, cell->getY(), value) ) {
      this->puzzle.setCandidate(i, cell->getY(), value);
    } // if
  } // for i
  
  
  // lastly update by subgrid
  ushort x_start = (cell->getX() / 3) * 3;
  ushort y_start = (cell->getY() / 3) * 3;
  
  for( ushort i = x_start; i < x_start + 3; i++ ) {
    for( ushort j = y_start; j < y_start + 3; j++ ) {
      if( this->puzzle.validAt(i, j, value) ) {
        this->puzzle.setCandidate(i, j, value);
      } // if
    } // for j
  } // for i
  
} // Solution::unsetCell()




void Solution::
setCandidatesAt( ushort x_coord, ushort y_coord ) {
  
  for( ushort i = 1; i < 10; i++ ) {
    
    if( this->puzzle.validAt( x_coord, y_coord, i ) ) {
      this->puzzle.setCandidate( x_coord, y_coord, i );
    } // if
  } // for
} // Solution::setCandidates()




bool Solution::
validateWithBacktrack() {
  
  ushort count = 0;
  backtrackChecker(count);
  
  for( ushort i = 0; i < 9; i++ ) {
    for( ushort j = 0; j < 9; j++ ) {
      this->puzzle.setValue( i, j, this->solutionValues[j][i] );
    } // for j
  } // for i
  
  return ( count == 1 );
  
} // Solution::solveWithBacktrack()



/*
void Solution::
backtrackSolver( ushort &count, std::vector<ushort> values ) {
  
  
  
} // Solution::backtrackSolver()
*/




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////      TODO TODO TODO TODO     //////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// how to check for duplicate solutions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Solution::
backtrackChecker( ushort &count ) {
  
  if( count > 1 )   return;
  
  if( this->openCells.empty() ) {
    
    // TODO: if unique solution, then update solution (if count == 0) and then count++
    //          regardless return (don't unsetCell)
    if( this->uniqueSolution() ) {
      if( count == 0 )
        this->updateSolution();
      
      count++;
    } // if
    //this->unsetCell();
    return;
  } // if
  
  Cell const *cell = this->getNextOpenCell();
  for( ushort value = 1; value < 10; value++ ) {
    
    // if value is not a valid candidate, continue
    //if( !( this->puzzle.hasCandidate(cell->getX(), cell->getY(), value)) )
    //  continue;
    
    // set the value if valid
    //if( this->puzzle.validAt(cell->getX(), cell->getY(), value) )
    //  this->setCell(value);
    
    if( this->puzzle.hasCandidate(cell->getX(), cell->getY(), value) ) {
      this->setCell(value);
      this->backtrackChecker( count );
      
      //if( !(this->openCells.empty()) )
      if( count < 2 )
        this->unsetCell();
    } // if
  } // for value
} // Solution::backtrackSolver()




bool Solution::
uniqueSolution() const {
  for( ushort i = 0; i < 9; i++ ) {
    for( ushort j = 0; j < 9; j++ ) {
      if( this->solutionValues[j][i] == this->puzzle.getValue( i, j ) )
        return false;
    } // for j
  } // for i
  
  return true;
} // Solution::uniqueSolution()




void Solution::
updateSolution() {
  
  for( ushort i = 0; i < 9; i++ ) {
    for( ushort j = 0; j < 9; j++ ) {
      this->solutionValues[j][i] = this->puzzle.getValue( i, j );
    } // for j
  } // for i
  
} // Solution::updateSolution()





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////      TODO TODO TODO TODO     //////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//        Each time a cell's candidates gets updated, check that it doesn't leave that cell with no candidates
//              If it does, then return False (make the return type bool) and handle this accordingly (i.e. early terminate
//                                                                                                        the backtrack stack frame)
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////













































