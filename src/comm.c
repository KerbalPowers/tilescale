/****************************************************************************************/
/*																						*/
/*							Common Functions and Structures								*/
/*																						*/
/*	Author	: HerrCrazi																	*/
/*	Date 	: 26/04/2024																*/
/*																						*/
/****************************************************************************************/

#include "comm.h"


/* Trace labels for 'trace()' macro */
char* COMM_szTraceLabels[ NB_TRACE_LEVELS ] = {
	"DEBUG",	/* TRACE_DEBUG */
	"INFO",		/* TRACE_INFO */
	"WARN",		/* TRACE_WARN */
	"ERROR",	/* TRACE_ERROR */
	""			/* TRACE_NONE */
};