#include "audio_analyzer.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
  audio_analyzer A(2);
  char filename1[]="testdata/03a01Fa.wav";
  char filename2[]="testdata/03a01Nc.wav";
  char filename3[]="testdata/03a01Wa.wav";
  //set the parameters
  A.set_fft_param(1024,1024,512);
  A.set_sparsez(0.2);
  A.set_thd_db(-4.5);
  A.set_thd_pz(0);
  A.set_numcomp(5);
  A.set_max_iter_plca(100);
  A.set_max_itertau(2);
  A.set_max_iter_cw(100);
  A.set_seglen(0);

  A.readindata(filename1,0);
  cout<<"read in file 1 finished!"<<endl;
  //test STFT
  A.do_stft();
  cout<<"stft finished!"<<endl;
  A.stftcpx2double();
  cout<<"stft to double finished!"<<endl;

  //test PLCA
  A.plca_on_data();
  cout<<"plca finished!"<<endl;
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

  //test PLCA
  A.plca_on_data();
  cout<<"plca finished!"<<endl;
  A.add_comps();
  cout<<"add comp finished!"<<endl;
  A.hierarchical_plca(0, 20, 0.025, 0.2);
  cout<<"hierarchical plca finished !"<<endl;
 

  A.readindata(filename3,0);
  cout<<"read in file3 finished!"<<endl;
  //test STFT
  A.do_stft();
  cout<<"stft finished!"<<endl;
  A.stftcpx2double();
  cout<<"stft to double finished!"<<endl;

  //test PLCA
  A.plca_on_data();
 //predict
  A.get_comp_weight();
  cout<<"get comp weight finished!"<<endl;
  int lb=A.pred_max_weight();
  // double* pts=A.get_pts();
  cout<<"predicted label is "<<lb<<endl;
  return 0;
}
