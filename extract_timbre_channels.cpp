#include <iostream>
#include "audio_analyzer.h"
#include "matfile.h"
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <fstream>
using namespace std;

/*
  this function is used to extract timbre channels
  usage:
  extract_timbre_channels [options] training_file_list extracting_file_list saving_path
  training_file_list: this is a file, in which each line is a path to a song. These songs are used to extract timbre components
  extracting_file_list: this is a file, in which each line is a path to a song. These songs are used to be by H estimated with fix W
  saving_path: the path to the directory to save the results
  -fftN N: N points fft. default: 4096
  -fftW W: window size. default: 4096
  -fftH H: hop size: default: 2048
  
 */
#define DEFAULT_FFTN 4096
#define DEFAULT_FFTW 4096
#define DEFAULT_FFTH 2048
#define DEFAULT_VERBOSE 0
#define DEFAULT_PSZ 0.3
#define DEFAULT_PNC 20
#define DEFAULT_PZTHD 0
#define DEFAULT_GSZ 0.25
#define DEFAULT_GNC 100
#define DEFAULT_GZTHD 0
#define DEFAULT_SEG_LEN_SEC 30
#define DEFAULT_SAVE 0
#define DEFAULT_DIRICH 1
#define DEFAULT_DP 0.01
#define DEFAULT_ESZ 0.2
int main(int argc, char** argv)
{
  if(argc<4)
    {
      cout<<"Usage: extract_timbre_channels [options] training_file_list extracting_file_list saving_path"<<endl;
      cout<<"Options:"<<endl;
      cout<<"-fftN N: N points fft. default: 4096"<<endl;
      cout<<"-fftW W: window size. default: 4096"<<endl;
      cout<<"-fftH H: hop size. default: 2048"<<endl;
      cout<<"-v V: verbose, how much you want to know. [0-1] default:0"<<endl;
      cout<<"-psz PSZ: per sample sparseness prior used in plca, default:0.3"<<endl;
      cout<<"-pnc PNC: per sample number of components used in plca, default:20"<<endl;
      cout<<"-pzthd PZTHD: threshhold for per sample to keep the components, default: 0"<<endl;
      cout<<"-gsz GSZ: sparseness prior used in global plca, default: 0.25"<<endl;
      cout<<"-gnc GNC: number of components used in global plca, default: 100"<<endl;
      cout<<"-gzthd GZTHD: threshhold for global plca to keep the components, default: 0"<<endl;
      cout<<"-sls SLS: segment length in second, default: 30"<<endl;
      cout<<"-save SAVE: whether to save the components of each sample, 0 or 1, default: 0"<<endl;
      cout<<"-dirich d: whether to use dirichlet process when estimating H functions,0: plca, 1:dirichlet plca, default: 1"<<endl;
      cout<<"-dp p: dirichlet prior in dirichlet plca. default: 0.01"<<endl;
      cout<<"-esz z: sparseness prior used when estimating H functions from global W. default: 0.2"<<endl;
      return 1;
    }
  //process args
  string saving_path(argv[argc-1]), extracting_file_list(argv[argc-2]), training_file_list(argv[argc-3]);
  int i;
  int fftN=DEFAULT_FFTN;
  int fftW=DEFAULT_FFTW;
  int fftH=DEFAULT_FFTH;
  int v=DEFAULT_VERBOSE;
  double psz=DEFAULT_PSZ;
  int pnc=DEFAULT_PNC;
  double pzthd=DEFAULT_PZTHD;
  double gsz=DEFAULT_GSZ;
  int gnc=DEFAULT_GNC;
  double gzthd=DEFAULT_GZTHD;
  int seg_len_sec=DEFAULT_SEG_LEN_SEC;
  int ifsave=0;
  int dirich=DEFAULT_DIRICH;
  double dp=DEFAULT_DP;
  double esz=DEFAULT_ESZ;
  for(i=1;i<argc-3;i++)
    {
      if(strcmp(argv[i],"-fftN")==0)
	fftN=atoi(argv[++i]);
      if(strcmp(argv[i],"-fftW")==0)
	fftW=atoi(argv[++i]);
      if(strcmp(argv[i],"-fftH")==0)
	fftH=atoi(argv[++i]);
      if(strcmp(argv[i],"-v")==0)
	v=atoi(argv[++i]);
      if(strcmp(argv[i],"-psz")==0)
	psz=atof(argv[++i]);
      if(strcmp(argv[i],"-pnc")==0)
	pnc=atoi(argv[++i]);
      if(strcmp(argv[i],"-pzthd")==0)
	pzthd=atof(argv[++i]);
      if(strcmp(argv[i],"-gsz")==0)
	gsz=atof(argv[++i]);
      if(strcmp(argv[i],"-gnc")==0)
	gnc=atoi(argv[++i]);
      if(strcmp(argv[i],"-gzthd")==0)
	gzthd=atof(argv[++i]);
      if(strcmp(argv[i],"-sls")==0)
	seg_len_sec=atoi(argv[++i]);
      if(strcmp(argv[i],"-save")==0)
	ifsave=atoi(argv[++i]);
      if(strcmp(argv[i],"-dirich")==0)
	dirich=atoi(argv[++i]);
      if(strcmp(argv[i],"-dp")==0)
	dp=atof(argv[++i]);
      if(strcmp(argv[i],"-esz")==0)
	esz=atof(argv[++i]);
    }

  //set parameters
  audio_analyzer A;
  A.set_fft_param(fftN,fftW,fftH);
  A.set_sparsez(psz);
  A.set_thd_db(-4.5);
  A.set_thd_pz(pzthd);
  A.set_max_iter_plca(100);
  A.set_seglen_sec(seg_len_sec);
  A.set_numcomp_per_seg(pnc);
  
  string line;
  ifstream fl(extracting_file_list.c_str());
  string cmd, samplename;
  ostringstream oss;
  size_t pos;
  while(getline(fl,line))
    {
      //read file
      if(A.readindata(line.c_str(),0)<0)
	{
	  cerr<<"can't open file"<<line<<endl;
	  return -1;
	}
      if(v>0)
	{
	  cout<<"read data from "<<line<<endl;
	  cout<<"now doing stft..."<<endl;
	}
      if(v>1)
	{
	  cout<<"fftN: "<<fftN<<"  fftW: "<<fftW<<"  fftH: "<<fftH<<endl;
	}
      A.do_stft();
      A.stftcpx2double();
      if(v>0)
	{
	  cout<<"now doing PLCA..."<<endl;
	}
      if(v>1)
	{
	  cout<<"sparseness prior over z is "<<psz<<", number of components is "<<pnc<<endl;
	}
      A.plca_on_data();
      if(ifsave)
	{
	  //save the components
	  //make a directory with the name of the sample
	  oss.str(""); //clear the content
	  oss<<"mkdir -p "<<saving_path<<"/";
	  if((pos=line.find_last_of("/")) != string::npos)
	    samplename=line.substr(pos+1);
	  else
	    samplename=line;
	  oss<<samplename;
	  cmd=oss.str();
	  system(cmd.c_str());
	  if(v>0)
	    cout<<"saving components..."<<endl;
	  //save W, note: each row of W is a distribution in frequency domain
	  oss.str("");
	  oss<<saving_path<<"/"<<samplename;
	  cmd=oss.str();
	  cmd+="_W.bin";
	  
	  mat2bin(cmd.c_str(), A.get_numcomp_all(),A.get_lenf(), A.get_pfz());

	  //save H, note: each column of H is a distribution in time domain
	  cmd=oss.str();
	  cmd+="_H.bin";
	  mat2bin(cmd.c_str(), A.get_lent(),A.get_numcomp_all(), A.get_ptz());

	  //save Z, note: each row is z for a segment, the number of rows is the number of segments
	  cmd=oss.str();
	  cmd+="_Z.bin";
	  mat2bin(cmd.c_str(),A.get_numseg(),A.get_numcomp_per_seg(),A.get_pz());
	  
	}
      A.add_comps();
    }
  fl.close();
  //do global plca
  if(v>0)
    cout<<"doing global plca"<<endl;
  A.hierarchical_plca(1, gnc,gzthd, gsz);
  
  //get H functions with give global W
  if(v>0)
    cout<<"Now start estimating H functions"<<endl;
  A.set_dirich(dirich);
  A.set_dp(dp);
  A.set_esz(esz);
  fl.open(extracting_file_list.c_str());
  FMmatrix<double> result;
  while(getline(fl,line))
    {
      if(A.readindata(line.c_str())<0)
	{
	  cerr<<"can't open file"<<line<<endl;
	  return -1;
	}
      if(v>0)
	{
	  cout<<"read data from "<<line<<endl;
	  cout<<"now doing stft..."<<endl;
	}
      if(v>1)
	{
	  cout<<"fftN: "<<fftN<<"  fftW: "<<fftW<<"  fftH: "<<fftH<<endl;
	}
      A.do_stft();
      A.stftcpx2double();
      if(v>0)
	cout<<"Now estimating H functions..."<<endl;
      if(v>1)
	cout<<"dirich: "<<dirich<<" ; dirichlet prior: "<<dp<<" ; esz: "<<esz<<endl;
      A.extract_adapted_time_functions();
      if(v>0)
	cout<<"saving results..."<<endl;
      //save the estimated components
      oss.str(""); //clear the content
      oss<<"mkdir -p "<<saving_path<<"/";
      if((pos=line.find_last_of("/")) != string::npos)
	samplename=line.substr(pos+1);
      else
	samplename=line;
      oss<<samplename;
      cmd=oss.str();
      system(cmd.c_str());
      oss.str("");
      oss<<saving_path<<"/"<<samplename;
      cmd=oss.str();
      cmd+="_eW.bin";     
      mat2bin(cmd.c_str(),A.get_ew());
      cmd=oss.str();
      cmd+="_eH.bin";
      mat2bin(cmd.c_str(), A.get_eh());
      cmd=oss.str();
      cmd+="_eZ.bin";
      mat2bin(cmd.c_str(),A.get_ez());
    }
  return 0;
}
