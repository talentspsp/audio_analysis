#include "audio_analyzer.h"
#include "STFT.h"
#include "wavproc/wav_in.h"
#include "plca2d.h"
#include <stdexcept>
#include <cmath>
#include <iostream>
#include "comp_weight.h"
#include "math_util.h"
#include <cstring> //for memcpy
using namespace std;

audio_analyzer::audio_analyzer(size_t nclass)
{
  if(nclass==0)
    throw(runtime_error("class number can't be zero!"));
  numclass=nclass;
  class_comps.resize(numclass);
  extracted_comps.resize(numclass);
  data=NULL;
  datasize=0;
  fftN=0;
  fftW=0;
  fftH=0;
  lenf=0;
  lent=0;
  fz=0;
  curr_label=-1;
  label_pred=-1;
  max_iter_plca=100;
  seglen=0;
  numcomp=0;
  sparse_z=0;
  max_itertau=2;
  pz=NULL;
  thd_db=-4.5;
  thd_pz=0;
  numz=new size_t[nclass];
  max_iter_cw=100;
  ncomp_gt_thd=0;
  opt_get_cw=1;
  STFT_cpx=NULL;
  ptz=NULL;
  pfz=NULL;
  STFT_double=NULL;
}

int audio_analyzer::readindata(char* filename, int label)
{
  curr_label=label;
  try
    {
      WAV_IN wavin(filename);
      int num_ch=wavin.get_num_channels();
      int num_samples = wavin.get_num_samples();
      datasize=num_samples/num_ch;
      fz= wavin.get_sample_rate_hz();
      double dl,dr; //data in left and right channels
      if(data!=NULL)
	delete []data;
      data=new complex<double>[datasize];
      int i;
      switch(num_ch)
	{
	case 1:
	  for(i=0;i<datasize;i++)
	    {
	      dl=wavin.read_current_input();
	      data[i].re=dl;
	      data[i].im=0;
	    }
	  break;
	case 2: // if double channel, add the samples from the two channels
	  for(i=0;i<datasize;i++)
	    {
	      wavin.read_current_input(&dl,&dr);
	      data[i].re=dl+dr;
	      data[i].im=0;
	    }
	  break;
	default:
	  throw(runtime_error("Error: wrong number of channels!"));
	}
      return 0;
    }
  catch(runtime_error err)
    {
      cerr<<err.what()<<endl;
      return -1;
    }
}

void audio_analyzer::do_stft()
{
  if(STFT_cpx != NULL)
    delete []STFT_cpx;
  STFT_cpx=STFT(data, datasize, fftN, fftW, fftH,lenf,lent);
}

void audio_analyzer::stftcpx2double(int option)
{
  int t,f;
  switch(option)
    {
    case 0: // the abs of the complex
      if(STFT_double != NULL)
	delete []STFT_double;
      STFT_double=new double[lent*lenf];
      for(t=0;t<lent;t++)
	for(f=0;f<lenf;f++)
	  STFT_double[t*lenf+f]=sqrt(pow(STFT_cpx[t*lenf+f].re,2)+pow(STFT_cpx[t*lenf+f].im,2));
      break;
    default:
      cerr<<"wrong option for stftcpx2double: "<<option<<endl;
      return;      
    }
}

void audio_analyzer::plca_on_data()
{
  if(ptz != NULL)
    delete []ptz;
  if(pfz != NULL)
    delete []pfz;
  if(pz != NULL)
    delete []pz;
  if(seglen==0)  //seglen==0, use the all the frames to do plca
    plca2d(STFT_double,(size_t)lenf, (size_t)lent, numcomp, max_iter_plca, sparse_z, &ptz, &pfz,&pz,max_itertau);
  else
    {
      //TODO: decide what seglen should represent, now it's only considered as number of frames to do plca
      numseg=(size_t)lent/seglen; //by this, drop the last part, if the last part is shorter than seglen
      ptz=new double[numseg*seglen*numcomp];//numseg*seglen row, numcomp column
      pfz=new double[numcomp*numseg*lenf];//numcomp*numseg row, lenf column
      pz=new double[numcomp*numseg];
      double* temp_ptz;
      double* temp_pfz;
      double* temp_pz;
      
      for(size_t i=0;i<numseg;i++)
	{
	  plca2d(STFT_double+i*seglen*lenf,(size_t)lenf, seglen, numcomp, max_iter_plca, sparse_z, &temp_ptz, &temp_pfz,&temp_pz,max_itertau);
	  //copy the temp result
	  memcpy(ptz+i*seglen*numcomp,temp_ptz,seglen*numcomp*sizeof(double));
	  memcpy(pfz+i*numcomp*lenf,temp_pfz,lenf*numcomp*sizeof(double));
	  memcpy(pz+i*numcomp,temp_pz,numcomp*sizeof(double));
	  delete []temp_ptz;
	  delete []temp_pfz;
	  delete []temp_pz;	  
	}
      
    }
}

