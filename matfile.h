#ifndef _MATFILE_H
#define _MATFILE_H


/* 
   write the 2D matrix in data to a file in ascii. if the matrix is 2D, dim3 is 1
   file format is:
   row
   col
   (empty line)
   data in 2D

 */
#include <cstdlib>
#include <iostream>
#include "fastmath.h"
using namespace std;

template <class T>
void printmat(T* mat, size_t row, size_t col)
{
  for(size_t i=0;i<row;i++)
    {
      for(size_t j=0;j<col;j++)
	cout<<mat[i*col+j]<<"\t";
      cout<<endl;
    }
}

bool mat2asc(const char* filename, size_t row, size_t col,double* data);
double* asc2mat(const char* filename, size_t* row, size_t* col);

/*
  save matrix to binary file, format
  row col data....
 */
bool mat2bin(const char* filename, size_t row, size_t col, double* data);
double* bin2mat(const char* filename, size_t* row, size_t* col);

bool mat2bin(const char* filename, const FMmatrix<double>& T);

#endif
