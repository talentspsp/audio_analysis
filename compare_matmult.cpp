#include <iostream>
#include <cstdlib>
#include <ctime>
#include "fastmath.h"
#include "matfile.h"
#include "CBLAS/include/cblas.h"
//#include "/Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/Accelerate.framework/Versions/Current/Frameworks/vecLib.framework/Versions/A/Headers/cblas.h"
using namespace std;

void mat_mult(double *C, double *A, double *B, int hA, int wA, int wB)
{
  int m,n,k;
  double sum;
  int hB=wA;
  //T* temp=new T[hA*wB];
 
    
  for (m=0; m<hA; m++) {
    for (n=0; n<wB; n++) {
      sum=0;
      for (k=0; k<wA; k++) {
	sum+=A[m*wA+k]*B[k*wB+n];
      }
      C[m*wB+n]=sum;
    }
  }
}


int main()
{
  srand(time(0));
  int hA=3;
  int wA=2;
  int hB=4;
  int wB=2;
  int hC=hA;
  int wC=hB;

  double* A=new double[hA*wA];
  double* B=new double[hB*wB];
  double* C=new double[hC*wC];
  double* D=new double[hC*wC];
  int m,n;
  for(m=0;m<hA;m++)
    for(n=0;n<wA;n++)
      A[m*wA+n]=rand()%5;
  for(m=0;m<hB;m++)
    for(n=0;n<wB;n++)
      B[m*wB+n]=rand()%5;
  clock_t start, finish;
  double tused;
  /* start=clock();
  mat_mult(C, A, B, hA, wA, wB);
  finish=clock();
  tused=(double)(finish-start)/CLOCKS_PER_SEC;
  cout<<"time for naive: "<<tused<<"s"<<endl;
  
  cout<<"result is "<<endl;
  printmat(C, hC, wC);*/

  FMmatrix<double> FA,FB,FC;
  FA.setdata_ncpy(hA,wA,A);
  FB.setdata_ncpy(hB,wB,B);
  
  start=clock();
  mat_mult(FC, FA, FB.transp());
  finish=clock();
  tused=(double)(finish-start)/CLOCKS_PER_SEC;
  cout<<"time for fastmath: "<<tused<<"s"<<endl;

  //mat2bin("A.bin", FA.numrow(), FA.numcol(),FA.getdata());
  //mat2bin("B.bin", FB.numrow(), FB.numcol(),FB.getdata());
  //mat2bin("C.bin", FC.numrow(), FC.numcol(),FC.getdata());

  cout<<"FA is"<<endl;
  FA.printmat();
  cout<<"FB is"<<endl;
  FB.printmat();
  cout<<"result is "<<endl;
  FC.printmat();

  start=clock();
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, hA, hB, wA, 1.0, A, wA, B, wB, 0.0,D, wC);
  finish=clock();
  tused=(double)(finish-start)/CLOCKS_PER_SEC;
  cout<<"time for blas: "<<tused<<"s"<<endl;
  
  cout<<"result is "<<endl;
  printmat(D, hC, wC);
  return 0;
  
}
