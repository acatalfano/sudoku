#ifndef COMPARE_H
#define COMPARE_H

#include "cell.h"


enum class State { next, success, failure };



class AtLeastOne {
public:
  AtLeastOne() {}
  
  State operator()(ushort value) const {
    if( value >= 1 )
      return State::success;
    else
      return State::next;
  } // operator()
  
}; // class AtLeastOne




class ExactlyOne {
public:
  ExactlyOne() {}
  
  State operator()(ushort value) const {
    if( value > 1 )
      return State::failure;
    else
      return State::next;
  } // operator()
  
}; // class ExactlyOne

#endif