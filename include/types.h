
#ifndef __HRSCL_TYPES_H__
#define __HRSCL_TYPES_H__

#include <stdint.h>
#include "errors.h"

#define __FUCK_OFF(def) #def
#define _TOSTRING(def) __FUCK_OFF(def)

/*
-------------------------------------------------------------------------------
	Common Types
-------------------------------------------------------------------------------
*/

typedef int8_t			int8;
typedef uint8_t			uint8;
typedef int16_t			int16;
typedef uint16_t		uint16;
typedef int32_t			int32;
typedef uint32_t		uint32;
typedef int64_t			int64;
typedef uint64_t		uint64;

typedef enum e_bool {
	FALSE = 0,
	TRUE  = 1
} bool;

/*
-------------------------------------------------------------------------------
	Image raster
-------------------------------------------------------------------------------
*/

typedef union {
	uint32	val;
	struct {
		uint8 r;
		uint8 g;
		uint8 b;
		uint8 a;
	};
} s_pixel;

typedef struct pixbuf {
	uint32 width;
	uint32 height;
	int    depth;
	s_pixel** pixels;
	union {
		uint8*	 bytes;
		s_pixel* px;
	};
} s_pixbuf;

#endif /* __HRSCL_TYPES_H__ */

