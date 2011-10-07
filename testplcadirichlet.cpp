#include <iostream>
#include "plcadirichlet.h"
#include "matfile.h"
#include "fastmath.h"
#include <ctime>
using namespace std;

int main()
{
  size_t lent=15;
  size_t lenf=10;
  int i,j;
 
  
 
  /*FMmatrix<double> x(lenf,lent);
    x.randset();*/

  double* in_data=bin2mat("testdata/80s_STFT.bin",(size_t*)&lent,(size_t*)&lenf);
  FMmatrix<double> x;
  x.setdata_ncpy(lent,lenf,in_data);
  x=x.transp(); 
  cout<<"x row is "<<x.numrow()<<" , col is "<<x.numcol()<<endl;
  int K=20;
  int iter=100;
  FMmatrix<double> in_sz(1,1,0.2);
  FMmatrix<double> in_sw(1,1,0);
  FMmatrix<double> in_sh(1,1,0);
  FMmatrix<double> in_z; //empty
  FMmatrix<double> in_h; //empty
  FMmatrix<double> in_w; //empty
  FMmatrix<bool> lw(1,K,false);
  FMmatrix<bool> lh(1,K,true);
  FMmatrix<double> out_w;
  FMmatrix<double> out_h;
  FMmatrix<double> out_z;
  
  clock_t start,finish;
  double timeused;
  start=clock();
  plcadirichlet(x,K,iter, in_sz, in_sw, in_sh, in_z, in_w, in_h,lw, lh, out_w,out_h,out_z,10);
  finish=clock();
  timeused=(double)(finish-start)/CLOCKS_PER_SEC;
  cout<<"Time used: "<<timeused<<"s"<<endl;
  
  //save result to file for analysis
  mat2asc("plca_data", x.numrow(),x.numcol(),x.getdata());
  mat2asc("plca_w", out_w.numrow(), out_w.numcol(), out_w.getdata());
  mat2asc("plca_h", out_h.numrow(), out_h.numcol(), out_h.getdata());
  mat2asc("plca_z", out_z.numrow(), out_z.numcol(), out_z.getdata());
  return 0;
}
