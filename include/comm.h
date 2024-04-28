
#ifndef __HRSCL_COMM__
#define __HRSCL_COMM__

#include <stdio.h>
#include "types.h"

#define trace( tracelevel, format, ...) printf("[%s:%d] %s " format, __FUNCTION__, __LINE__, COMM_szTraceLabels[tracelevel], ##__VA_ARGS__)


typedef enum e_tracelevels {
	TRACE_DEBUG,
	TRACE_INFO,
	TRACE_WARN,
	TRACE_ERROR,
	TRACE_NONE,
	NB_TRACE_LEVELS
} t_tracelevel;

extern char* COMM_szTraceLabels[ NB_TRACE_LEVELS ];

#endif /* __HRSCL_COMM__ */