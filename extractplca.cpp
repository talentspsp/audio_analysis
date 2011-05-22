#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include "audio_analyzer.h"
#include "matfile.h"
#include <sstream>
using namespace std;

/*
  this program is used to extract components of different songs, wav->STFT->plca->save P(f|z),Pt(z),p(z)
  input:
  genredir: the directory containing all the genres
  listfile: list the paths to all the wav files, format should be genre/wavfile
  savedir: the path to the directory to save the results
  
 */

void parseline(const string& line, string& genre,  string& name, string& ext)
{
  size_t pos=0,posn;
  string wavfile;
  posn=line.find("/",pos);
  genre=line.substr(pos,posn-pos);
  pos=posn+1;
  posn=line.find("/",pos);
  wavfile=line.substr(pos,posn-pos);
  pos=wavfile.find_last_of(".");
  name=wavfile.substr(0,pos);
  ext=wavfile.substr(pos+1);
  
}
int main(int argc, char** argv)
{
  if(argc<4)
    {
      cerr<<"Usage: extractplca genredir listfile savedir"<<endl;
      return 1;
    }
 
  //set up audio_analyzer
  audio_analyzer A;
  A.set_fft_param(4096,4096,1024);
  A.set_sparsez(0.2);
  A.set_thd_db(-4.5);
  //A.set_thd_pz(0);
  A.set_numcomp_per_seg(20);
  A.set_max_iter_plca(100);
  A.set_max_itertau(2);
  //A.set_max_iter_cw(100);
  A.set_seglen_sec(30); //30s second segments

  //parse the list file and read in the files
  ifstream fl(argv[2]); //open the file list
  if(!fl)
    {
      cerr<<"cannot open file "<<argv[1]<<endl;
      return 1;
    }
  string line,genre,name,ext;
  string cmd; //string used to save command
  cmd.clear();
  cmd+="mkdir -p ";
  cmd+=argv[1];
  system(cmd.c_str());
  ostringstream oss;
  while(getline(fl,line))
    {
      oss.str("");
      oss<<argv[1]<<"/"<<line;
      cmd=oss.str();
      A.readindata(cmd.c_str());
      A.do_stft();
      A.stftcpx2double();
      A.plca_on_data();
      parseline(line, genre, name,ext);
      oss.str("");
      oss<<"mkdir -p "<<argv[3]<<"/"<<genre<<"/"<<name;
      cmd=oss.str();
      system(cmd.c_str());
      oss.str("");
      oss<<argv[3]<<"/"<<genre<<"/"<<name<<"/"<<name<<"_W.bin"; //construct save file
      cmd=oss.str();
      mat2bin(cmd.c_str(), A.get_numcomp_all(),A.get_lenf(), A.get_pfz());
      oss.str("");
      oss<<argv[3]<<"/"<<genre<<"/"<<name<<"/"<<name<<"_Z.bin"; //construct save file
      cmd=oss.str();
      mat2bin(cmd.c_str(),A.get_numseg(),A.get_numcomp_per_seg(),A.get_pz());
    }
  fl.close();
  return 0;
}


