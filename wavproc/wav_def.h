/******************* FILE : wav_def.h ********************/


// DISTRIBUTED UNDER THE CONDITIONS OF THE GNU GENERAL PUBLIC LICENSE, SUBJECT TO
// THE CONDITION THAT CREDIT MUST BE GIVEN TO THE ORIGINAL AUTHOR, DR. FRED DEPIERO
// THIS NOTICE MUST REMAIN UNALTERED




// header of wav file
typedef struct{
	char rID[4];				// 'RIFF'	
	int rLen;
	char wID[4];				// 'WAVE'	
	char fId[4];				// 'fmt '
	
	int pcm_header_len;			// content varies...
	short int wFormatTag;
	
	short int nChannels;		// 1,2 for stereo data is (l,r) pairs
	int nSamplesPerSec;
	int nAvgBytesPerSec;
	short int nBlockAlign;		
	short int nBitsPerSample;
	
}	WAV_HDR;

	
// header of wav file
typedef struct{
	char dId[4];				// 'data' or 'fact'
	int dLen;
}	CHUNK_HDR;


