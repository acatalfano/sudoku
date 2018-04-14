#include <algorithm>
#include <chrono>
#include <numeric>
#include <random>
#include <stack>

#include "puzzle.h"





// Puzzle ctor
Puzzle::
Puzzle() :  data( 9, std::vector<Cell *> (9) )    {
  
  this->unfilled.reserve(81);
  
  Cell *tempCell = nullptr;
  
  for(ushort i = 0; i < 9; i++) {
    for(ushort j = 0; j < 9; j++) {
      tempCell = new Cell(i, j);
      this->unfilled.push_back( tempCell );
      
      this->data[j][i] = tempCell;
      
    } // for j
  } // for i
} // Puzzle ctor




// copy ctor
Puzzle::
Puzzle( const Puzzle &puzzle ) : data( 9, std::vector<Cell *> (9) ) {
  
  this->makeCopy(puzzle);
  
} // Puzzle copy ctor




// copy assignment operator
Puzzle &Puzzle::
operator=( const Puzzle &puzzle ) {
  this->data = std::vector<std::vector<Cell *>> ( 9, std::vector<Cell *> (9) );
  this->makeCopy(puzzle);
  
  return *this;
  
} // Puzzle copy assignment operator




// copy helper
void Puzzle::
makeCopy( const Puzzle &puzzle ) {
  
  Cell *tempCell = nullptr;
  
  for( ushort i = 0; i < 9; i++ ) {
    for( ushort j = 0; j < 9; j++ ) {
      
      tempCell = new Cell( *(puzzle.data[i][j]) );
      this->data[i][j] = tempCell;
      
      if(tempCell->getValue() == 0)
        this->unfilled.push_back( tempCell );
    } // for j
  } // for i
  
} // Puzzle::makeCopy()




// Puzzle dtor
Puzzle::
~Puzzle() {
  
  for( ushort i = 0; i < 9; i++ ) {
    for( ushort j = 0; j < 9; j++ ) {
      
      delete this->data[i][j];
      this->data[i][j] = nullptr;
    }
  }
} // Puzzle dtor




void Puzzle::
print() const {
  for(ushort i = 0; i < 9; i++) {
    
    std::cout << std::endl << ' ';
    
    if( i == 3 || i == 6 ) {
      std::cout << "------+-------+-------" << std::endl << ' ';
    }    
    
    for(ushort j = 0; j < 9; j++) {
      
      if( j == 3 || j == 6 ) {
        std::cout << "| ";
      }
      
      if(this->data[i][j]->getValue() == 0) {
        std::cout << ". ";
      }
      else {
        std::cout << this->data[i][j]->getValue() << ' ';
      }
    } // for j
  } // for i
  
  std::cout << std::endl;
  
} // Puzzle::print()



void Puzzle::
buildPuzzle() {
  
  // first create a completely filled and valid grid
  this->fillGrid();
  
  
  
  // selectively clear cells that allow for only one solution to the puzzle
  
  // create a vector of all the filled cells from which cells will be selected to be cleared
  std::vector<Cell const*> filled;
  filled.reserve(81);
  
  
  for( ushort i = 0; i < 9; i++ ) {
    for( ushort j = 0; j < 9; j++ ) {
      
      filled.push_back( this->getCell(i, j) );
      
    }
  }
  
  
  // shuffle the vector
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  auto randomGen = std::default_random_engine(seed);
  std::shuffle( filled.begin(), filled.end(), randomGen );
  
  Cell const *cell = nullptr;
  ushort value = 0;
  
  while( !filled.empty() ) {
    cell = filled.back();
    filled.pop_back();
    
    value = cell->getValue();
    this->unsetValue( cell->getX(), cell->getY() );
    
    if( this->isUnique( filled ) )
      continue;
    else
      this->setValue( cell->getX(), cell->getY(), value );
    
  } // while
} // Puzzle::buildPuzzle()






void Puzzle::
fillGrid() {
  
  std::vector<Cell const*> filled;
  ushort matches = 0;
  AtLeastOne pred;
  
  this->backtrack_fillGrid( this->unfilled, filled, matches, pred );
  
} // Puzzle::fillGrid()




template <class StateFunctor>
void Puzzle::
backtrack_fillGrid( std::vector<Cell const *> &unfilled,      std::vector<Cell const *> &filled,
                    ushort &matches,                          StateFunctor &condition              ) {
  
  
  
  if( condition(matches) != State::next )    return;
  
  if( condition(matches) == State::next && unfilled.empty() ) {
    matches++;
    return;
  } // if
  
  
  
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  auto randomGen = std::default_random_engine(seed);
  
  std::vector<ushort> values(9);
  std::iota(values.begin(), values.end(), 1);
  std::shuffle( values.begin(), values.end(), randomGen );
  
  ushort val = 0;
  Cell const *cell = nullptr;
  
  
  while( !unfilled.empty() && !values.empty() ) {
    val = values.back();
    values.pop_back();
    
    cell = unfilled.back();
    unfilled.pop_back();
    filled.push_back(cell);
    
    if( this->validAt( cell->getX(), cell->getY(), val ) ) {
      this->setValue( cell->getX(), cell->getY(), val );
      backtrack_fillGrid( unfilled, filled, matches, condition );
    } // if
    
    if( condition(matches) == State::next ) {
      this->unsetValue( cell->getX(), cell->getY() );
      filled.pop_back();
      unfilled.push_back(cell);
    } // if
    
  } // while
  
} // backtrack_fillGrid()



// TODO TODO TODO
bool Puzzle::
isUnique( const std::vector<Cell const*> &filled ) {
  
  Puzzle copy(*this);
  
  std::vector<Cell const*> filledCopy(filled);
  
  ushort matches = 0;
  ExactlyOne pred;
  copy.backtrack_fillGrid( copy.unfilled, filledCopy, matches, pred );
  
  return (pred(matches) != State::failure);
  
} // Puzzle::isUnique()




bool Puzzle::
validAt(ushort x_coord, ushort y_coord, ushort value) const {
  
  return !( rowContains(y_coord, value) || colContains(x_coord, value) || subgridContains(x_coord, y_coord, value) );
  
} // Puzzle::validAt()



bool Puzzle::
rowContains(ushort y_coord, ushort value) const {
  
  for(ushort i = 0; i < 9; i++) {
    
    if(this->data[y_coord][i]->getValue() == value)
      return true;
  }
  
  return false;
  
} // Puzzle::rowContains()



bool Puzzle::
colContains(ushort x_coord, ushort value) const {
  
  for(ushort i = 0; i < 9; i++) {
    
    if(this->data[i][x_coord]->getValue() == value)
      return true;
  }
  
  return false;
  
} // Puzzle::colContains()



bool Puzzle::subgridContains(ushort x_coord, ushort y_coord, ushort value) const {
  
  ushort xStart = x_coord / 3 * 3;
  ushort yStart = y_coord / 3 * 3;
  
  for(ushort i = xStart; i < xStart + 3; i++) {
    for(ushort j = yStart; j < yStart + 3; j++) {
      
      if(this->data[j][i]->getValue() == value)
        return true;
    } // for j
  } // for i
  
  return false;
  
} // Puzzle::subgridContains()




















