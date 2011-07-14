#ifndef _PLCA_H
#define _PLCA_H
#include "fastmath.h" //it's still not fast enough, will use lapack instead of it

/*
  This function implement plca algorithm
  Input:
  x: a M*N 2d distribution. each elememt must be nonnegative, each column is distribution in frequency domain
  K: number of components
  iter: max number of iteration
  in_sz: sparsity of z. it should only be 1*1 matrix or iter*K matrix
  in_sw: sparsity of w. it should only be 1*1 matrix or iter*K matrix
  in_sh: sparsity of h. it should only be 1*1 matrix or iter*K matrix
  in_z: initial value of p(z). it should only be an empty matrix or a 1*K matrix
  in_w: initial value of p(w). it should only be an empty matrix, or a M row matrix with column less or equal than K
  in_h: initial value of p(h). it should only be an empty matrix, or a N column matrix with column less or equal than K
  lw: a 1*K matrix. lw(i)=true indicates to update this column
  lh: a 1*K matrix. lh(i)=true indicates to update this row
  out_*: used for returning
 */



void plca(const FMmatrix<double>& x, int K, int iter, const FMmatrix<double>& in_sz, const FMmatrix<double>& in_sw, const FMmatrix<double>& in_sh, const FMmatrix<double>& in_z, const FMmatrix<double>& in_w, const FMmatrix<double>& in_h, const FMmatrix<bool>& lw, const FMmatrix<bool>& lh, FMmatrix<double>& out_w,  FMmatrix<double>& out_h,  FMmatrix<double>& out_z);


#endif
