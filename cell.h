#ifndef CELL_H
#define CELL_H

#include <vector>

typedef unsigned short ushort;

class Cell {

public:
  // ctor
  Cell(ushort x, ushort y, ushort value = 0) : _value(value), _x(x), _y(y), _candidates(9, false)   {}
  
  //copy ctor
  Cell( const Cell &cell ) : _value(cell._value), _x(cell._x), _y(cell._y), _candidates(cell._candidates) {}
  
  
  
  
  // getters
  ushort getValue() const                 { return this->_value; }
  
  ushort getX() const                     { return this->_x; }
  
  ushort getY() const                     { return this->_y; }
  
  bool hasCandidate(ushort val) const     { return this->_candidates[val - 1]; }
  
  
  
  
  // setters
  void setValue(ushort val)               { this->_value = val; }
  
  void unsetValue()                       { this->_value = 0; }
  
  void setCandidate(ushort val)           { this->_candidates[val - 1] = true; }
  
  void unsetCandidate(ushort val)         { this->_candidates[val - 1] = false; }
  
  void clearCandidates()                  { for( auto &&i: this->_candidates)  i = false; }



private:
  ushort _value;
  ushort _x;
  ushort _y;
  
  std::vector<ushort> _candidates;
}; // class Cell

#endif