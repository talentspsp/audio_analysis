/*************** FILE : wav_in.cpp *******************/


// DISTRIBUTED UNDER THE CONDITIONS OF THE GNU GENERAL PUBLIC LICENSE, SUBJECT TO
// THE CONDITION THAT CREDIT MUST BE GIVEN TO THE ORIGINAL AUTHOR, DR. FRED DEPIERO
// THIS NOTICE MUST REMAIN UNALTERE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>

#include "exit_msg.h"

#include "wav_def.h"
#include "wav_in.h"

using namespace std;


/********************************************\
\********************************************/
long int WAV_IN::get_num_samples_stereo_pairs()
{ 
  	 if(num_ch!=2) exit_msg(-233,"wav file does not contain stereo data");
  	 return g_max_isamp / num_ch; 
}
long int WAV_IN::get_num_samples_mono()
{ 
  	 if(num_ch!=2) exit_msg(-233,"wav file does not contain stereo data");
  	 return g_max_isamp; 
}

long int WAV_IN::get_num_samples(){ return g_max_isamp; }



/********************************************\
\********************************************/
int WAV_IN::get_num_channels(){ return num_ch; }

/********************************************\
\********************************************/
int WAV_IN::get_index(){ return g_num_isamp; }

/********************************************\
\********************************************/
int WAV_IN::get_bits_per_sample(){ return bits_per_sample; }

/********************************************\
\********************************************/
double WAV_IN::get_sample_rate_hz(){ return fs_hz; }



/********************************************\
\********************************************/
void WAV_IN::reset_reread()
{ 
	g_num_isamp = 0;
	fseek(fw, fpos_start, SEEK_SET);

	return; 
}



/********************************************\
\********************************************/
int WAV_IN::skip_ahead(double secs)
{ 
	int nsamp = (int)(secs * fs_hz);
	return skip_ahead(nsamp); 
}


/********************************************\
\********************************************/
int WAV_IN::skip_ahead(int nsamp)
{
	// get file demographics
	int bits_per_sample = this->get_bits_per_sample();
	int bytes_per_sample = bits_per_sample / 8;
	bytes_per_sample *= this->get_num_channels();

	// find file offset, from current position
	unsigned int soffset = (unsigned int)( nsamp * bytes_per_sample );

	unsigned int wstat = fseek(fw,soffset,SEEK_CUR);
	if(wstat!=0) exit_msg(-377,"cant fseek test file");

	return 0; 
}





/********************************************\
\********************************************/
int WAV_IN::more_data_available()
{ 
	if( g_num_isamp >= get_num_samples() )
	{
	 	printf("no more data available! (index,max) = (%d,%ld)\n",g_num_isamp,get_num_samples());
        return 0;
    }
    
	return 1;
}



/**********************************************************
**********************************************************/
double WAV_IN::read_current_input()
{ 
	if( (fw==NULL) || (g_num_isamp<0) )
	  throw(runtime_error("input file not ready (or not loaded)"));

	if( g_num_isamp == get_num_samples() )
	{
	   printf("warning: near file limit at sample %d\n",g_num_isamp);
 	   g_num_isamp += 1; 
	   return 0.0; 
    }
	else if( g_num_isamp > get_num_samples() )
	{
	   printf("error: file limit reached at sample %d\n",g_num_isamp);
	   throw(runtime_error("attempt to read past end of input WAV file!\n")); 
 	   g_num_isamp += 1; 
	   return 0.0; 
    }
    

	unsigned int wstat;
	int wbuff_len;
	short int *uptr;
	unsigned char *cptr;
	char wbuff[16];
	double wout = 0.0;
	
	// convert data
	if(bits_per_sample == 16)
	{
		wbuff_len = 2;
		wstat = fread((void *)wbuff,wbuff_len,(size_t)1,fw);
		if(wstat!=1) throw(runtime_error("cant read 16-bit WAV file"));

		uptr = (short *) wbuff;
		wout = (double) (*uptr);
	}
	else
	{
		wbuff_len = 1;
		wstat = fread((void *)wbuff,wbuff_len,(size_t)1,fw);
		if(wstat!=1) throw(runtime_error("cant read WAV file"));

		cptr = (unsigned char *) wbuff;
		wout = (double) (*cptr);
	}

	g_num_isamp += 1;

	return wout;
}




/**********************************************************
**********************************************************/
int WAV_IN::read_current_input(double *left,double *right)
{
	*left = read_current_input();
	*right = read_current_input();

	return 0;
}




