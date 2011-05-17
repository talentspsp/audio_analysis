/*
 *  math_util.h
 *  Created by Qingyuan Kong on 03/07/11.
 *  Copyright 2011 Dartmouth. All rights reserved.
 *
 */
#ifndef _MATH_UTIL_H
#define _MATH_UTIL_H
#include <cstdlib>
using namespace std;

#define MIN_DOUBLE 1e-250
void MatrixMultiply(double *C, double *A, double *B, size_t hA, size_t wA, size_t wB, size_t crossover=128);
double* transpose(double *A, size_t m, size_t n);

template <class T>
inline void scalar_mult_mat(T* result, T* mat, size_t m, size_t n, T sc) //result can be same as vec
{
  for(size_t i=0;i<m;i++)
    for(size_t j=0;j<n;j++)
      result[i*n+j]=mat[i*n+j]*sc;
}

template <class T>
inline void mat_add(T* result, T* A, T* B, size_t m, size_t n) //result can be same as A or B
{
  for(size_t i=0;i<m;i++)
    for(size_t j=0;j<n;j++)
      result[i*n+j]=A[i*n+j]+B[i*n+j];
}

template <class T>
inline void ew_mult(T* result, T* A, T* B, size_t m, size_t n) //element-wise multiply, result can be same as A or B
{
  for(size_t i=0;i<m;i++)
    for(size_t j=0;j<n;j++)
      result[i*n+j]=A[i*n+j]*B[i*n+j];
}

template <class T>
inline void ew_div(T* result, T* A, T* B, size_t m, size_t n) //element-wise divide, result can be same as A or B
{
  for(size_t i=0;i<m;i++)
    for(size_t j=0;j<n;j++)
      result[i*n+j]=A[i*n+j]/B[i*n+j];
}

template <class T>
inline T mean(T* array, size_t sz)
{
  T sum=0;
  for(size_t i=0;i<sz;i++)
    sum+=arrayp[i];
  return sum/sz;
}

inline void setzero(double* p, size_t sz)
{
  for(size_t i=0;i<sz;i++)
    p[i]=0;
}

inline void avoidzero(double* p, size_t sz)
{
  for(size_t i=0;i<sz;i++)
    p[i]=(p[i]<MIN_DOUBLE)?MIN_DOUBLE:p[i];
}
		
#endif
