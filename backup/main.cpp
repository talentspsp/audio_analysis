#include <iostream>
#include "STFT.h"
#include <ctime>
#include <cstdlib>
#include <fstream>
#include "spuc/generic/complex.h"
#include "spuc/generic/spuc.h"
#include "spuc/generic/cplxfft.h"
//#include "spuc/matrix/matrix.h"
using namespace std;
using namespace SPUC;

#define BUFSIZE 100

int main (int argc, char* argv[])
{
  if(argc<3)
    {
      cout<<"input the data file name and out file name"<<endl;
      return 1;
    }
  ofstream datafd(argv[1]);
  if(!datafd)
    {
      cerr<<"can't open file "<<argv[1]<<endl;
      return -1;
    }
  ofstream outfd(argv[2]);
  if(!outfd)
    {
      cerr<<"can't open file "<<argv[2]<<endl;
      return -1;
    }
  srand((unsigned)time(0));
  int i,lent,lenf,j;
  complex<double>* data=new complex<double>[BUFSIZE];
  cout<<"data:"<<endl;
  for(i=0;i<BUFSIZE;i++)
    {
      data[i].re=rand()%100;
      data[i].im=0;
      cout<<data[i].re<<"+i"<<data[i].im<<endl;
      datafd<<data[i].re;
      if(data[i].im>=0)
	datafd<<"+";
      datafd<<data[i].im<<"i"<<endl;
    }
  complex<double>* result=STFT(data, BUFSIZE, 16, 16,5,lenf,lent);
  cout<<"result:"<<endl;
  for(i=0;i<lent;i++)
    {
      for(j=0;j<lenf;j++)
	{
	  cout<<result[i*lenf+j].re<<"+i"<<result[i*lenf+j].im<<"\t";
	  outfd<<result[i*lenf+j].re;
	  if(result[i*lenf+j].im>=0)
	    outfd<<"+";
	  outfd<<result[i*lenf+j].im<<"i"<<"\t";
	}
      cout<<endl;
      outfd<<endl;
    }
  delete []result;
  datafd.close();
  outfd.close();
  
  //test fft
  cfft< complex<double> > fftobj(16);
  fftobj.fft(data);
  ofstream fftoutfd("fftout");
  cout<<"FFT output: "<<endl;
  for(i=0;i<BUFSIZE;i++)
    {
      cout<<data[i].re<<"+i"<<data[i].im<<endl;
      fftoutfd<<data[i].re;
      if(data[i].im>=0)
	fftoutfd<<"+";
      fftoutfd<<data[i].im<<"i"<<endl;
    }
  fftoutfd.close();
  return 0;
  
}
