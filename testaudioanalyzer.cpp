#include "audio_analyzer.h"
#include "matfile.h"
#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
using namespace std;

int main()
{
  audio_analyzer A;
  char filename1[]="testdata/80s.wav";
  //char filename1[]="testdata/03a01Nc.wav";
      /* char filename3[]="testdata/03a01Wa.wav";
  char filename4[]="testdata/Intermission.wav";*/
  //set the parameters
  A.set_fft_param(1024,1024,256);
  A.set_sparsez(0.2);
  A.set_thd_db(-1000);
  A.set_thd_pz(0);
  A.set_numcomp_per_seg(10);
  A.set_max_iter_plca(100);
  A.set_max_itertau(2);
  A.set_max_iter_cw(100);
  A.set_seglen_sec(0);

  A.readindata(filename1,0);
  cout<<"read in file 1 finished!"<<endl;
  cout<<"frequecy is "<<A.get_fz()<<endl;
  cout<<"data size is "<<A.get_datasize()<<endl;
  double* wavdata=new double[A.get_datasize()];
  complex<double>* wavdatacpx=A.get_data();
  size_t i,j;
  for(i=0;i<A.get_datasize();i++)
    {
      wavdata[i]=wavdatacpx[i].re;
    }
  mat2bin("testdata/80s_wavdata.bin",1,A.get_datasize(),wavdata);

  //test STFT
  A.do_stft();
  int lent=A.get_lent(), lenf=A.get_lenf();
  //see whether sftf which return complex number is correct
  ofstream outfd("testdata/stftout");
  complex<double>* result=A.get_STFT_cpx();
  for(i=0;i<lent;i++)
    {
      for(j=0;j<lenf;j++)
	{
	  outfd<<result[i*lenf+j].re;
	  if(result[i*lenf+j].im>=0)
	    outfd<<"+";
	  outfd<<result[i*lenf+j].im<<"i"<<"\t";
	}
      outfd<<endl;
    }
  delete []result;
  //  datafd.close();
  outfd.close();



  cout<<"stft finished!"<<endl;
  A.stftcpx2double();
  mat2bin("testdata/80s_STFT.bin",(size_t)(A.get_lent()),(size_t)(A.get_lenf()), A.get_STFT_double());
  cout<<"stft to double finished!"<<endl;
  cout<<"lenf is "<<A.get_lenf()<<endl;
  cout<<"lent is "<<A.get_lent()<<endl;
  cout<<"thd_db is "<<A.get_thd_db()<<endl;
  cout<<"lent_gt_thd is "<<A.get_lent_gt_thd()<<endl;
  //test PLCA
  clock_t start, finish;
  double timeused;
  cout<<"plca start!"<<endl;
  start=clock();
  A.plca_on_data();
  finish=clock();
  cout<<"plca finished!"<<endl;
  timeused=(double)(finish-start)/CLOCKS_PER_SEC;
  cout<<"use time "<<timeused<<"s"<<endl;
  cout<<"seglen frame is "<<A.get_seglen_frame()<<endl;
  cout<<"number of segments is "<<A.get_numseg()<<endl;
  cout<<"current label is "<<A.get_curr_label()<<endl;
  cout<<"file1's number of components are"<<A.get_numcomp_all()<<endl;
  // printmat(A.get_pfz(),A.get_numcomp(),A.get_lenf());
  mat2bin("testdata/80s_W.bin",A.get_numcomp_all(),A.get_lenf(),A.get_pfz());
  mat2bin("testdata/80s_H.bin",A.get_lent(),A.get_numcomp_all(),A.get_ptz());
  mat2bin("testdata/80s_Z.bin",A.get_numseg(),A.get_numcomp_per_seg(),A.get_pz());
  /* A.add_comps();
  cout<<"add comps finished!"<<endl;
  //add another file
  A.readindata(filename2,1);
  cout<<"read in file2 finished!"<<endl;
  //test STFT
  A.do_stft();
  cout<<"stft finished!"<<endl;
  A.stftcpx2double();
  cout<<"stft to double finished!"<<endl;
  cout<<"lenf is "<<A.get_lenf()<<endl;
  cout<<"lent is "<<A.get_lent()<<endl;
  cout<<"thd_db is "<<A.get_thd_db()<<endl;
  cout<<"lent_gt_thd is "<<A.get_lent_gt_thd()<<endl;

  //test PLCA
  A.plca_on_data();
  cout<<"plca finished!"<<endl;
  cout<<"current label is "<<A.get_curr_label()<<endl;
  cout<<"file2's components are"<<endl;
  // printmat(A.get_pfz(),A.get_numcomp(),A.get_lenf());
  mat2asc("03a01Nc_comps.ascii", A.get_numcomp_all(),A.get_lenf(),A.get_pfz());
  A.add_comps();
  cout<<"add comp finished!"<<endl;
  A.hierarchical_plca(0, 20, 0.025, 0.2);
  cout<<"hierarchical plca finished !"<<endl;
 

  A.readindata(filename1,0);
  cout<<"read in file3 finished!"<<endl;
  cout<<"number of class is "<<A.get_numclass()<<endl;
  //test STFT
  A.do_stft();
  cout<<"stft finished!"<<endl;
  A.stftcpx2double();
  cout<<"stft to double finished!"<<endl;

  //test PLCA
  A.plca_on_data();
 //predict
  clock_t start, finish;
  double timeused;
  start=clock();
  A.get_comp_weight();
  finish=clock();
  timeused=(double)(finish-start)/CLOCKS_PER_SEC;
  cout<<"get comp weight finished!"<<endl;
  int lb=A.pred_max_weight();
  // double* pts=A.get_pts();
  cout<<"predicted label is "<<lb<<endl;
  cout<<"time used is "<<timeused<<endl;
  cout<<"class prob is "<<endl;
  printmat(A.get_class_prob(),1,A.get_numclass());*/
  return 0;
}
