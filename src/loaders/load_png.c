

#include <png.h>
#include <setjmp.h>
#include <stdlib.h>

#include "types.h"
#include "comm.h"
#include "load_png.h"
#include "version.h"

#define PNG_BYTES_TO_CHECK 8



bool checkPNG( FILE** fp ) {

	uint8 buf[PNG_BYTES_TO_CHECK];

	/* Read in some of the signature bytes. */
	if ( fread(buf, 1, PNG_BYTES_TO_CHECK, *fp) != PNG_BYTES_TO_CHECK )
		return 0;

	// fseek( fp, 0, SEEK_SET );

	/* Compare the first PNG_BYTES_TO_CHECK bytes of the signature. */
	return (png_sig_cmp(buf, 0, PNG_BYTES_TO_CHECK) == 0);
}


e_ret hrscl_load_png( char* filename, s_pixbuf* img ) {

	png_bytep 	pPNGBuf;

	png_struct* pPNG;
	png_infop	pInfo;
	png_bytepp	pRows;	

	int depth, tColors, tInterlace;

	FILE* fp;

	
	/* Open file */
	if ( !(fp = fopen( filename, "r" )) )
		return ERR_LOAD_FILE_IO;
	
	/* Init PNG internal read struct */
	pPNG = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
	if ( pPNG == NULL ) {
		fclose( fp );
		return ERR_LOAD_PNG;
	}

	/* Init PNG image info struct */
	pInfo = png_create_info_struct( pPNG );
	if ( pInfo == NULL ) {
		fclose( fp );
		png_destroy_read_struct( &pPNG, NULL, NULL );
		return ERR_LOAD_PNG;
	}

	/* Diagnosis : simply a difference in skill */
	if ( setjmp(png_jmpbuf(pPNG)) ) {
		/* Clean your mess, baka */
		trace( TRACE_ERROR, "Internal libpng error (skill issue)\n" );
		png_destroy_read_struct( &pPNG, &pInfo, NULL );
		fclose( fp );
		return ERR_SKILL_ISSUE;
	}

	/* Read metadata info from file */
	png_init_io( pPNG, fp );

	png_read_info( pPNG, pInfo );
	png_get_IHDR( pPNG, pInfo, &(img->width), &(img->height), &img->depth, &tColors, &tInterlace, NULL, NULL );
	trace( TRACE_INFO, "PNG image info: %ux%u depth:%d colors:%#x interlace: %#x\n", img->width, img->height, img->depth, tColors, tInterlace );

	/* Allocate memory for image pixbuf */
	size_t rowbytes = png_get_rowbytes( pPNG, pInfo );
    uint8 nch = png_get_channels( pPNG, pInfo );
	
	img->pixels = malloc( img->height * sizeof(png_bytep) );
	img->bytes = malloc( img->height * rowbytes );
    if ( img->bytes == NULL ) {
		trace( TRACE_ERROR, "Cannot allocate pixbuf of size %lu for image (%u:%u)\n", img->height*rowbytes, img->width, img->height );
        png_destroy_read_struct( &pPNG, &pInfo, NULL );
        return ERR_LOAD_PNG;
    }

	/* Prepare row pointers */
	for ( uint32 i = 0; i < img->height; i++ ) {
		img->pixels[i] = (s_pixel*)(img->bytes + i*rowbytes);
	}
	
	/* Read whole image */
	png_read_image( pPNG, (png_bytepp)img->pixels );
	trace( TRACE_INFO, "Image loaded\n" );

	/* Free libpng structs */
	png_read_end( pPNG, NULL );
	png_destroy_read_struct( &pPNG, &pInfo, NULL );
	fclose( fp );

	return NO_ERR;
}

static void c_write_err( png_structp pPNG ) {
	t_userinfo* u = png_get_error_ptr( pPNG );
	trace( TRACE_ERROR, "Error writing PNG '%s' (%ux%u)\n", u->path, u->img->width, u->img->height );

	if ( u == NULL ) exit( ERR_UNKNOWN );
	else longjmp( u->jmpbuf, ERR_SKILL_ISSUE );
}

e_ret hrscl_save_png( s_pixbuf* img, char* filename ) {
	png_structp pPNG;
	png_infop pInfo;
	FILE* fp;
	t_userinfo u = { .img = img, .path = filename };

	pPNG = png_create_write_struct( PNG_LIBPNG_VER_STRING, &u, &c_write_err, NULL );
	if ( pPNG == NULL ) return ERR_LOAD_PNG;

	pInfo = png_create_info_struct( pPNG );
	if ( pInfo == NULL ) {
		png_destroy_write_struct( &pPNG, NULL );
		return ERR_LOAD_PNG;
	}

	/* Diagnosis : simply a difference in skill */
	if ( setjmp( u.jmpbuf ) ) {
		/* Clean your mess, baka */
		trace( TRACE_ERROR, "Internal libpng error (skill issue)\n" );
		png_destroy_write_struct( &pPNG, &pInfo );
		return ERR_SKILL_ISSUE;
	}

	/* Open file */
	if ( !(fp = fopen( filename, "wb+" )) )
		return ERR_LOAD_FILE_IO;
	
	png_init_io( pPNG, fp );

	/* Set PNG properties */
	png_set_IHDR( pPNG, pInfo, 
		img->width,
		img->height,
		8,
		PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);

	/* Set PNG metadata */
	png_time t;
	png_convert_from_time_t( &t, time(NULL) );
	png_set_tIME( pPNG, pInfo, &t );

	png_text text[2] = {
		{
			.compression = PNG_TEXT_COMPRESSION_NONE,
			.key = "Software",
			.text = HRSCL_NAME " " HRSCL_VERSION
		}
	};
	png_set_text( pPNG, pInfo, text, 2 );

	/* Write headers & info */
	png_write_info( pPNG, pInfo );

	/* Write image data */
	trace( TRACE_INFO, "Saving [--------------------] --%%", filename );
	int prct, last = 0;
	for ( uint32 r = 0; r < img->height; r++ ) {
		png_write_row( pPNG, (png_bytep)img->pixels[r] );
		
		prct = (r/(float)img->height) * 100;
		if ( prct != last ) {
			printf("\033[26D[");
			for( int i = 0; i < 20; i++ ) putchar( (i <= prct/5) ? '#' : '-' );
			printf("] %02d%%", prct);
			fflush(stdout);
			last = prct;
		}
	}
	putchar('\n');

	/* Finish IO and free */
	png_write_end( pPNG, pInfo );
	png_destroy_write_struct( &pPNG, &pInfo );
	fflush( fp );
	fclose( fp );
	trace( TRACE_INFO, "Image saved to '%s'\n", filename );

	return NO_ERR;
}