/****************** FILE : exit_msg.h **********************/

/****** NOTICE: LIMITATIONS ON USE AND DISTRIBUTION ********\

  This software is provided on an as-is basis, it may be 
  distributed in an unlimited fashion without charge provided
  that it is used for scholarly purposes - it is not for 
  commercial use or profit. This notice must remain unaltered. 

  Software by Dr Fred DePiero - CalPoly State University

\******************** END OF NOTICE ************************/



#ifndef _INCLUDE_EXIT_MSG
#define _INCLUDE_EXIT_MSG

// #define PC_STDIO

extern "C" 
{
	int check_fwd_errno();
	int exit_msg(int eval,const char *emsg);
	void yield_to_background();
	void os_delay(double sec);
}

#endif

