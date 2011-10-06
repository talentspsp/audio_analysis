#include "plca.h"
#include "fastmath.h"
using namespace std;

/*
  this function is used to call plca
 */
bool plca2d(double* data, size_t lenf, size_t lent, size_t K, size_t max_iter, double sz, double** result_ptz, double** result_pfz,double** result_pz, size_t max_itertau)
{
  FMmatrix<double> x;
  x.setdata_cpy(lent, lenf,data);
  x=x.transp();
  FMmatrix<double> in_sz(1,1,sz);
  FMmatrix<double> in_sw(1,1,0);
  FMmatrix<double> in_sh(1,1,0);
  FMmatrix<double> in_z; //empty
  FMmatrix<double> in_h; //empty
  FMmatrix<double> in_w; //empty
  FMmatrix<bool> lw(1,K,true);
  FMmatrix<bool> lh(1,K,true);
  FMmatrix<double> out_w;
  FMmatrix<double> out_h;
  FMmatrix<double> out_z;
  plca(x,K,max_iter, in_sz, in_sw, in_sh, in_z,in_w, in_h, lw, lh, out_w,  out_h, out_z);
  double* ptz=new double[lent*K];//t row, z column
  double* pfz=new double[K*lenf];//z row, f column
  double* pz=new double[K];
  int i,j;
  for(i=0;i<lent;i++)
    for(j=0;j<K;j++)
	ptz[i*K+j]=out_h(j,i);
  for(i=0;i<K;i++)
    for(j=0;j<lenf;j++)
      pfz[i*lenf+j]=out_w(j,i);
  
  for(i=0;i<K;i++)
    pz[i]=out_z(i);

  *result_pfz=pfz;
  *result_ptz=ptz;
  *result_pz=pz;
  return true;
}
