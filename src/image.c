
#include <stdlib.h>

#include "comm.h"
#include "types.h"
#include "image.h"


e_ret hrscl_create_image( uint32 width, uint32 height, s_pixbuf* img ) {

	img->width = width;
	img->height = height;

	img->pixels = malloc( height * sizeof(img->px) );
	if ( img->pixels == NULL ) {
		trace( TRACE_ERROR, "Cannot allocate row pointers for image (%u:%u)\n", width, height );
		return ERR_MEMALLOC;
	}

	img->bytes = calloc( height*width, sizeof(*img->px));
	if ( img->pixels == NULL ) {
		trace( TRACE_ERROR, "Cannot allocate pixbuf for image (%u:%u)\n", width, height );
		return ERR_MEMALLOC;
	}

	for ( uint32 r = 0; r < height; r++ )
		img->pixels[r] = (s_pixel*)( img->bytes + r*(width * sizeof(*img->px)) );

	return NO_ERR;
}

void hrscl_free_image( s_pixbuf* img ) {

	free( img->pixels );
	free( img->bytes );
	img->width = 0;
	img->height = 0;
}

