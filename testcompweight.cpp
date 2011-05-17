#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <sstream>
#include "comp_weight.h"
#include "matfile.h"
#include "math_util.h"
using namespace std;

int main()
{
  size_t nums=10;
  size_t numf=10;
  size_t numt=10;
  size_t* numz=new size_t[nums];
  size_t maxz=20;
  double** psfz=new double*[nums];
  double* Vft=new double[numt*numf];
  size_t maxiter=1000;
  srand(time(NULL));
  //random initialize the components in each sourch
  size_t s,f,t,z;
  double sum;
  for(s=0;s<nums;s++)
    {
      numz[s]=rand()%maxz+1;
      psfz[s]=new double[numz[s]*numf];
      for(z=0;z<numz[s];z++)
	{
	  sum=0;
	  for(f=0;f<numf;f++)
	    {
	      psfz[s][z*numf+f]=rand()%1000+1;
	      sum+=psfz[s][z*numf+f];
	    }
	  scalar_mult_mat(psfz[s]+z*numf, psfz[s]+z*numf, 1, numf, 1/sum);
	}
    }
  //random initialize Vft
  sum=0;
  for(t=0;t<numt;t++)
    for(f=0;f<numf;f++)
      {
	Vft[t*numf+f]=rand()%1000+1;
	sum+=Vft[t*numf+f];
      }
  scalar_mult_mat(Vft, Vft, numt, numf, 1/sum);
  //result
  double* pts;
  double** ptzs=new double*[nums];
  comp_weight( psfz,numz, nums,numf, Vft, numt, maxiter, &pts, ptzs);
  ostringstream oss;
  string filename;
  for(s=0;s<nums;s++)
    {
      oss.str("");
      oss<<"psfz_"<<s<<".txt";
      filename=oss.str();
      cout<<"numz is "<<numz[s]<<endl;
      mat2asc(filename.c_str(),numz[s], numf, psfz[s]);
    }
  filename="Vft.txt";
  mat2asc(filename.c_str(),numt, numf, Vft);

  filename="pts.txt";
  mat2asc(filename.c_str(),numt, nums, pts);

  for(s=0;s<nums;s++)
    {
      oss.str("");
      oss<<"ptzs_"<<s<<".txt";
      filename=oss.str();
      mat2asc(filename.c_str(),numt, numz[s], ptzs[s]);
    }
  return 0;
  
}
