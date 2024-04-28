

#include <stdio.h>
#include <unistd.h>

#include "types.h"
#include "comm.h"
#include "version.h"
#include "image.h"
#include "upscalers.h"
#include "load_png.h"


int main( int argc, char* argv[] ) {
	uint8 err = NO_ERR;
	s_pixbuf image, out;

	printf( HRSCL_NAME " " HRSCL_VERSION "\n" );
	if ( argc < 3 && access(argv[1], F_OK) == 0 ) {
		trace( TRACE_ERROR, "Usage: tilescale <input> <output>\n" );
		return ERR_USAGE;
	}

	err = hrscl_load_png( argv[1], &image );
	if ( err != NO_ERR ) return err;

	err = hrscl_create_image( image.width*3, image.height*3, &out );
	if ( err != NO_ERR ) return err;

	err = hrscl_upscale3x( &image, &out );

	hrscl_save_png( &out, argv[2] );
	
	hrscl_free_image( &image );
	hrscl_free_image( &out );
	return NO_ERR;
}