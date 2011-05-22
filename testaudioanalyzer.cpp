#include "audio_analyzer.h"
#include "matfile.h"
#include <iostream>
#include <string>
#include <ctime>
using namespace std;

int main()
{
  audio_analyzer A(2);
  char filename1[]="testdata/03a01Fa.wav";
  char filename2[]="testdata/03a01Nc.wav";
  char filename3[]="testdata/03a01Wa.wav";
  char filename4[]="testdata/Intermission.wav";
  //set the parameters
  A.set_fft_param(1024,1024,512);
  A.set_sparsez(0.2);
  A.set_thd_db(-4.5);
  A.set_thd_pz(0);
  A.set_numcomp_per_seg(5);
  A.set_max_iter_plca(100);
  A.set_max_itertau(2);
  A.set_max_iter_cw(100);
  A.set_seglen_sec(5);

  A.readindata(filename1,0);
  cout<<"read in file 1 finished!"<<endl;
  cout<<"frequecy is "<<A.get_fz()<<endl;


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
  cout<<"seglen frame is "<<A.get_seglen_frame()<<endl;
  cout<<"number of segments is "<<A.get_numseg()<<endl;
  cout<<"current label is "<<A.get_curr_label()<<endl;
  cout<<"file1's number of components are"<<A.get_numcomp_all()<<endl;
  // printmat(A.get_pfz(),A.get_numcomp(),A.get_lenf());
  mat2asc("03a01Fa_comps.ascii", A.get_numcomp_all(),A.get_lenf(),A.get_pfz());
  A.add_comps();
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
  printmat(A.get_class_prob(),1,A.get_numclass());
  return 0;
}
