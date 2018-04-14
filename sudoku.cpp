#include <cstdlib>
#include "solution.h"

int main() {
  
  std::cout << "Building puzzle...\n\n";
  Solution *solution = new Solution;
  
  
  solution->print();
  
  std::cout << "Solving puzzle...\n\n";
  bool foundSolution = solution->validateWithBacktrack();
  
  std::cout << ( foundSolution ? "Found " : "Did not find " ) << "a singular solution\n\n";
  solution->print();
  
  delete solution;
  solution = nullptr;
  
  return 0;
}