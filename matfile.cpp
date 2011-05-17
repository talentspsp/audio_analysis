#include <cstdlib>
#include "matfile.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

/*
  write matrix to an ascii text file
  format:
  row
  col

  data
 */

bool mat2asc(const char* filename, size_t row, size_t col, double* data)
{
  ofstream outfile(filename);
  if(!outfile)
    {
      cerr<<"can't open file "<<filename<<endl;
      return false;
    }
  if(row*col<1)
    {
      cerr<<"each dim must be greater than zero!"<<endl;
      return false;
    }
  outfile<<row<<endl;
  outfile<<col<<endl;
  size_t i,j;
  outfile<<endl;
  for(i=0;i<row;i++)
    {
      for(j=0;j<col;j++)
	outfile<<data[i*col+j]<<" ";
      outfile<<endl;
    }
  outfile.close();
  return true;
}

double* asc2mat(const char* filename, size_t* row, size_t* col)
{
  ifstream in(filename);
  if(!in)
    {
      cerr<<"cannot open file "<<filename<<endl;
      return NULL;
    }
  string line;
  istringstream istr;
  size_t r,c,i,k;
  in>>r;
  in>>c;
  *row=r;
  *col=c;
  double* data=new double[r*c];
  i=0;
  getline(in,line); //newline char
  getline(in,line); //empty line
  while(getline(in,line))
    {
      istr.str(line);
      for(k=0;k<c;k++)
	{
	  istr>>data[i];
	  i++;
	}
    }

  in.close();
  return data;
}

bool mat2bin(const char* filename, size_t row, size_t col, double* data)
{
  ofstream ou(filename, ios::out | ios::binary);
  if(!ou)
    {
      cerr<<"cannot open file "<<filename<<endl;
      return false;
    }
  ou.write((char*)&row, sizeof(size_t));
  ou.write((char*)&col, sizeof(size_t));
  ou.write((char*)data,row*col*sizeof(size_t));
  ou.close();
  return true;
}

double* bin2mat(const char* filename, size_t* row, size_t* col)
{
  ifstream in(filename, ios::in | ios::binary);
  if(!in)
    {
      cerr<<"cannot open file "<<filename<<endl;
      return NULL;
    }
  in.read((char*)row,sizeof(size_t));
  in.read((char*)col,sizeof(size_t));
  double* data=new double[(*row)*(*col)];
  in.read((char*)data,(*row)*(*col)*sizeof(size_t));
  in.close();
  return data;
}
