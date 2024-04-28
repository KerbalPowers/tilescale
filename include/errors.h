
#ifndef __HRSCL_ERRORS_H__
#define __HRSCL_ERRORS_H__

/*
-------------------------------------------------------------------------------
	Common Errors
-------------------------------------------------------------------------------
*/

typedef enum {
	NO_ERR						= 0x0000,	/* No Errors					*/
	ERR_UNKNOWN					= 0xFFFF,	/* Unknown error 				*/

	ERR_NOT_IMPLEMENTED			= 0xFF01,	/* Not implemented				*/
	ERR_BAD_PARAMS				= 0xFF02,	/* Bad param (value)			*/
	ERR_USAGE					= 0xFF03,	/* Incorrect usage				*/
	ERR_MEMALLOC				= 0xFF04,	/* Memory allocation error		*/
	ERR_SKILL_ISSUE				= 0xDEAD,	/* Unspecified internal error	*/

	ERR_LOAD_FILE_IO			= 0x0101,	/* Can't read file				*/
	ERR_LOAD_FILE_TYPE			= 0x0102,	/* Unsupported file type		*/
	ERR_LOAD_PNG				= 0x0103,	/* libpng error loading file	*/
} e_ret;

#endif /* __HRSCL_ERRORS_H__ */