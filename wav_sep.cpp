#include <cstdio>
#include "wavproc/wav_in.h"
#include "wavproc/wav_out.h"
#include <stdexcept>
#include <sstream>
#include <string>
#include "wav_sep.h"
using namespace std;

void wav_sep(const char* filename, double start, double last, const char* fname_out)
{
  WAV_IN wav_in(filename);
  int num_channels = wav_in.get_num_channels();
  long num_samples = wav_in.get_num_samples();
  double fs_hz = wav_in.get_sample_rate_hz();
  long datasize=num_samples/num_channels;
  long startpoint=start*fs_hz;
  if(startpoint>=datasize)
    throw runtime_error("start time out of range!");
  long outsize;
  if((start+last)*fs_hz>datasize) //if the last time pass the end, truncate it
    outsize=datasize-startpoint;
  else
    outsize=last*fs_hz;
  string outname;
  //format the out file name
  if(fname_out != 0)
    outname=fname_out;
  else
    {
      string inname=filename;
      size_t pos=inname.find_last_of('.');
      if(pos==string::npos)
	throw runtime_error("input file format wrong: no extension name!");
      ostringstream oss;
      oss<<inname.substr(0,pos);
      oss<<'_'<<(int)start<<'-'<<(int)last<<".wav";
      outname=oss.str();
    }
  WAV_OUT wav_out(outname.c_str(),&wav_in);
  double dl,dr; //data in left and right channels
  long p=0;
  while(p<startpoint)
    {
      if(num_channels==1)
	wav_in.read_current_input();
      else if(num_channels==2)
	wav_in.read_current_input(&dl,&dr);
      else
	throw runtime_error("number of channels is greater than 2!");
      p++;
    }
  p=0;
  while(p<outsize)
    {
      if(num_channels==1)
	{
	  dl=wav_in.read_current_input();
	  wav_out.write_current_output(dl);
	}
      else
	{
	  wav_in.read_current_input(&dl,&dr);
	  wav_out.write_current_output(dl,dr);
	}
      p++;
    }
  wav_out.close_wave_file();
  
}
