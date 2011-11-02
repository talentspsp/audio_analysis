#include "plcadirichlet.h"
#include "fastmath.h"
#include "LambertWs.h"
#include "lambert_compute_with_offset.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "CBLAS/include/cblas.h"
using namespace std;

//FMmatrix<double> lambert_compute_with_offset(const FMmatrix<double>& in_omeg, double z, double lam_offset);

void plcadirichlet_BLAS(const FMmatrix<double>& x, int K, int iter, const FMmatrix<double>& in_sz, const FMmatrix<double>& in_sw, const FMmatrix<double>& in_sh, const FMmatrix<double>& in_z, const FMmatrix<double>& in_w, const FMmatrix<double>& in_h, const FMmatrix<bool>& lw, const FMmatrix<bool>& lh, FMmatrix<double>& out_w,  FMmatrix<double>& out_h,  FMmatrix<double>& out_z, double dc)
{
  srand(time(0));
  int M=x.numrow(), N=x.numcol();
  int i,j;
  //initialize
  if(dc<0.001)
    dc=0.001;
  dc=-dc;
  /* FMmatrix<bool> dw(lw);
  FMmatrix<bool> dh(lh);
  //bool empflag=true; // if all the elements of lw is true, then dw=lw, otherwith, dw=allw-lw
  for(i=0;i<lw.numel();i++)
    dw(i)=!lw(i);
  for(i=0;i<lh.numel();i++)
  dh(i)= !lh(i);*/
    

  out_w.reset(M,K);
  if(in_w.isempty())
    out_w.randset();
  else
    {
      if(in_w.numrow() != M || in_w.numcol() > K)
	throw runtime_error("plca input w's dimension wrong!");
      for(i=0;i<M;i++)
	for(j=0;j<in_w.numcol();j++)
	  out_w(i,j)=in_w(i,j);
      for(;j<K;j++)
	for(i=0;i<M;i++)
	  out_w(i,j)=rand()%1000+1;	
    }
  mat_ewdiv_vec(out_w,out_w,sum(out_w,1));

  out_h.reset(K,N);
  if(in_h.isempty())
    out_h.randset();
  else
    {
      if(in_h.numcol() != N || in_h.numrow()>K)
	throw runtime_error("plca input h's dimension wrong!");
      for(i=0;i<in_h.numrow();i++)
	for(j=0;j<N;j++)
	  out_h(i,j)=in_h(i,j);
      for(;i<K;i++)
	for(j=0;j<N;j++)
	  out_h(i,j)=rand()%1000+1;
    }
  mat_ewdiv_vec(out_h,out_h,sum(out_h,2));
  
  if(!in_z.isempty() && (in_z.numrow() != 1 || in_z.numcol() != K))
    throw runtime_error("the input z should be an empty matrix or a 1*K matrix!");
  if(in_z.isempty())
    {
      out_z.reset(1,K);
      out_z.randset();
    }
  else
    out_z=in_z;
  mat_ewdiv_vec(out_z,out_z,sum(out_z,0));

  FMmatrix<double> sw,sh,sz;
  FMmatrix<double> tempsum;
  bool isws, ishs, iszs;
  if(in_sw.numel()==1)
    sw.reset(iter,K,in_sw(0));
  else
    {
      if(in_sw.numrow() != iter || in_sw.numcol() != K)
	throw runtime_error("in_sw should only be 1*1 matrix or iter*K matrix");
      sw=in_sw;
    }
  tempsum=sum(in_sw,0);
  isws=(tempsum(0) != 0);
 
  if(in_sh.numel()==1)
    sh.reset(iter,K,in_sh(0));
  else
    {
      if(in_sh.numrow() != iter || in_sh.numcol() != K)
	throw runtime_error("in_sh should only be 1*1 matrix or iter*K matrix");
      sh=in_sh;
    }
  tempsum=sum(in_sh,0);
  ishs=(tempsum(0) != 0);

  if(in_sz.numel()==1)
    sz.reset(iter,K,in_sz(0));
  else
    {
      if(in_sz.numrow() != iter || in_sz.numcol() != K)
	throw runtime_error("in_sz should only be 1*1 matrix or iter*K matrix");
      sz=in_sz;
    }
  tempsum=sum(in_sz,0);
  iszs=(tempsum(0) != 0);

  FMmatrix<double> zh(K,N);
  FMmatrix<double> R;
  FMmatrix<double> tempprod, tempcol, tempcol2, temprow,temprow2;
  FMmatrix<double> nw,nh;
  int it;
  bool islw=false, islh=false;
  if(lw.numrow() != 1 || lw.numcol() != K)
    throw runtime_error("lw should be a 1*K matrix");
  for(i=0;i<K;i++)
    if(lw(i))
      {
	islw=true;
	break;
      }

  if(lh.numrow() != 1 || lh.numcol() != K)
    throw runtime_error("lh should be a 1*K matrix");
  for(i=0;i<K;i++)
    if(lh(i))
      {
	islh=true;
	break;
      }
  double kappa;
  for(it=0;it<iter;it++)
    {
      kappa=exp(dc*(it+1));
      //zh=diag(z)*h
      mat_ewmult_vec(zh, out_h, out_z.transp());
      //tempprod=w*zh
      // mat_mult(tempprod, out_w, zh);
      tempprod.reset(M,N);
      cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, out_w.numrow(), zh.numcol(), out_w.numcol(), 1.0, out_w.getdata(), out_w.numcol(), zh.getdata(), zh.numcol(), 0.0,tempprod.getdata(), zh.numcol());
      //R=x./(w*zh)
      ew_div(R, x, tempprod);
      
      //M-step
      //  if(islw)
      //	{
	  // nw=w.*(R*zh')
      // mat_mult(tempprod, R, zh.transp());
      tempprod.reset(M,K);
      cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, R.numrow(), zh.numrow(), R.numcol(), 1.0, R.getdata(), R.numcol(), zh.getdata(), zh.numcol(), 0.0,tempprod.getdata(), zh.numrow());
	  ew_mult(nw, out_w, tempprod);
	  //}
	  // if(islh)
	  //{
	  //nh = zh .* (w'*R);
	  // mat_mult(tempprod, out_w.transp(), R);
	  tempprod.reset(K,N);
	  cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans, out_w.numcol(), R.numcol(), R.numrow(), 1.0, out_w.getdata(), out_w.numcol(), R.getdata(), R.numcol(), 0.0,tempprod.getdata(), R.numcol());
	  ew_mult(nh,zh,tempprod);
	  //	}

      if(islw)
	out_z=sum(nw,1);
      else //if(islh)
	{
	  out_z=sum(nh,2);
	  out_z=out_z.transp();
	}
      //Impose sparsity constraints
      int tl;
      if(isws)
	for(tl=0;tl<lw.numel();tl++)
	  if(sw(it,tl) != 0)
	    {
	      nw.setcol(tl,lambert_compute_with_offset(nw.getcol(tl), sw(it,tl), 0));
	      //out_w.setcol(tl, tempcol);
	    }
      
      if(ishs)
	for(tl=0;tl<lh.numel();tl++)
	  if(sh(it,tl) != 0)
	    {
	      nh.setrow(tl,lambert_compute_with_offset(nh.getrow(tl), sh(it,tl), 0));
	      //out_h.setrow(tl,temprow);
	    }
      
      if(sz(it) != 0 )
	out_z=lambert_compute_with_offset(out_z, sz(it), 0);
      out_z.avoidzero();
      tempsum=sum(out_z,0);
      scalar_mult_mat(out_z,1/tempsum(0),out_z); 
      
      for(tl=0;tl<lw.numel();tl++)
	{
	  tempcol=nw.getcol(tl);
	  tempcol.avoidzero();
	  tempsum=sum(tempcol,0);
	  scalar_mult_mat(tempcol,1/tempsum(0),tempcol);
	  if(!lw(tl))
	    {
	      tempcol2=out_w.getcol(tl);
	      scalar_mult_mat(tempcol2, 1-kappa,tempcol2);
	      scalar_mult_mat(tempcol,kappa,tempcol);
	      mat_add(tempcol,tempcol,tempcol2);
	      tempcol.avoidzero();
	    }
	  out_w.setcol(tl, tempcol);
	}
      for(tl=0;tl<lh.numel();tl++)
	  {
	    temprow=nh.getrow(tl);
	    temprow.avoidzero();
	    tempsum=sum(temprow,0);
	    scalar_mult_mat(temprow,1/tempsum(0),temprow);
	    if(!lh(tl))
	      {
		temprow2=out_h.getrow(tl);
		scalar_mult_mat(temprow2, 1-kappa,temprow2);
		scalar_mult_mat(temprow,kappa,temprow);
		mat_add(temprow,temprow,temprow2);
		temprow.avoidzero();
	      }
	    out_h.setrow(tl,temprow);
	  }      
    }  
}


/*FMmatrix<double> lambert_compute_with_offset(const FMmatrix<double>& in_omeg, double z, double lam_offset)
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
*/
