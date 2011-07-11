#include "fastmath.h"
#include <iostream>
#include "matfile.h"
#include <cstdlib>
#include <ctime>
using namespace std;

int main()
{
  /*int rowA=3,colA=4, rowB=4,colB=5;
  FMmatrix<double> A(rowA,colA);
  int i,j;
  for(i=0;i<rowA;i++)
    {
      for(j=0;j<colA;j++)
	{
	  A.setval(i,j,(double)(i+j)/2);
	  cout<<"A[i,j] is "<<A.getval(i,j)<<'\t';
	}
      cout<<endl;
    }
  cout<<"number of rows of A is "<<A.getrow()<<endl;
  cout<<"number of col of A is "<<A.getcol()<<endl;

  FMmatrix<double> B(rowB,colB,0.1);
  cout<<"the value in B is "<<B.getval(1,2)<<endl;
  B.setval(1,2,0);
  cout<<"B[1,2] now is "<<B.getval(1,2)<<endl;
  B.avoidzero();
  cout<<"B[1,2] now is "<<B.getval(1,2)<<endl;
  
  FMmatrix<double> C=B;
  cout<<"number of rows of C is "<<C.getrow()<<endl;
  cout<<"number of cols of C is "<<C.getcol()<<endl;
  cout<<"C[1,2] is "<<C.getval(1,2)<<endl;
  B.setval(3,4,2);
  C=B;
  cout<<"C[3,4] is "<<C.getval(3,4)<<endl;

  cout<<"start testing the operations"<<endl;
  cout<<"matrix A is"<<endl;
  A.printmat();
  cout<<"matrix B is "<<endl;
  B.printmat();
  cout<<"matrix C is "<<endl;
  C.printmat();

  scalar_mult_mat(0.3, B, B);
  cout<<"B=B * 0.3, B is "<<endl;
  B.printmat();
  FMmatrix<double> sumBC,BdpC,BddC;
  mat_add(sumBC, B, C);
  cout<<"B+C="<<endl;
  sumBC.printmat();
  ew_mult(BdpC, B, C);
  cout<<"B.*C="<<endl;
  BdpC.printmat();
  ew_div(BddC, B,C);
  cout<<"B./C="<<endl;
  BddC.printmat();

  cout<<endl<<endl;
  cout<<"start testing statistic operations"<<endl;
  FMmatrix<double> D(3,4);
  FMmatrix<double> ret;
  for(i=0;i<3;i++)
    for(j=0;j<4;j++)
      D.setval(i,j,rand()%5);
  cout<<"matrix D is "<<endl;
  D.printmat();
  ret=mean(D,1);
  cout<<"mean along 1st dim is "<<endl;
  ret.printmat();
  ret=mean(D,2);
  cout<<"mean along 2nd dim is "<<endl;
  ret.printmat();
  ret=mean(D,0);
  cout<<"mean of the whole mat is "<<endl;
  ret.printmat();
  int indr[4],indc[3];
  ret=min(D, 1, indr);
  cout<<"min along 1st dim is "<<endl;
  ret.printmat();
  cout<<"indices are "<<endl;
  for(i=0;i<4;i++)
    cout<<indr[i]<<'\t';
  cout<<endl;
  ret=min(D,2,indc);
  cout<<"min along 2nd dim is "<<endl;
  ret.printmat();
  cout<<"indices are "<<endl;
  for(i=0;i<3;i++)
    cout<<indc[i]<<'\t';
  cout<<endl;
  ret=min(D,0,indr);
  cout<<"min of the whole mat is "<<endl;
  ret.printmat();
  cout<<"the index is "<<endl;
  cout<<indr[0]<<endl;
  
  ret=max(D, 1, indr);
  cout<<"max along 1st dim is "<<endl;
  ret.printmat();
  cout<<"indices are "<<endl;
  for(i=0;i<4;i++)
    cout<<indr[i]<<'\t';
  cout<<endl;
  ret=max(D,2,indc);
  cout<<"max along 2nd dim is "<<endl;
  ret.printmat();
  cout<<"indices are "<<endl;
  for(i=0;i<3;i++)
    cout<<indc[i]<<'\t';
  cout<<endl;
  ret=max(D,0,indr);
  cout<<"max of the whole mat is "<<endl;
  ret.printmat();
  cout<<"the index is "<<endl;
  cout<<indr[0]<<endl;

  cout<<"matrix D is "<<endl;
  D.printmat();

  ret=norm2(D,1);
  cout<<"norm2 along 1st dim is "<<endl;
  ret.printmat();
  ret=norm2(D,2);
  cout<<"norm2 along 2nd dim is "<<endl;
  ret.printmat();
  ret=norm2(D,0);
  cout<<"norm2 of the whole mat is "<<endl;
  ret.printmat();*/
  srand(time(0));
  FMmatrix<double> A(300,400);
  FMmatrix<double> B(400,500);
  FMmatrix<double> C;

  //initilize
  int i,j;
  for(i=0;i<300;i++)
    for(j=0;j<400;j++)
      A.setval(i,j,rand()%20);
  for(i=0;i<400;i++)
    for(j=0;j<500;j++)
      B.setval(i,j,rand()%15);
  mat_mult(C, A, B);
  mat2asc("A.txt", A.getrow(), A.getcol(),A.getdata());
  mat2asc("B.txt", B.getrow(), B.getcol(),B.getdata());
  mat2asc("C.txt", C.getrow(), C.getcol(),C.getdata());
  return 0;
  
  
}
