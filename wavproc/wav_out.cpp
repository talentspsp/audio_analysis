/******************* FILE : wav_out.cpp ********************/

// DISTRIBUTED UNDER THE CONDITIONS OF THE GNU GENERAL PUBLIC LICENSE, SUBJECT TO
// THE CONDITION THAT CREDIT MUST BE GIVEN TO THE ORIGINAL AUTHOR, DR. FRED DEPIERO
// THIS NOTICE MUST REMAIN UNALTERED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exit_msg.h"

#include "wav_def.h"
#include "wav_out.h"
#include "wav_in.h"




/**********************************************************
**********************************************************/
int WAV_OUT::close_wave_file()
{
	return save_wave_file(fname_copy);
}




/**********************************************************
**********************************************************/
int WAV_OUT::save_wave_file(const char *fname)
{
	unsigned int wstat;
	int wbuff_len;
	int rlen_data;
	int dlen_data;

	if(fw==NULL) return 0;
	if(g_num_osamp<=0) exit_msg(-980,"warning, no new data written to output");

	// find correct lengths
	wbuff_len = g_num_osamp * bits_per_sample / 8;

	rlen_data = sizeof(WAV_HDR) + sizeof(CHUNK_HDR) + wbuff_len;
	dlen_data = wbuff_len;


	// close and re-open file
	fclose(fw);

	fw = fopen(fname,"rb+");
	if(fw==NULL) exit_msg(-372,"cant open wav file to finalize via read/write");


	
	// seek file positions and overwrite length data
	wstat = fseek(fw,header_offset_rlen,SEEK_SET);
	if(wstat!=0) exit_msg(-377,"cant fseek test file");

	wstat = fwrite((void *)&rlen_data,sizeof(int),1,fw);
	if(wstat!=1) exit_msg(-371,"cant re-write header (1)");
	

	wstat = fseek(fw,header_offset_dlen,SEEK_SET);
	if(wstat!=0) exit_msg(-377,"cant fseek test file");

	wstat = fwrite((void *)&dlen_data,sizeof(int),1,fw);
	if(wstat!=1) exit_msg(-371,"cant re-write header (2)");


	
	// close file, final
	fclose(fw);

/*
	printf("\nCreated WAV File: %s\n",fname);
	printf(" Fs = %1.0lf Hz,",fs_hz);
    if(num_ch==2)
	    printf(" # Samples (Saved as Stereo Pairs) = %d,",g_num_osamp/num_ch);
    else
	    printf(" # Samples (Mono) = %d,",g_num_osamp/num_ch);
	printf(" Bits Per Sample = %d\n",bits_per_sample);
*/

	// reset output stream index
	g_num_osamp = 0;
	fw = NULL;

	return 1;

/* int WAV_OUT::save_wave_file(char *fname) */}






/**********************************************************
**********************************************************/
int WAV_OUT::write_current_output(double left,double right)
{
	if(this->num_ch!=2)
		exit_msg(-587,"attempt to write to a mono file with stereo-write method");

	int wstat;

	this->num_ch = 1;	
	write_current_output(left);
	wstat = write_current_output(right);
	this->num_ch = 2;

	return wstat;
}





/**********************************************************
**********************************************************/
int WAV_OUT::write_current_output(double ooo)
{
	if(this->num_ch!=1)
		exit_msg(-581,"attempt to write to a stereo file with mono-write method");

	if(fw==NULL) exit_msg(-583,"not ready for WAV write");

	// convert data
	double ttt;
	double max_uuu =  (65536.0 / 2.0) - 1.0;
	double min_uuu = -(65536.0 / 2.0);
	double max_ccc = 256.0;
	double min_ccc = 0.0;

	int wstat;
	short int sidata;
	unsigned char ucdata;
	if(bits_per_sample == 16)
	{
		ttt = ooo;
		if(ttt>max_uuu) ttt = max_uuu;
		if(ttt<min_uuu) ttt = min_uuu;
		sidata = (short int) (ttt + 0.5);

		wstat = fwrite((void *)(&sidata),sizeof(short int),(size_t)1,fw);
		if(wstat!=1) exit_msg(-376,"cant write WAV_OUT");
	}

	else if(bits_per_sample == 8)
	{
		ttt = ooo;
		if(ttt>max_ccc) ttt = max_ccc;
		if(ttt<min_ccc) ttt = min_ccc;
		ucdata = (unsigned char) (ttt + 0.5);

		wstat = fwrite((void *)(&ucdata),sizeof(unsigned char),(size_t)1,fw);
		if(wstat!=1) exit_msg(-376,"cant write WAV_OUT");
	}
	else exit_msg(-376,"bunk bits_per_sample");


	// another sample saved
	g_num_osamp += 1;

	// be polite
	return 1;

/* int WAV_OUT::write(double ooo) */}






