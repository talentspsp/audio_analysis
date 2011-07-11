#ifndef _FASTMATH_H
#define _FASTMATH_H
#include <iostream>
#define MIN_DOUBLE 1e-250
template <class T> class FMmatrix;

template <class T>
inline void scalar_mult_mat(T sc, const FMmatrix<T>& mat, FMmatrix<T>& result);

template <class T>
inline void mat_add(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B); //result can be same as A or B

template <class T>
inline void ew_mult(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B); //element-wise multiply, result can be same as A or B

template <class T>
inline void ew_div(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B);

template <class T>
inline FMmatrix<T> mean(const FMmatrix<T>& A, int dim); //dim: 0, mean of all the elements; 1, mean along the first dimemsion, return a row vector; 2, mean along the second dimension, return a column vector
template <class T>
inline FMmatrix<T> min(const FMmatrix<T>& A, int dim, int* ind=0); //dim: 0, min of all the elements; 1, min along the first dimemsion, return a row vector; 2, min along the second dimension, return a column vector, ind is an array to save the indices, it should already be allocated

template <class T>
inline FMmatrix<T> max(const FMmatrix<T>& A, int dim, int* ind=0);//dim: 0, norm2 of all the elements; 1, norm2 along the first dimemsion, return a row vector; 2, norm2 along the second dimension, return a column vector. norm2 is the sqrt of sum of square of each element

template <class T>
inline FMmatrix<T> norm2(const FMmatrix<T>& A, int dim); //norm2 along dim.

template <class T>
void mat_mult(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B, int crossover=128); //crossover is the point after which to do basic matrix multiplication

template <class T>
class FMmatrix
{
public:
  FMmatrix(): row(0), col(0) {data=0;}
  FMmatrix(int in_row, int in_col): row(in_row), col(in_col) {data=new T[row*col];}
  inline FMmatrix(int in_row, int in_col, T val);
  inline FMmatrix(const FMmatrix& rhs); //deep copy
  inline FMmatrix<T>& operator= (const FMmatrix& rhs); //deep copy
  inline void init(int in_row, int in_col); //init is used to allocate space, which can only be used by object created by the constructor with no argument
  inline T getval(int r, int c);
  inline void setval(int r, int c, T val);
  int getrow() {return row;}
  int getcol() {return col;}
  T* getdata() {return data;}
  inline void avoidzero();
  void printmat(const std::ostream& ou=std::cout);

  friend void scalar_mult_mat<T>(T sc, const FMmatrix<T>& mat, FMmatrix<T>& result);
  friend void mat_add<T>(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B);
  friend void ew_mult<T>(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B);
  friend void ew_div<T>(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B);
  friend void mat_mult<T>(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B);
  friend FMmatrix<T> mean<T>(const FMmatrix<T>& A, int dim);
  friend FMmatrix<T> min<T>(const FMmatrix<T>& A, int dim, int* ind);
  friend FMmatrix<T> max<T>(const FMmatrix<T>& A, int dim, int* ind);
  friend FMmatrix<T> norm2<T>(const FMmatrix<T>& A, int dim);
  ~FMmatrix() {delete []data;}
private:
  int row;
  int col;
  T* data;
};

#include "fastmath.cpp"
#endif
