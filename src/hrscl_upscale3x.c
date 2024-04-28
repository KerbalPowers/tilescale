/****************************************************************************************/
/*																						*/
/*									hrscl_upscale3x										*/
/*																						*/
/*	Upscaler that aims to preserve original colors (no interpolation) and shapes, from	*/
/*	Scale3x/ScaleNs shader implementations by Sp00kyFox									*/
/*	Targeted for use on GIS rasters														*/
/*																						*/
/*	Author	: HerrCrazi																	*/
/*	Date 	: 25/01/2024																*/
/*																						*/
/****************************************************************************************/


#include "upscalers.h"
#include "comm.h"

#define PX_BLANK 0x000000
#define PX_FILL	 px_in->pixels[y][x].val



e_ret hrscl_upscale3x(s_pixbuf* restrict px_in, s_pixbuf* restrict px_out) {

	int prct, last;
	trace( TRACE_INFO, "Upscaling [--------------------] --%%" );

	for ( uint32 register y = 0 ; y < px_in->height ; y++ ) {
		for ( uint32 register x = 0 ; x < px_in->width ; x++ ) {

			/*
					J
				  A B C		E0 E1 E2
				K D E F L	E3 E4 E5
				  G H I		E6 E7 E8
					M
			*/
			
			// pixel loading
			uint32 E = px_in->pixels[y][x].val;

			uint32 B = (y>0)			 ? px_in->pixels[y-1][x].val : PX_FILL;
			uint32 D = (x>0)			 ? px_in->pixels[y][x-1].val : PX_FILL;
			uint32 F = (x<px_in->width-1)  ? px_in->pixels[y][x+1].val : PX_FILL;
			uint32 H = (y<px_in->height-1) ? px_in->pixels[y+1][x].val : PX_FILL;

			uint32 A = (y>0 && x>0)		 						? px_in->pixels[y-1][x-1].val : PX_FILL;
			uint32 C = (y>0 && x<px_in->width-1)		 		? px_in->pixels[y-1][x+1].val : PX_FILL;
			uint32 G = (y<px_in->height-1 && x>0)  				? px_in->pixels[y+1][x-1].val : PX_FILL;
			uint32 I = (y<px_in->height-1 && x<px_in->width-1)	? px_in->pixels[y+1][x+1].val : PX_FILL;

			uint32 J = (y>1)					? px_in->pixels[y-2][x].val : PX_FILL;
			uint32 K = (x>1)					? px_in->pixels[y][x-2].val : PX_FILL;
			uint32 L = (x<(px_in->width-2))		? px_in->pixels[y][x+2].val : PX_FILL;
			uint32 M = (y<(px_in->height-2))	? px_in->pixels[y+2][x].val : PX_FILL;

			// equality checks
			bool AE = (A==E), AJ = (A==J), AK = (A==K), BD = (B==D), BF = (B==F), CE = (C==E), CJ = (C==J), CL = (C==L), DH = (D==H), EG = (E==G), EI = (E==I), FH = (F==H), GK = (G==K), GM = (G==M), IL = (I==L), IM = (I==M), EF = (E==F), EB = (E==B), ED = (E==D), EH=(E==H);

			// border condition
			bool par0 = !(BF || DH);		/* 	   J                 */
			bool par1 = !(BD || FH);		/*   A B C		E0 E1 E2 */	
											/* K D E F L	E3 E4 E5 */
			// diagonal condition			/*   G H I		E6 E7 E8 */
			bool dia0 = AE && EI;			/* 	   M                 */
			bool dia1 = CE && EG;

			// artifact prevention
			bool art0 = BD && par0 && (!AE || AJ || AK);
			bool art2 = BF && par1 && (!CE || CJ || CL);
			bool art6 = DH && par1 && (!EG || GK || GM);
			bool art8 = FH && par0 && (!EI || IL || IM);

			// line detection
			bool lin0 = dia0 && BF && DH && !(EB || EH);
			bool lin1 = dia1 && BD && FH && !(EF || ED);

			// rules
			uint32 E0 = art0 || BD && (!AE || lin1) ? B : E;
			uint32 E2 = art2 || BF && (!CE || lin0) ? F : E;
			uint32 E6 = art6 || DH && (!EG || lin0) ? D : E;
			uint32 E8 = art8 || FH && (!EI || lin1) ? H : E;
			// uint32 E0 = art0 || BD && (CE && !CJ && !AE || EG && !AE && !GK) ? B : E;
			// uint32 E2 = art2 || BF && (AE && !AJ && !CE || EI && !CE && !IL) ? F : E;
			// uint32 E6 = art6 || DH && (AE && !AK && !EG || EI && !EG && !IM) ? D : E;
			// uint32 E8 = art8 || FH && (CE && !CL && !EI || EG && !EI && !GM) ? H : E;

			uint32 E1 = art0 && !CE || art2 && !AE ? B : E;
			uint32 E3 = art0 && !EG || art6 && !AE ? D : E;
			uint32 E5 = art8 && !CE || art2 && !EI ? F : E;
			uint32 E7 = art8 && !EG || art6 && !EI ? H : E;

			// to output
			px_out->pixels[y*3][x*3].val		= E0;
			px_out->pixels[y*3][x*3+1].val		= E1;
			px_out->pixels[y*3][x*3+2].val		= E2;

			px_out->pixels[y*3+1][x*3].val		= E3;
			px_out->pixels[y*3+1][x*3+1].val	= E;
			px_out->pixels[y*3+1][x*3+2].val	= E5;
			
			px_out->pixels[y*3+2][x*3].val		= E6;
			px_out->pixels[y*3+2][x*3+1].val	= E7;
			px_out->pixels[y*3+2][x*3+2].val	= E8;

#ifdef TEST_PATTERN
			// test pattern
			px_out->pixels[y*3][x*3].a		= 200;
			px_out->pixels[y*3][x*3+1].a	= 200;
			px_out->pixels[y*3][x*3+2].a	= 200;

			px_out->pixels[y*3+1][x*3].a	= 200;
			px_out->pixels[y*3+1][x*3+1].a	= 255;
			px_out->pixels[y*3+1][x*3+2].a	= 200;
			
			px_out->pixels[y*3+2][x*3].a	= 200;
			px_out->pixels[y*3+2][x*3+1].a	= 200;
			px_out->pixels[y*3+2][x*3+2].a	= 200;
#endif
		}

		prct = (y/(float)px_in->height) * 100;
		if ( prct != last ) {
			printf("\033[26D[");
			for( int i = 0; i < 20; i++ ) putchar( (i <= prct/5) ? '#' : '-' );
			printf("] %02d%%", prct);
			fflush(stdout);
			last = prct;
		}
	}
	putchar('\n');
}
