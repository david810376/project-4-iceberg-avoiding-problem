///////////////////////////////////////////////////////////////////////////////
// crossing_algs.hpp
//
// Algorithms that solve the iceberg avoiding problem.
//
// All of the TODO sections for this project reside in this file.
//
// This file builds on ices_types.hpp, so you should familiarize yourself
// with that file before working on this file.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>
#include <iostream>

#include "ices_types.hpp"

namespace ices {
// Solve the iceberg avoiding problem for the given grid, using an exhaustive
// optimization algorithm.
//
// This algorithm is expected to run in exponential time, so the grid's
// width+height must be small enough to fit in a 64-bit int; this is enforced
// with an assertion.
//
// The grid must be non-empty.
unsigned int iceberg_avoiding_exhaustive(const grid& setting) {
    
  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);

  // Compute the path length, and check that it is legal.
  const size_t steps = setting.rows() + setting.columns() - 2;
  assert(steps < 64);

  unsigned int count_paths = 0;

  // from 0 to max
  for(size_t bits = 0; bits < pow(2, steps); bits++){
      path candidate(setting); //       candidate = [start]
    //for each possible sequence
    for(size_t k = 0; k < steps; k++){
      size_t bit = (bits >> k) & 1;
      if(bit == 1){
        if(candidate.is_step_valid(STEP_DIRECTION_RIGHT)){
          candidate.add_step(STEP_DIRECTION_RIGHT);
        }
      }
      else{
        if(candidate.is_step_valid(STEP_DIRECTION_DOWN)){
          candidate.add_step(STEP_DIRECTION_DOWN);
        }
      }
      // if reaches destination (location [r-1][c-1])  (its valid)
      if((candidate.final_row() == setting.rows() -1) && (candidate.final_column() == setting.columns()-1)){
        count_paths++;
      }
    }
  }

  return count_paths;
}

// Solve the iceberg avoiding problem for the given grid, using a dynamic
// programming algorithm.
//
// The grid must be non-empty.
unsigned int iceberg_avoiding_dyn_prog(const grid& setting) {

  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);


  const int DIM=100;
  std::vector<std::vector<unsigned>> A(DIM, std::vector<unsigned>(DIM));

  A[0][0] = 1;

  for(unsigned int i = 0; i < setting.rows(); i++){
    for(unsigned int j = 0; j < setting.columns(); j++){
      if(i == 0 && j == 0){  // base case. do nothing 
        continue;
      }
      if(setting.get(i,j) == CELL_ICEBERG){ // none and stays home
        A[i][j] = 0;
        continue;
      }

      int from_above = 0;
      int from_left = 0;

      // when we are not on the top row, and when the cell above is not an iceberg.
      if(i > 0 && A[i-1][j] != 0){
        from_above = A[i-1][j];
      }
      //  when we are not on the leftmost column and when the cell to the left of is not an iceberg.
      if(j > 0 && A[i][j-1] != 0){
        from_left = A[i][j-1];
      }
      // sum of from_above and from_left
      A[i][j] = from_above + from_left;

    }

  }

  return A[setting.rows()-1][setting.columns()-1];
}


}
