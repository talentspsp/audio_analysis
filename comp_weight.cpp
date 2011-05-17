/*
  This function is used to get the weights of different components in a given distribution
  Input: 
  psfz:a 1*nums double* array. each double* is the components of a source, which is a 2-D matrix, each row is a distribution in frequency domain
  numz:a 1*nums array, each element is the number of the components of the corresponding source
  nums:number of sources
  numf:length of frequency domain
  Vft:a numt*numf array, each row is a distribution in frequency domain
  numt: length in time domain
  maxiter: max number of iteration
  result_pts: a numt*nums array, each row the probability of different sources, used for returning, space will be malloc inside the function
  ptzs: a 1*nums double* array. this part of space need to be malloced before calling this function. each double* points to a numt*numz[s] array, each row is the probability of the components of the sources, this part of space will be allocated inside the function. used for returning
  *****Notice: this function doesn't check the validation of the input!******
 */




#include <iostream>
#include "math_util.h"
#include <cstdlib>
#include <cmath>
#include <ctime>

using namespace std;

#define MIN_DOUBLE 1e-250




void comp_weight( double** psfz,size_t* numz, size_t nums,size_t numf, double* Vft, size_t numt, size_t maxiter, double** result_pts, double** ptzs)
{
  size_t s,t,z,f;
  size_t iter;
  //mxArray* temp;
  double *pts; //Pt(s)
  //double **psfz; //Ps(f|z)
  double **ptszf; //Pt(s,z | f)
  double *denom, *temp, *mxtemp;
  double tsum_s; //temporary sum over s
 

  pts=new double[numt*nums];
  //ptzs=new double*[nums];
  for(s=0;s<nums;s++)
    ptzs[s]=new double[numt*numz[s]];
  ptszf=new double*[nums];
  for(s=0;s<nums;s++)
    ptszf[s]=new double[numz[s]*numf];


  //randomly initialize pts and ptzs
  srand(time(NULL));
  for(t=0;t<numt;t++)
    {
      tsum_s=0;
      for(s=0;s<nums;s++)
	{
	  pts[t*nums+s]=rand()%1000+1;
	  tsum_s+=pts[t*nums+s];
	}
      scalar_mult_mat(pts+t*nums,pts+t*nums,1,nums,1/tsum_s); //make the sum of each line equal to 1
    }

  for(s=0;s<nums;s++)
    {
      for(t=0;t<numt;t++)
	{
	  tsum_s=0;
	  for(z=0;z<numz[s];z++)
	    {
	      ptzs[s][t*numz[s]+z]=rand()%1000+1;
	      tsum_s+=ptzs[s][t*numz[s]+z];
	    }
	  scalar_mult_mat(ptzs[s]+t*numz[s],ptzs[s]+t*numz[s],1,numz[s],1/tsum_s);
	}
    }

  temp=new double[numf];
  denom=new double[numf];

  for(t=0;t<numt;t++)
    {
      iter=0;
      while(iter<maxiter)
	{
	  iter++;
	  // denom=sum_s Pt(s) sum_z Pt(z|s)Ps(f|z)
	  setzero(denom, numf);
	  for(s=0;s<nums;s++)
	    {
	       setzero(temp, numf);
	       for(z=0;z<numz[s];z++)
		 for(f=0;f<numf;f++)
		   temp[f]+=ptzs[s][t*numz[s]+z]*psfz[s][z*numf+f];
	       scalar_mult_mat(temp,temp,1, numf,pts[t*nums+s]);
	       mat_add(denom,denom,temp,1,numf);
	    }
	    
	  // Pt(s,z|f)=Pt(s)*Pt(z|s)*Ps(f|z)/denom
	  for(s=0;s<nums;s++)
	    for(z=0;z<numz[s];z++)
	      {
		scalar_mult_mat(ptszf[s]+z*numf, psfz[s]+z*numf, 1, numf, pts[t*nums+s]*ptzs[s][t*numz[s]+z]);
		//Protect: check whether the denom contain 0
		for(f=0;f<numf;f++)
		  denom[f]=(denom[f]<MIN_DOUBLE)?MIN_DOUBLE:denom[f];
		ew_div(ptszf[s]+z*numf, ptszf[s]+z*numf, denom, 1, numf);
	      }

	  //debug:
	  /*mexPrintf("iter %d, t %d: ptszf is\n",iter,t);
	  for(s=0;s<nums;s++)
	    {	    
	      mxprintmat(ptszf[s],numz[s],numf);
	      mexPrintf("\n");
	      }*/

	  //Pt(s)=(sum_z sum_f Pt(s,z|f)*Vft) /( sum_s sum_z sum_f Pt(s,z|f)*Vft)
	  //and Pt(z|s)=(sum_f Pt(s,z|f)*Vft) /( sum_z sum_f Pt(s,z|f)*Vft) they have similar parts, so implemented together
	  setzero(pts+t*nums,nums);
	  for(s=0;s<nums;s++)
	    setzero(ptzs[s]+t*numz[s],numz[s]);
	  
	  for(s=0;s<nums;s++)
	    for(z=0;z<numz[s];z++)
	      for(f=0;f<numf;f++)
		ptzs[s][t*numz[s]+z]+=ptszf[s][z*numf+f]*Vft[t*numf+f];


	  for(s=0;s<nums;s++)
	    for(z=0;z<numz[s];z++)
	      pts[t*nums+s]+=ptzs[s][t*numz[s]+z];

	  //Protect: check whether some pts=0
	  for(s=0;s<nums;s++)
	    pts[t*nums+s]=(pts[t*nums+s]<MIN_DOUBLE)?MIN_DOUBLE:pts[t*nums+s];
	  
	  tsum_s=0;
	  for(s=0;s<nums;s++)
	    tsum_s+=pts[t*nums+s];

	  for(s=0;s<nums;s++)
	    scalar_mult_mat(ptzs[s]+t*numz[s],ptzs[s]+t*numz[s],1,numz[s],1/pts[t*nums+s]);

	  scalar_mult_mat(pts+t*nums,pts+t*nums,1,s,1/tsum_s);

	  //debug:
	  /*mexPrintf("iter %d, t %d: pts is\n",iter,t);
	  mxprintmat(pts,numt,nums);
	  mexPrintf("\n");*/
	  //debug:
	  /* mexPrintf("iter %d, t %d: ptzs is\n",iter,t);
	  for(s=0;s<nums;s++)
	    {
	      mxprintmat(ptzs[s],numt,numz[s]);
	      mexPrintf("\n");
	      } */
	}
      
    }
  
  *result_pts=pts;

  //Release the space for ptszf
  for(s=0;s<nums;s++)
    delete []ptszf[s];
  delete []ptszf;
  delete []temp;
  delete []denom;
}

