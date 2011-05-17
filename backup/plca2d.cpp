#include "plca2d.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include "math_util.h"
#include "LambertW1.h"
using namespace std;

bool plca2d(double* data, size_t lenf, size_t lent, size_t K, size_t max_iter, double sz, double** result_ptz, double** result_pfz, size_t max_itertau)
{
  //check input
  if(lenf==0 || lent==0 || K==0 || max_iter==0)
    {
      cerr<<"lenf, lent, K, maxiter must be positive"<<endl;
      return false;
    }
  if(sz<0)
    {
      cerr<<"sz can't be negative!"<<endl;
      return false;
    }
  size_t t,f,z;
  double* ptzf=new double[K*lenf*lent]; //z row, f column, t 3D
  double* ptz=new double[lent*K];//t row, z column
  double* pfz=new double[K*lenf];//z row, f column
  double* oz=new double[lent*z]; //omega_z, t row, z column
  double* tautz=new double[lent*z]; //tau_t, leave the z dimemsion for possible future use
  //random initialize, avoid zero
  double sum_z,sum_f; //sum over z, sum over f
  //initialize ptz
  for(t=0;t<lent;t++)
    {
      sum_z=0;
      for(z=0;z<K;z++)
	{
	  ptz[t*K+z]=rand()%1000+1;
	  sum_z+=ptz[t*K+z];
	}
      scalar_mult_mat(ptz+t*K, ptz+t*K, 1, K, 1/sum_z);
    }
  //initize pfz
  for(z=0;z<K;z++)
    {
      sum_f=0;
      for(f=0;f<lenf;f++)
	{
	  pfz[z*lenf+f]=rand()%1000+1;
	  sum_f+=pfz[z*lenf+f];
	}
      scalar_mult_mat(pfz+z*lenf,pfz+z*lenf,1,lenf,1/sum_f);
    }
  size_t iter=0, itertau;
  double* tmp_lenf=new double[lenf];
  double tt,wp; //taut, and parameter for lambertW function
  double tv1; //temp variable
  while(iter<max_iter)
    {
      //ptz*pfz
      for(t=0;t<lent;t++)
	{
	  setzero(tmp_lenf,lenf);
	  for(z=0;z<K;z++)
	    {
	      scalar_mult_mat(ptzf+t*lenf*K+z*lenf,pfz+z*lenf,1,lenf,ptz[t*K+z]);
	      mat_add(tmp_lenf,tmp_lenf,ptzf+t*lenf*K+z*lenf,1,lenf);
	    }
	  //ptz*pfz/sum(ptz*pfz)
	  avoidzero(tmp_lenf,lenf);
	  for(z=0;z<K;z++)
	    {
	      ew_div(ptzf+t*lenf*K+z*lenf,ptzf+t*lenf*K+z*lenf,tmp_lenf,1,lenf);
	    }
	}
      //sum_t Pt(z|f)*Lft
      setzero(pfz,K,lenf);
      for(t=0;t<lent;t++)
	{
	  for(z=0;z<K;z++)
	    ew_mult(ptzf+t*K*lenf+z*lenf,ptzf+t*K*lenf+z*lenf,data+t*lenf,1,lenf); //Notice: ptzf here has been changed to ptzf*Lft
	  mat_add(pfz,pfz,ptzf+t*K*lenf,K,lenf);
	}
      //Ps(f|z)=sum_t Pt(z|f)*Lft / sum_f sum_t Pt(z|f) Lft
      for(z=0;z<K;z++)
	{
	  sum_f=0;
	  for(f=0;f<lenf;f++)
	    sum_f+=pfz[z*lenf+f];
	  //avoid zero
	   sum_f=(sum_f<MIN_DOUBLE)?MIN_DOUBLE:sum_f;
	   scalar_mult_mat(pfz+z*lenf,pfz+z*lenf,1,lenf,1/sum_f);
	}
      //omega_z=sum_f Lft*Pt(z|f), note: ptzf above has been chaged to ptzf*Lft
      setzero(oz,lent,K);
      for(t=0;t<lent;t++)
	for(z=0;z<K;z++)
	  for(f=0;f<lenf;f++)
	    oz[t*K+z]=ptzf[t*K*lenf+z*lenf+f];
      
      for(itertau=0;itertau<max_itertau;itertau++)
	{
	  avoidzero(ptz,lent*K);
	  //tau_t=-omega_z/ptz-sz-sz*log(ptz)
	  ew_div(tautz,oz,ptz,lent,K);//tau_t=omega_z/ptz
	  for(t=0;t<lent;t++)
	    for(z=0;z<K;z++)
	      ptz[t*K+z]=sz+sz*log(ptz[t*K+z]);
	  mat_add(tautz, tautz, ptz, lent, K);
	  scalar_mult_mat(tautz,tautz,lent,K,-1);
	
	  //Pt(z)=-omega_z/sz  /  W(-omega_z*exp(1+taut/sz)/sz)
	  for(t=0;t<lent;t++)
	    {
	      tt=mean(tautz+t*K,K);
	      for(z=0;z<K;z++)
		{
		  tv1=-oz[t*K+z]/sz;
		  wp=tv1*exp(1+tt/sz);
		  ptz[t*K+z]=tv1/LambertW1(wp); //add protection to LamberW1, if fail, it will return 1
		}
	    }
	}
      iter++;
    }
}
