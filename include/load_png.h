

#ifndef __HRSCL_LOAD_PNG__
#define __HRSCL_LOAD_PNG__

#include <png.h>
#include "types.h"

typedef struct userinfo {
	char* path;
	jmp_buf jmpbuf;
	s_pixbuf* img;
} t_userinfo;

e_ret hrscl_load_png( char* filename, s_pixbuf* buf );
e_ret hrscl_save_png( s_pixbuf* img, char* filename );

#endif /* __HRSCL_LOAD_PNG__ */