#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include "plca2d.h"
#include "matfile.h"
using namespace std;

int main()
{
  srand(time(NULL));
  size_t lent;
  size_t lenf;
  size_t comp=10;
  size_t i,j;
  double sum=0;
  //double* data=new double[lent*lenf];
  bool st;
  double sz=0.001;
  double* ptz;
  double* pfz;
  double* pz;
  size_t max_iter=2;
  ofstream dataout("plcadata.txt");
  /*  for(i=0;i<lent;i++)
    for(j=0;j<lenf;j++)
      {
	data[i*lenf+j]=rand()%1000+1;
	sum+=data[i*lenf+j];
      }
  cout<<"data:"<<endl;
  for(i=0;i<lent;i++)
    {
      for(j=0;j<lenf;j++)
	{
	  data[i*lenf+j]=data[i*lenf+j]/sum;
	  // cout<<data[i*lenf+j]<<"  ";
	  dataout<<data[i*lenf+j]<<"  ";
	}
      //cout<<endl;
      dataout<<endl;
    }
  dataout.close();
  cout<<endl<<endl;*/
  double* data=bin2mat("testdata/80s_STFT.bin",&lent,&lenf);
  sum=0;
  for(i=0;i<lent;i++)
    for(j=0;j<lenf;j++)
      {
	sum+=data[i*lenf+j];
      }
   for(i=0;i<lent;i++)
    {
      for(j=0;j<lenf;j++)
	{
	  data[i*lenf+j]=data[i*lenf+j]/sum;
	}
    }
  //ofstream dataout("plcadata.txt");
  for(i=0;i<lent;i++)
    {
      for(j=0;j<lenf;j++)
	{
	  dataout<<data[i*lenf+j]<<"  ";
	}
      dataout<<endl;
    }
  dataout.close();
  clock_t start,finish;
  double timeused;
  start=clock();
  st=plca2d(data,lenf,lent,comp, max_iter, sz, &ptz, &pfz,&pz);
  finish=clock();
  timeused=(double)(finish-start)/CLOCKS_PER_SEC;
  cout<<"Time used: "<<timeused<<"s"<<endl;
  // cout<<"ptz:"<<endl;
  ofstream ptzout("plcaptz.txt");
  for(i=0;i<lent;i++)
    {
      for(j=0;j<comp;j++)
	{
	  //cout<<ptz[i*comp+j]<<"  ";
	  ptzout<<ptz[i*comp+j]<<"  ";
	}
      // cout<<endl;
      ptzout<<endl;
    }
  ptzout.close();
  cout<<endl<<endl;
  // cout<<"pfz"<<endl;
  ofstream pfzout("plcapfz.txt");
  for(i=0;i<comp;i++)
    {
      for(j=0;j<lenf;j++)
	{
	  // cout<<pfz[i*lenf+j]<<"  ";
	  pfzout<<pfz[i*lenf+j]<<"  ";
	}
      // cout<<endl;
      pfzout<<endl;
    }
  pfzout.close();

  ofstream pzout("plcapz.txt");
  // cout<<"pz is "<<endl;
  for(i=0;i<comp;i++)
    {	
      // cout<<pz[i]<<"  ";
      pzout<<pz[i]<<"  ";
    }
  //  cout<<endl;
  pzout.close();
  return 0;
}
