#include "plca2d.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include "math_util.h"
#include "LambertWs.h"
#include "LambertWnew.h"
using namespace std;
#define TOLERANT -600  //this the minimal base e expotional number for Lambert function, below this number should use lambert_arg_outof_range
#define MINEX -1000


bool plca2d(double* data, size_t lenf, size_t lent, size_t K, size_t max_iter, double sz, double** result_ptz, double** result_pfz,double** result_pz, size_t max_itertau)
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
  double* pz=new double[K];
  double* oz=new double[lent*K]; //omega_z, t row, z column
  double* tautz=new double[lent*K]; //tau_t, leave the z dimemsion for possible future use
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
  double* pt=new double[lent];
  double* tmp_K=new double[K];
  double tt,wp; //taut, and parameter for lambertW function
  double tv1,tv2; //temp variable
  double en; //expotional number
  while(iter<max_iter)
    {
#ifdef TEST
      cout<<"iter is "<<iter<<endl;
#endif
      //ptz*pfz
      for(t=0;t<lent;t++)
	{
	  setzero(tmp_lenf,lenf);
	  //memset(tmp_lenf,0,lenf*sizeof(double));

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
#ifdef TEST
      cout<<"ptzf is "<<endl;
      for(t=0;t<lent;t++)
	{
	  for(z=0;z<K;z++)
	    {
	      for(f=0;f<lenf;f++)
		cout<<ptzf[t*lenf*K+z*lenf+f]<<" ";
	      cout<<endl;
	    }
	  cout<<endl;
	}
#endif
      //sum_t Pt(z|f)*Lft
      setzero(pfz,K*lenf);
      // memset(pfz,0,K*lenf*sizeof(double));

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
      setzero(oz,lent*K);
      //memset(oz,0,lent*K*sizeof(double));

      for(t=0;t<lent;t++)
	for(z=0;z<K;z++)
	  for(f=0;f<lenf;f++)
	    oz[t*K+z]+=ptzf[t*K*lenf+z*lenf+f];
#ifdef TEST
      cout<<"first oz is "<<endl;
      for(t=0;t<lent;t++)
	{
	  for(z=0;z<K;z++)
	    cout<<oz[t*K+z]<<" ";
	  cout<<endl;
	}
#endif
      avoidzero(oz,lent*K);
      if(sz==0)
	{
	  for(t=0;t<lent;t++)
	    {
	      sum_z=0;
	      for(z=0;z<K;z++)
		sum_z+=oz[t*K+z];
	      scalar_mult_mat(ptz+t*K,oz+t*K,1,K,1/sum_z);
	    }
	}
      else
	{
	  for(itertau=0;itertau<max_itertau;itertau++)
	    {
#ifdef TEST
	      cout<<"itertau is "<<itertau<<endl;
#endif
	      avoidzero(ptz,lent*K);
	      //tau_t=-omega_z/ptz-sz-sz*log(ptz)
	      ew_div(tautz,oz,ptz,lent,K);//tau_t=omega_z/ptz
#ifdef TEST
	      cout<<"tautz is "<<endl;
	      for(t=0;t<lent;t++)
		{
		  for(z=0;z<K;z++)
		    cout<<tautz[t*K+z]<<" ";
		  cout<<endl;
		}
	      cout<<"oz is "<<endl;
	      for(t=0;t<lent;t++)
		{
		  for(z=0;z<K;z++)
		    cout<<oz[t*K+z]<<" ";
		  cout<<endl;
		}
#endif
	      for(t=0;t<lent;t++)
		for(z=0;z<K;z++)
		  ptz[t*K+z]=sz+sz*log(ptz[t*K+z]);
	      mat_add(tautz, tautz, ptz, lent, K);
	      scalar_mult_mat(tautz,tautz,lent,K,-1.0);
	
	      //Pt(z)=-omega_z/sz  /  W(-omega_z*exp(1+taut/sz)/sz)
	      for(t=0;t<lent;t++)
		{
		  sum_z=0;
		  for(z=0;z<K;z++)
		    {
		      if(itertau==0)
			tt=min(tautz+t*K,K);
		      else
			tt=tautz[t*K+z];
		      tv1=-oz[t*K+z]/sz;
		      wp=tv1*exp(1+tt/sz);
		      en=log(-tv1)+1+tt/sz;
		      en=(en<MINEX)?MINEX:en;
#ifdef TEST
		      cout<<"arg expotional number for lambert is "<<en<<" and tt is "<<tt<<" and wp is "<<wp<<endl;
#endif

		      if(en>TOLERANT)
			{
			  ptz[t*K+z]=tv1/LambertW1(wp); //TODO: add protection to LamberW1, if fail, it will return 1
			  //ptz[t*K+z]=tv1/lambert_Wm1(wp);
#ifdef TEST
			  tv2=LambertW1(wp);
#endif
			}
		      else
			{
			  ptz[t*K+z]=tv1/lambert_arg_outof_range(en);
#ifdef TEST
			  tv2=lambert_arg_outof_range(en);
#endif
			}
#ifdef TEST
		      cout<<"w is "<<tv2<<endl;
#endif
		      sum_z+=ptz[t*K+z];
		    }
		  scalar_mult_mat(ptz+t*K,ptz+t*K,1,K,1/sum_z);
#ifdef TEST
		  cout<<"t is "<<t<<endl;
		  cout<<"Ptz is "<<endl;
		  for(z=0;z<K;z++)
		    cout<<ptz[t*K+z]<<" ";
		  cout<<endl;
		
#endif
		}
	    }
	}
      iter++;
    }
  //compute P(t)=sum_f P(f,t)
  setzero(pt,lent);
  //memset(pt,0,lent*sizeof(double));

  for(t=0;t<lent;t++)
    for(f=0;f<lenf;f++)
      pt[t]+=data[t*lenf+f];
  //P(z)=sum_t P(z|t)*P(t)
  setzero(pz,K);
  //memset(pt,0,K*sizeof(double));

  for(t=0;t<lent;t++)
    {
      scalar_mult_mat(tmp_K,ptz+t*K,1,K,pt[t]);
      mat_add(pz, pz, tmp_K, 1, K);
    }
  

  *result_ptz=ptz;
  *result_pfz=pfz;
  *result_pz=pz;
  
  delete []ptzf;
  delete []oz;
  delete []tautz;
  delete []tmp_lenf;
  delete []pt;
  delete []tmp_K;
  return true;
}