size_t audio_analyzer::add_comps()
{
  if(curr_label<0 || curr_label>=numclass)
    {
      cerr<<"the class of the data is not decided, can't add the components."<<endl;
      return 0;
    }
  ncomp_gt_thd=0;
  size_t z,f;
  for(z=0;z<numcomp;z++)
    if(pz[z]>thd_pz)
      ncomp_gt_thd++;
  datablk blk;
  blk.row=ncomp_gt_thd;
  blk.col=lenf;
  blk.data=new double[ncomp_gt_thd*lenf];
  size_t cnt=0;
  for(z=0;z<numcomp;z++)
    if(pz[z]>thd_pz)
      {
	for(f=0;f<lenf;f++)
	  {
	    blk.data[cnt*lenf+f]=pfz[z*lenf+f];	    
	  }
	cnt++;
      }
  class_comps[curr_label].push_back(blk);
  return cnt;
}


bool audio_analyzer::hierarchical_plca(size_t depth, size_t ncomps, double thd_gpz, double sz)
{
  if(depth>1)
    {
      cerr<<"depth for hierarchical plca can only be 0 or 1"<<endl;
      return false;
    }
  double* inputdata;
  double* gptz;
  double* gpfz;
  double* gpz;
  size_t z,i,j,k,nc,p;
  datablk blk;
  for(z=0;z<numclass;z++)
    {
      nc=0;
      for(i=0;i<class_comps[z].size();i++)
	nc+=class_comps[z][i].row;
      inputdata=new double[nc*lenf];
      p=0;
      for(i=0;i<class_comps[z].size();i++)
	{
	  for(j=0;j<class_comps[z][i].row*class_comps[z][i].col;j++)
	    {
	      inputdata[p+j]=class_comps[z][i].data[j];
	    }
	  p+=class_comps[z][i].row*class_comps[z][i].col;
	}
      if(depth==0) // just copy to extracted_comps
	{
	  blk.row=nc;
	  blk.col=lenf;
	  blk.data=inputdata;
	  extracted_comps[z]=blk;
	}
      if(depth==1)
	{
	  plca2d(inputdata,(size_t)lenf, nc, ncomps, max_iter_plca, sz, &gptz, &gpfz,&gpz,max_itertau);
	  nc=0;
	  for(i=0;i<ncomps;i++)
	    if(gpz[i]>thd_gpz)
	      nc++;
	  blk.row=nc;
	  blk.col=lenf;
	  blk.data=new double[nc*lenf];
	  k=0;
	  for(i=0;i<ncomps;i++)
	    if(gpz[i]>thd_gpz)
	      {
		for(j=0;j<lenf;j++)
		  blk.data[k+j]=gpfz[i*lenf+j];
		k+=lenf;
	      }
	  extracted_comps[z]=blk;
	  delete []inputdata;
	}
    }
  return true;
}

bool audio_analyzer::get_comp_weight()
{
  double** psfz=new double*[numclass];
  size_t i,z,f;
  double* v;
  size_t cnt;
  for(i=0;i<numclass;i++)
    {
      numz[i]=extracted_comps[i].row;
      psfz[i]=extracted_comps[i].data;
    }
  ptzs=new double*[numclass];
  switch(opt_get_cw)
    {
    case 1:
      comp_weight( psfz,numz,numclass, lenf, STFT_double, (size_t)lent, max_iter_cw, &pts, ptzs);
      break;
    case 2:
      comp_weight( psfz,numz,numclass, lenf, pfz, (size_t)numcomp, max_iter_cw, &pts, ptzs);
      break;
    case 3:
      v=new double[ncomp_gt_thd*lenf];
      cnt=0;
      for(z=0;z<numcomp;z++)
	if(pz[z]>thd_pz)
	  for(f=0;f<lenf;f++)
	    {
	      v[cnt*lenf+f]=pfz[z*lenf+f];
	      cnt++;
	    }
      comp_weight( psfz,numz,numclass, lenf, v, ncomp_gt_thd, max_iter_cw, &pts, ptzs);
      delete []v;
      break;
    default:
      cerr<<"error in get_comp_weight: no option "<<opt_get_cw<<endl;
      return false;
    }

  delete []psfz;
  return true;
}

int audio_analyzer::pred_max_weight()
{
  size_t tmplent,tmplb;
  switch(opt_get_cw)
    {
    case 1:
      tmplent=lent;
      break;
    case 2:
      tmplent=(size_t)numcomp;
      break;
    case 3:
      tmplent=ncomp_gt_thd;
      break;
    default:
      cerr<<"error in pred_max_weight: no such option "<<opt_get_cw<<endl;
      return -1;
      break;
    }
  double* tmppts=new double[numclass];
  size_t t;
  setzero(tmppts,numclass);
  for(t=0;t<tmplent;t++)
    mat_add(tmppts, tmppts, pts, 1, numclass);
  max(tmppts,numclass,tmplb);
  label_pred=tmplb;
  delete []tmppts;
  return label_pred;
}
