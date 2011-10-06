#ifndef _WAV_SEP_H
#define _WAV_SEP_H
/*
  this function is used to separate out a part from the whole wav file.
  filename: input wav filename
  start: the start second
  last: duration(second), if start+last>the length of the wav, last will be truncated into length-start
  fname_out: outfile name. By default, it is formatted as inname_start_last.wav
 */
void wav_sep(const char* filename, double start, double last, const char* fname_out=0);
#endif
