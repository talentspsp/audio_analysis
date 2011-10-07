#ifndef _PLCA_H
#define _PLCA_H
#include "fastmath.h"

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

/*
  plca_BLAS is similar to plca, but it uses BLAS routines to do the matrix multiplication.
  *************** Notice *****************
  1 To achieve the optimal speed, you should use the BLAS library shipped with your machine.
    To do this:
    1) you need to change 
    #include "CBLAS/include/cblas.h"
    in plca_BLAS.cpp to your blas header file.
    2) you need to change BLASLIB to the native blas library you want to link against, e.g. on Mac OSX, it can be "-framework veclib". Change BLASHEADER to the path to the header file you used.

  2 If you don't have a native blas library, then you need to install BLAS and CBLAS. You just need to go to these two directories and type "make all". And you can use the configuration for BLASLIB and BLASHEADER in the provided Makefile. You can type "make testnewplca_BLAS" to see how to compile it. Notice: if you use the non-native BLAS library, plca_BLAS is not guaranteed to be faster than plca.
 */
void plca_BLAS(const FMmatrix<double>& x, int K, int iter, const FMmatrix<double>& in_sz, const FMmatrix<double>& in_sw, const FMmatrix<double>& in_sh, const FMmatrix<double>& in_z, const FMmatrix<double>& in_w, const FMmatrix<double>& in_h, const FMmatrix<bool>& lw, const FMmatrix<bool>& lh, FMmatrix<double>& out_w,  FMmatrix<double>& out_h,  FMmatrix<double>& out_z);


#endif
