#include "lambert_compute_with_offset.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

FMmatrix<double> lambert_compute_with_offset(const FMmatrix<double>& in_omeg, double z, double lam_offset)
{
  if(in_omeg.numrow() != 1 && in_omeg.numcol() != 1)
    throw runtime_error("In lambert_compute_with_offset: omeg should be a vector!");
  if(z==0)
    throw runtime_error("In lambert_compute_with_offset: z can't be zero!");
  FMmatrix<double> omeg(in_omeg);
  omeg.avoidzero();
  FMmatrix<double> oz;
  scalar_mult_mat(oz, -1/z, omeg);
  FMmatrix<double> lambda;
  FMmatrix<double> logomeg;
  int i;

  if(z>0)
    {
      //lambda= min(z*(log(z)-log(omeg)-2)-1)-lam_offset
      logmat(logomeg,omeg);
      scalar_mult_mat(logomeg,-z,logomeg);
      scalar_add_mat(logomeg,z*log(z)-2*z-1,logomeg);
      lambda=min(logomeg,0);
      scalar_add_mat(lambda,-lam_offset,lambda);
    }
  else
    {
      //lambda=-sum(omeg)-min(log(omeg))
      logmat(logomeg,omeg);
      mat_add(lambda,sum(omeg,0),min(logomeg,0));
      scalar_mult_mat(lambda,-1.0,lambda);	
    }
  lambda.reset(omeg.numrow(),omeg.numcol(),lambda(0));
  FMmatrix<double> thet(omeg.numrow(),omeg.numcol());
  FMmatrix<double> la(omeg.numrow(),omeg.numcol());
  FMmatrix<double> tempsum;
  int lIter;
  for(lIter=0;lIter<2;lIter++)
    {
      //la=log(sgn*oz)+(1+lambda/z)   sgn=sign(-z)
      la=oz;
      if(z>0)
	scalar_mult_mat(la,-1.0,la);
      logmat(la,la);
      scalar_add_mat(la,1.0,la);
      scalar_mult_mat(lambda,1/z,lambda);
      mat_add(la,la,lambda);
      
      if(z>0)
	{
	  for(i=0;i<omeg.numel();i++)
	    {
	      if(la(i)>-745) //gidx
		thet(i)=oz(i)/LambertW1(-1*exp(la(i)));
	      else
		thet(i)=oz(i)/lambert_arg_outof_range(la(i));
	    }
	}
      else
	{
	  for(i=0;i<omeg.numel();i++)
	    {
	      if(la(i)<709) //gidx
		thet(i)=oz(i)/LambertW(exp(la(i)));
	      else
		thet(i)=oz(i)/lambert_arg_outof_range(la(i));
	    }
	}
      tempsum=sum(thet,0);
      scalar_mult_mat(thet, 1/tempsum(0),thet);
      ew_div(lambda,omeg,thet);
      logmat(logomeg,thet);  //logomeg=log(thet)
      scalar_mult_mat(logomeg,z,logomeg);
      mat_add(lambda,lambda,logomeg);
      scalar_mult_mat(lambda,-1.0,lambda);
      scalar_add_mat(lambda,-z-lam_offset,lambda);      
    }
  return thet;
}

