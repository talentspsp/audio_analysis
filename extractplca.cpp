#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include "audio_analyzer.h"
#include "matfile.h"
using namespace std;

/*
  this program is used to extract components of different songs, wav->STFT->plca->save P(f|z),Pt(z),p(z)
  input:
  listfile: list the full paths to all the wav files
  savedir: the path to the directory to save the results
  
 */
int main(int argc, char** argv)
{
  if(argc<3)
    {
      cerr<<"Usage: extractplca listfile savedir"<<endl;
      return 1;
    }
  ifstream wavfile(argv[1]);
  if(!wavfile)
    {
      cerr<<"Error: cannot open file "<<argv[1]<<endl;
      return -1;
    }
  string line;
  //set up audio_analyzer
  audio_analyzer AA;
  A.set_fft_param(4096,4096,1024);
  A.set_sparsez(0.2);
  //A.set_thd_db(-4.5);
  //A.set_thd_pz(0);
  A.set_numcomp(20);
  A.set_max_iter_plca(100);
  A.set_max_itertau(2);
  //A.set_max_iter_cw(100);
  A.set_seglen(0); //TODO: decide seg length
}


