
// DISTRIBUTED UNDER THE CONDITIONS OF THE GNU GENERAL PUBLIC LICENSE, SUBJECT TO
// THE CONDITION THAT CREDIT MUST BE GIVEN TO THE ORIGINAL AUTHOR, DR. FRED DEPIERO
// THIS NOTICE MUST REMAIN UNALTERED

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "wav_def.h"
#include "wav_out.h"
#include "wav_in.h"

#include "exit_msg.h"


int main (int argc, char * const argv[])
{
	// banner
	printf("ready for wav io...\n");

	// setup files names for in/out
	char *fname_in = (char *) "03a02Wb.wav";
	char *fname_out = (char *) "out.wav";

	// allocate input wav object	
	WAV_IN *wav_in = new WAV_IN(fname_in);
	if(wav_in==NULL) exit_msg(-682,"cant allocate wav_in");
	
	
	
	// access file info and display
	int num_channels = wav_in->get_num_channels();
	if(num_channels != 1){ printf("sorry this simple demo only works for mono WAV files :(\n"); exit(0); }
	
	int num_samples = wav_in->get_num_samples();

	double fs_hz = wav_in->get_sample_rate_hz();
	printf("Sample rate for mono WAV file is %1.0lf Hz (%d samples)\n",fs_hz,num_samples);
	
	
	
	// create output wav object, mimicing style of input (fs...)
	WAV_OUT *wav_out = new WAV_OUT(fname_out,wav_in);
	if(wav_out==NULL) exit_msg(-682,"cant allocate wav_out");
	
	
	// access input and generate a low pass version for output file
	double in_prev = 0.0;
	double in_prev2 = 0.0;
	for(int i=0;i<num_samples;i++)
	{
		double in_curr = wav_in->read_current_input();
		
		double out = 0.33 * in_curr + 0.33 * in_prev + 0.33 * in_prev2;
		in_prev2 = in_prev;
		in_prev = in_curr;
		
		wav_out->write_current_output(out);
	}
	
	wav_out->close_wave_file();
	
	
	// clean  up
	delete wav_in;
	delete wav_out;
    return 0;
}