/********************************************\
\********************************************/
int WAV_IN::construct(const char *file_name,int quiet_flag)
{	
	unsigned int wstat;
	int wbuff_len;
	int i;
	char obuff[80];

	WAV_HDR *wav;
	CHUNK_HDR *chk;
	int sflag;
	long int rmore;

	// set defaults
	g_num_isamp = 0;
	g_max_isamp = 0;

	// allocate wav header
	wav = new WAV_HDR;
	chk = new CHUNK_HDR;
	if(wav==NULL) throw(runtime_error("cant new headers"));
	if(chk==NULL) throw(runtime_error("cant new headers"));

	/* open wav file */
	fw = fopen(file_name,"rb");
	if(fw==NULL)
	{	
		printf("cant open wav file: %s\n",file_name);
		throw(runtime_error("cant open wav file"));
	}
	
	/* read riff/wav header */
	wstat = fread((void *)wav,sizeof(WAV_HDR),(size_t)1,fw);
	if(wstat!=1) throw(runtime_error("cant read wav"));
	
	
	/*
	printf("WAV Header:\n");
	char *cptr = (char *) &(wav[0]);
	for(i=0;i<sizeof(WAV_HDR);i++)
	{
		printf("[%2d] %c (0x%2x) \n",i,cptr[i],(int)cptr[i]);
	}
	printf("\n");
	*/
	

	// check format of header
	for(i=0;i<4;i++) obuff[i] = wav->rID[i];
	obuff[4] = 0;
	if(strcmp(obuff,"RIFF")!=0) throw(runtime_error("bad RIFF format"));
	
	for(i=0;i<4;i++) obuff[i] = wav->wID[i];
	obuff[4] = 0;
	if(strcmp(obuff,"WAVE")!=0) throw(runtime_error("bad WAVE format"));

	for(i=0;i<3;i++) obuff[i] = wav->fId[i];
	obuff[3] = 0;
	if(strcmp(obuff,"fmt")!=0) throw(runtime_error("bad fmt format"));

	if(wav->wFormatTag!=1) throw(runtime_error("bad wav wFormatTag"));
		
	if( (wav->nBitsPerSample != 16) && (wav->nBitsPerSample != 8) )
	  throw(runtime_error("bad wav bit depth"));


	// skip over any remaining portion of wav header
	rmore = wav->pcm_header_len - (sizeof(WAV_HDR) - 20);
//	if(rmore!=0) printf("warning: rmore = %d\n",rmore);

	wstat = fseek(fw,rmore,SEEK_CUR);
	if(wstat!=0) throw(runtime_error("cant seek"));
	

	// read chunks until a 'data' chunk is found
	sflag = 1;
	while(sflag!=0){

		// check attempts
	  if(sflag>10) throw(runtime_error("too many chunks"));
		
		// read chunk header
		wstat = fread((void *)chk,sizeof(CHUNK_HDR),(size_t)1,fw);
		if(wstat!=1) throw(runtime_error("cant read chunk"));
		
		// check chunk type
		for(i=0;i<4;i++) obuff[i] = chk->dId[i];
		obuff[4] = 0;
		if(strcmp(obuff,"data")==0) break;
		
		// skip over chunk
		sflag++; 
		wstat = fseek(fw,chk->dLen,SEEK_CUR);
		if(wstat!=0) throw(runtime_error("cant seek"));
	}

	/* find length of remaining data */
	wbuff_len = chk->dLen;

	// find number of samples
	g_max_isamp = chk->dLen;
	g_max_isamp /= (wav->nBitsPerSample / 8);


	// save demographics
	fs_hz = (double) (wav->nSamplesPerSec);
	bits_per_sample = wav->nBitsPerSample;
	num_ch = wav->nChannels;
	dlen = chk->dLen;
	
	if(quiet_flag==0)
	{
		printf("\nLoaded WAV File: %s\n",file_name);
		printf(" Fs = %1.0lf Hz,",fs_hz);
		if(wav->nChannels == 2)
			printf(" # Samples (Readable as Stereo Pairs) = %ld\n",get_num_samples()/get_num_channels());
		else
			printf(" # Samples (Mono) = %ld,",get_num_samples());
		printf(" Bits Per Sample = %d\n",bits_per_sample);
		// printf(" Number of Channels: %d\n\n",get_num_channels());
	}

	// reset buffer stream index
	g_num_isamp = 0;

	// get starting position in file for WAV data
	fpos_start = ftell(fw);

	// be polite - clean up
	if(wav!=NULL) delete wav;
	if(chk!=NULL) delete chk;

	return 0;

/* int WAV_IN::construct(char *file_name,int quiet_flag) */}





/********************************************\
\********************************************/
WAV_IN::WAV_IN(const char *file_name,int quiet_flag)
{	
	construct(file_name,quiet_flag);

	return;
}


/********************************************\
\********************************************/
WAV_IN::WAV_IN(const char *file_name)
{	
	construct(file_name,1);

	return;
}



/********************************************\
\********************************************/
WAV_IN::~WAV_IN()
{ 
	fclose(fw);
}




