#ifndef _FASTMATH_H
#define _FASTMATH_H
#include <iostream>
#define MIN_DOUBLE 1e-250

/*
  note: this class is not safe for some non-sense input. less protection for speed
 */
template <class T> class FMmatrix;

template <class T>
inline void scalar_mult_mat(FMmatrix<T>& result, T sc, const FMmatrix<T>& mat);

template <class T>
inline void scalar_add_mat(FMmatrix<T>& result, T sc, const FMmatrix<T>& mat);

template <class T>
inline void mat_add(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B); //result can be same as A or B

template <class T>
inline void ew_mult(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B); //element-wise multiply, result can be same as A or B

template <class T>
inline void ew_div(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B);

template <class T>
inline FMmatrix<T> mean(const FMmatrix<T>& A, int dim); //dim: 0, mean of all the elements; 1, mean along the first dimemsion, return a row vector; 2, mean along the second dimension, return a column vector

template <class T>
inline FMmatrix<T> sum(const FMmatrix<T>& A, int dim); //dim: 0, sum of all the elements; 1, sum along the first dimemsion, return a row vector; 2, mean along the second dimension, return a column vector

template <class T>
inline FMmatrix<T> min(const FMmatrix<T>& A, int dim, int* ind=0); //dim: 0, min of all the elements; 1, min along the first dimemsion, return a row vector; 2, min along the second dimension, return a column vector, ind is an array to save the indices, it should already be allocated

template <class T>
inline FMmatrix<T> max(const FMmatrix<T>& A, int dim, int* ind=0);//dim: 0, norm2 of all the elements; 1, norm2 along the first dimemsion, return a row vector; 2, norm2 along the second dimension, return a column vector. norm2 is the sqrt of sum of square of each element

template <class T>
inline FMmatrix<T> norm2(const FMmatrix<T>& A, int dim); //norm2 along dim.

template <class T>
void mat_mult(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B, int crossover=128); //crossover is the point after which to do basic matrix multiplication


/*
  matrix multiply vector element-wisely. the size of one dimension of B should be 1. if it's a row vector, it should have same columns as A; if it's a column vector, it shuold have same rows as A.
 */
template <class T>
inline void mat_ewmult_vec(FMmatrix<T>& result, const FMmatrix<T>& mat, const FMmatrix<T>& vec);

/*
  matrix divide vector element-wisely. the size of one dimension of B should be 1. if it's a row vector, it should have same columns as A; if it's a column vector, it shuold have same rows as A.
 */
template <class T>
inline void mat_ewdiv_vec(FMmatrix<T>& result, const FMmatrix<T>& mat, const FMmatrix<T>& vec);

template <class T>
inline void logmat(FMmatrix<T>& result, const FMmatrix<T>& mat);

template <class T>
inline void expmat(FMmatrix<T>& result, const FMmatrix<T>& mat);

template <class T>
class FMmatrix
{
public:
  FMmatrix(): row(0), col(0), data(0), capacity(0) {}
  FMmatrix(int in_row, int in_col): row(in_row), col(in_col), capacity(in_row*in_col) {data=new T[row*col];}
  inline FMmatrix(int in_row, int in_col, T val);
  inline FMmatrix(const FMmatrix& rhs); //deep copy
  inline FMmatrix<T>& operator= (const FMmatrix& rhs); //deep copy
  inline void init(int in_row, int in_col); //init is used to allocate space, which can only be used by object created by the constructor with no argument
  //inline T getval(int r, int c);
  //inline void setval(int r, int c, T val);
  inline T& operator()(int r, int c);
  inline const T& operator()(int r, int c) const;
  inline T& operator()(int ind);
  inline const T& operator()(int ind) const;
  int numrow() const {return row;}
  int numcol() const {return col;}
  int getcap() const {return capacity;}
  T* getdata() const {return data;}
  bool isempty() const {return (data==0);}
  int numel() const {return row*col;}
  inline FMmatrix<T> transp() const;
  inline void clear();
  inline void avoidzero();
  void printmat(const std::ostream& ou=std::cout) const;
  inline void reset(int in_row, int in_col); //delete original data, re-allocate space
  inline void reset(int in_row, int in_col, T val);//delete original data, re-allocate space, set all the element as val
  inline void randset(int M=1000); //random set all the values between 1~M
  inline FMmatrix<T> getrow(int rowind) const;
  inline FMmatrix<T> getcol(int colind) const;
  inline void setrow(int rowind, const FMmatrix<T>& val);
  inline void setcol(int colind, const FMmatrix<T>& val);
  inline void setdata_cpy(int in_row, int in_col, const T* in_data); //this function will copy the data in in_data to the object
  inline void setdata_ncpy(int in_row, int in_col, T* in_data); //this funciont will set this->data=in_data;
  inline void setdata_zero(); //this function will set this->data=0, without free the memory! so be careful when using it!

  friend void scalar_mult_mat<T>(FMmatrix<T>& result, T sc, const FMmatrix<T>& mat);
  friend void scalar_add_mat<T>(FMmatrix<T>& result, T sc, const FMmatrix<T>& mat);
  friend void mat_add<T>(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B);
  friend void ew_mult<T>(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B);
  friend void ew_div<T>(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B);
  friend void mat_mult<T>(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B, int crossover);
  friend void mat_ewmult_vec<T>(FMmatrix<T>& result, const FMmatrix<T>& mat, const FMmatrix<T>& vec);
  friend void mat_ewdiv_vec<T>(FMmatrix<T>& result, const FMmatrix<T>& mat, const FMmatrix<T>& vec);
  friend void logmat<T>(FMmatrix<T>& result, const FMmatrix<T>& mat);
  friend void expmat<T>(FMmatrix<T>& result, const FMmatrix<T>& mat);
  friend FMmatrix<T> mean<T>(const FMmatrix<T>& A, int dim);
  friend FMmatrix<T> sum<T>(const FMmatrix<T>& A, int dim);
  friend FMmatrix<T> min<T>(const FMmatrix<T>& A, int dim, int* ind);
  friend FMmatrix<T> max<T>(const FMmatrix<T>& A, int dim, int* ind);
  friend FMmatrix<T> norm2<T>(const FMmatrix<T>& A, int dim);
  
  
  ~FMmatrix() 
  {
    if(data != 0)
      delete []data;
  }
private:
  int row;
  int col;
  int capacity; //the capacity of the data block, if a bigger block is needed, then free the original block, and allocate new block, otherwise, just used the original block
  //so, row*col <= capacity, use capacity to reduce the free and allocation of memory
  T* data;
};

#include "fastmath.cpp"
#endif
