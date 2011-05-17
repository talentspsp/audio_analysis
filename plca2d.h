#ifndef _PLCA_2D_H
#define _PLCA_2D_H

/*
  This function implement plca2d algorithm
  Input:
  data: a lent* lenf. each elememt must be nonnegative, each row is distribution in frequency domain
  lenf: length in frequency domain
  lent: length in time domain
  K: number of components
  max_iter: max number of iteration
  sz: sparseness over z
  result_ptz: Pt(z), used for returning
  result_pfz: P(f|z), used for returning
  result_pz: p(z),used for returning
  max_itertau: max number of iteration for tau inside each update, default 2
  this function will allocate space for ptz and pfz
 */
#include <cstdlib>
using namespace std;

bool plca2d(double* data, size_t lenf, size_t lent, size_t K, size_t max_iter, double sz, double** result_ptz, double** result_pfz,double** result_pz, size_t max_itertau=2);
#endif
