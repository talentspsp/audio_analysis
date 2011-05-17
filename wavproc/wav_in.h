/******************* FILE : wav_in.h *********************/


#ifndef INCLUDE_WAV_IN
#define INCLUDE_WAV_IN

class WAV_OUT;

class WAV_IN
{

public:

	WAV_IN(char *wav_file_name);	 
	WAV_IN(char *file_name,int quiet_flag);

	~WAV_IN();
	
	// routine for reading one sample from a (previously loaded) wave file
	//  returns current sample as a double 
	double read_current_input();
	int read_current_input(double *left,double *right);

	// determines end-of-file condition, returns 1==true if more data ready
	int more_data_available();

	// seeks start of array, to reread stream
	void reset_reread();
	int skip_ahead(double secs);
	int skip_ahead(int nsamp);

	// returns number of samples in file
	long int get_num_samples();
	long int get_num_samples_stereo_pairs();
	long int get_num_samples_mono();

	// reports number of channels (1==mono, 2==stereo)
	int get_num_channels();

	// reports the number of bits in each sample
	int get_bits_per_sample();

	// reports sample rate in Hz
	double get_sample_rate_hz();

	int get_index();
	
protected:
		
	int construct(char *file_name,int quiet_flag);

	FILE *fw;

	double fs_hz;
	int bits_per_sample;
	int num_ch;
	int dlen;

	int g_num_isamp;
	long int g_max_isamp;

	long int fpos_start;

	friend class WAV_OUT;
};

#endif
