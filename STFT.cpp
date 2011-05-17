/*
 *  STFT.cpp
 *  audio_analysis
 *
 *  Created by Qingyuan Kong on 4/16/11.
 *  Copyright 2011 Dartmouth. All rights reserved.
 *
 */

#include "STFT.h"
#include "spuc/generic/spuc.h"
#include "spuc/generic/cplxfft.h"
#include <cmath>
#include <cstring>  //use memcpy in it
#include "math_util.h"
using namespace SPUC;

complex<double>* STFT(complex<double>* buf, int bufsize, int fftN, int fftW, int fftH, int& lenf, int& lent)
{
  lenf=fftN/2+1;
  lent=(int)((bufsize-fftN)/fftH)+1;
  complex<double>* r=new complex<double>[lent*lenf]; // r is the result matrix, each row only contain the first half of fft
  //construct the hann window
  //if even, change fftW to odd
  if(fftW%2==0)
    fftW++;
  complex<double>* w=new complex<double>[fftN];
  complex<double>* temp=new complex<double>[fftN];
  int i;
  for(i=0;i<fftN;i++)
    {
      w[i].re=0;
      w[i].im=0;
    }
  
  int halfw=(fftW-1)/2;
  int halff=fftN/2;
  for(i=MAX(halff-halfw,0);i<fftN-MAX(halff-halfw,0);i++)
    w[i].re=0.5*(1+cos(PI*(i-halff)/halfw)); //this is the hann window
  
  cfft< complex<double> > fftobj(fftN);
  for(i=0;i<lent;i++)
    {
      memcpy(temp,buf+i*fftH,fftN*sizeof(complex<double>));
      ew_mult(temp, temp, w, 1, fftN); //multiply the window
      fftobj.fft(temp);
      //copy the first half of data to the result
      memcpy(r+i*lenf,temp,lenf*sizeof(complex<double>));
    }
  return r;

}
