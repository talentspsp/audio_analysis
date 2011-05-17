/*
 *  STFT.h
 *  audio_analysis
 *
 *  Created by Qingyuan Kong on 4/16/11.
 *  Copyright 2011 Dartmouth. All rights reserved.
 *
 */
#ifndef _STFT_H
#define _STFT_H

#include <cstddef>
#include "spuc/generic/cplxfft.h"
#include "spuc/generic/complex.h"
//#include "spuc/matrix/matrix.h"
using namespace SPUC;
/* this function implement STFT using cfft class of spuc
   INPUT:
   buf: the buffer to be processed
   bufsize: the size of the buffer
   fftN: number of fft points
   fftW: window size, hann window will be used
   fftH: hop size
   OUTPUT:
   it returns a complex matrix, each row is a time frame
   this function doesn't check the correctness of input
*/

complex<double>* STFT(complex<double>* buf, int bufsize, int fftN, int fftW, int fftH, int& lenf, int& lent);

#endif