/**********************************************************
**********************************************************/
int WAV_OUT::create_wave_file(const char *fname)
{
	unsigned int wstat;
	int i;
	char obuff[80];

	WAV_HDR *wav;
	CHUNK_HDR *chk;
	int wbuff_len;


	// allocate wav header
	wav = new WAV_HDR;
	chk = new CHUNK_HDR;
	if(wav==NULL) exit_msg(-376,"cant new headers");
	if(chk==NULL) exit_msg(-376,"cant new headers");

	// setup size - just assume 1 second length for now!
	wbuff_len = ((int) fs_hz) * bits_per_sample / 8;


	// setup wav header
	sprintf(obuff,"RIFF");
	for(i=0;i<4;i++) wav->rID[i] = obuff[i];

	sprintf(obuff,"WAVE");
	for(i=0;i<4;i++) wav->wID[i] = obuff[i];

	sprintf(obuff,"fmt ");
	for(i=0;i<4;i++) wav->fId[i] = obuff[i];

	wav->nBitsPerSample = bits_per_sample;
	wav->nSamplesPerSec = (int) fs_hz;
	wav->nAvgBytesPerSec = (int) fs_hz;
	wav->nAvgBytesPerSec *= bits_per_sample / 8;
	wav->nAvgBytesPerSec *= num_ch;
	wav->nChannels = num_ch;
	
	wav->pcm_header_len = 16;
	wav->wFormatTag = 1;
	wav->rLen = sizeof(WAV_HDR) + sizeof(CHUNK_HDR) + wbuff_len;
	wav->nBlockAlign = num_ch * bits_per_sample / 8;


	// setup chunk header
	sprintf(obuff,"data");
	for(i=0;i<4;i++) chk->dId[i] = obuff[i];

	chk->dLen = wbuff_len;


	// save (file) offsets for later size updates
//	header_offset_rlen = ((int) &(wav->rLen));
//	header_offset_rlen -= ((int) wav);

	header_offset_rlen = 4;
	
//	header_offset_dlen = ((int) &(chk->dLen)) - ((int) chk);
//	header_offset_dlen += sizeof(WAV_HDR);
	
	header_offset_dlen = sizeof(WAV_HDR) + 4;


	/* open wav file */
	fw = fopen(fname,"wb");
	if(fw==NULL) exit_msg(-371,"cant open wav file for create");

	/* write riff/wav header */
	wstat = fwrite((void *)wav,sizeof(WAV_HDR),(size_t)1,fw);
	if(wstat!=1) exit_msg(-376,"cant write wav");
	
	/* write chunk header */
	wstat = fwrite((void *)chk,sizeof(CHUNK_HDR),(size_t)1,fw);
	if(wstat!=1) exit_msg(-377,"cant write chk");
	

	// reset output stream index
	g_num_osamp = 0;

	// copy file name
	fname_copy = new char [strlen(fname)+1];
	if(fname_copy==NULL) exit_msg(-378,"cant create string buffer for fname");
	fname_copy[0] = 0;
	strcpy(fname_copy, fname);

	// be polite
	if(wav!=NULL) delete wav;
	if(chk!=NULL) delete chk;

	return 1;

/* int WAV_OUT::create_wave_file(char *fname) */}






/********************************************\
\********************************************/
WAV_OUT::WAV_OUT(const char *fname,double _fs_hz,int _bits_per_sample,int _num_ch)
{
	fs_hz = _fs_hz;
	bits_per_sample = _bits_per_sample;
	num_ch = _num_ch;

	g_num_osamp = 0;

	fw = NULL;
	create_wave_file(fname);

	return;
	
/* WAV_OUT::WAV_OUT(,,,) */}





/********************************************\
\********************************************/
WAV_OUT::WAV_OUT(const char *fname,WAV_IN *wav_in)
{
	fs_hz = wav_in->fs_hz;
	bits_per_sample = wav_in->bits_per_sample;
	num_ch = wav_in->num_ch;

	g_num_osamp = 0;

	fw = NULL;
	create_wave_file(fname);

	return;
	
/* WAV_OUT::WAV_OUT(,,,) */}



/********************************************\
\********************************************/
WAV_OUT::~WAV_OUT()
{ 
	save_wave_file(fname_copy);
}


