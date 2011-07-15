/******************* FILE : wav_out.h *********************/


#ifndef INCLUDE_WAV_OUT
#define INCLUDE_WAV_OUT

class WAV_IN;


class WAV_OUT
{

public:

	// create a new wav_out with given parameters
	//  note: soundcards typically support a limited range of values!
	//        hence the next constructor is safer: WAV_OUT(WAV_IN *wav);
	WAV_OUT(const char *fname,double fs_hz,int bits_per_sample,int num_ch);
	
	// create a wav_out with the same parameters as a given wav_in
	WAV_OUT(const char *fname,WAV_IN *wav_in);	

	~WAV_OUT();
	
	// routine for writing output sample
	//  samples are stored in actual WAV file, but with an arbitrary length (1 sec)
	//  when save_wave_file() is called, the file is closed and length corrected
	//  returns 1 on success 
	int write_current_output(double ooo);
	int write_current_output(double left,double right);

	int close_wave_file();

protected:
	
	// creates file and writes header (arbitrary length set to 1 sec)
	int create_wave_file(const char *fname);

	char *fname_copy;

	// routine for saving a wave file. 
	//  closes file, reopens, and corrects file length in header
	//  returns 1 on success 
	int save_wave_file(const char *fname);

	FILE *fw;

	int header_offset_rlen;
	int header_offset_dlen;

	double fs_hz;
	int bits_per_sample;
	int num_ch;

	int g_num_osamp;

	friend class WAV_IN;
};

#endif
