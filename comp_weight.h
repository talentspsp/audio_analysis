#ifndef _COMP_WEIGHT_H
#define _COMP_WEIGHT_H

#include <cstdlib>
using namespace std;

/*
  This function is used to get the weights of different components in a given distribution
  Input: 
  psfz:a 1*nums double* array. each double* is the components of a source, which is a 2-D matrix, each row is a distribution in frequency domain
  numz:a 1*nums array, each element is the number of the components of the corresponding source
  nums:number of sources
  numf:length of frequency domain
  Vft:a numt*numf array, each row is a distribution in frequency domain
  numt: length in time domain
  maxiter: max number of iteration
  result_pts: a numt*nums array, each row the probability of different sources, used for returning, space will be malloc inside the function
  ptzs: a 1*nums double* array. this part of space need to be malloced before calling this function. each double* points to a numt*numz[s] array, each row is the probability of the components of the sources, this part of space will be allocated inside the function. used for returning
  *****Notice: this function doesn't check the validation of the input!******
 */
void comp_weight( double** psfz,size_t* numz, size_t nums,size_t numf, double* Vft, size_t numt, size_t maxiter, double** result_pts, double** ptzs);
#endif
