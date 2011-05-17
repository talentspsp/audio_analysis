/****************** FILE : exit_msg.cpp ******************/

/****** NOTICE: LIMITATIONS ON USE AND DISTRIBUTION ********\

  This software is provided on an as-is basis, it may be 
  distributed in an unlimited fashion without charge provided
  that it is used for scholarly purposes - it is not for 
  commercial use or profit. This notice must remain unaltered. 

  Software by Dr Fred DePiero - CalPoly State University

\******************** END OF NOTICE ************************/


//// uncomment this for stdio


#define PC_STDIO
// #define PC_MFC

// #define WAIT_ON_ERROR


//// uncomment this for MFC mode, also
// #include <stdafx.h>
// #include <stdlib.h>



#ifdef PC_STDIO

#include <stdio.h>
#include <stdlib.h>

#endif



int fwd_errno = 0;

extern "C" 
{
	int check_fwd_errno();
	int exit_msg(int eval,const char *emsg);
	void yield_to_background();
	void os_delay(double sec);
}



/********************************************\
\********************************************/
int check_fwd_errno()
{
	return fwd_errno;
}


/********************************************\
\********************************************/
int exit_msg(int eval,const char *emsg)
{

	fwd_errno = eval;

	fflush(stdout);

#ifdef PC_STDIO

	printf("exiting! (%d), %s\n",eval,emsg);

	FILE *fp;
	fp = fopen("depiero_log.txt","a");
	if(fp!=NULL)
	{
		fprintf(fp,"exiting! (%d), %s\n",eval,emsg);
		fclose(fp);
	}

#ifdef WAIT_ON_ERROR
//	getchar();
#endif

	exit(eval);

#endif


#ifdef PC_MFC

	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	AfxMessageBox(emsg,MB_OK,(UINT)0);

	FILE *fp;
	fp = fopen("depiero_log.txt","a");
	if(fp!=NULL)
	{
		fprintf(fp,"exiting! (%d), %s\n",eval,emsg);
		fclose(fp);
	}

	exit(eval);

#endif

	return(0);
	
/* int exit_msg(int eval,char *emsg) */}



#ifdef PC_MFC

void yield_to_background()
{
	MSG win_msg;
	LONG lIdle;

	// message pump 
	while(::PeekMessage( &win_msg, NULL, 0, 0, PM_NOREMOVE ) )
		AfxGetApp()->PumpMessage();

	// let MFC do its idle processing
	lIdle = 0;
	while( AfxGetApp()->OnIdle(lIdle++ ) ) ;

	return;
}

void os_delay(double sec)
{
	Sleep((long)(sec * 1000.0));
	return;
}

#endif



