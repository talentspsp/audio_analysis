#include <iostream>
#include "matfile.h"
#include <cstdlib>
#include <ctime>
using namespace std;

int main()
{
  srand(NULL);
  //generate random mat
  size_t row=3,col=4,i,j;
  double d;
  double* data_out=new double[row*col];
  for(i=0;i<row*col;i++)
    {
      d=rand()%100;
      d/=rand()+1;
      data_out[i]=d;
    }
  cout<<"orginal data is "<<endl;
  for(i=0;i<row;i++)
    {
      for(j=0;j<col;j++)
	cout<<data_out[i*col+j]<<"\t";
      cout<<endl;
    }
  mat2asc("testmat.ascii", row, col,data_out);
  mat2bin("testmat.bin", row, col, data_out);
  size_t row_in, col_in;
  double *data_asc,*data_bin;
  data_asc=asc2mat("testmat.ascii", &row_in, &col_in);
  cout<<"asc in is"<<endl;
  cout<<"row is "<<row_in<<" col is "<<col_in<<endl;
   for(i=0;i<row_in;i++)
    {
      for(j=0;j<col_in;j++)
	cout<<data_asc[i*col_in+j]<<"\t";
      cout<<endl;
    }

  data_bin=bin2mat("testmat.bin", &row_in, &col_in);
  cout<<"bin in is"<<endl;
  cout<<"row is "<<row_in<<" col is "<<col_in<<endl;
   for(i=0;i<row_in;i++)
    {
      for(j=0;j<col_in;j++)
	cout<<data_bin[i*col_in+j]<<"\t";
      cout<<endl;
    }
  return 0;
}

