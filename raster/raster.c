#include "general.h"
#include "pic_load.h"
#include "raster/dither.h"
#include "raster/raster.h"

void raster_mono ( dec_data * info, void * _dst, APPVar *app)
{
#ifdef USE_ASM
	__asm__ volatile (
		"	subq.w		#1, %2			| width\n"
		"	move.l		%3, d0			| scale -> index\n"
		"	addq.l		#1, d0\n"
		"	lsr.l		#1, d0\n"
		"0:\n"
		"	clr.w 		d1				| chunk\n"
		"	move.w		#0x8000, d2		| pixel\n"
		"1:\n"
		"	swap		d0\n"
		"	btst		#0, (d0.w,%1) 	| (src[index>>16] & 1)\n"
		"	beq.b		2f\n"
		"	or.w		d2, d1        	| chunk |= pixel\n"
		"2:\n"
		"	swap		d0\n"
		"	add.l		%3, d0        	| index += info->IncXfx\n"
		"	lsr.w		#1, d2\n"
		"	dbeq		%2, 1b\n"
		"	move.w		d1, (%0)+\n"
		"	subq.w		#1, %2\n"
		"	bpl.b		0b"
		:                                       /* output */
		: "a"(_dst), "a"(info->RowBuf),
		  /*  %0         %1             */
		  "d"(info->DthWidth),"d"(info->IncXfx) /* input  */
		  /*  %2                  %3            */
		: "d0","d1","d2"                        /* clobbered */
	);
#else
	uint16	*dst   	= _dst;
	uint32 	*map	= info->Pixel;
	int16   width 	= info->DthWidth;
	uint32  x     	= (info->IncXfx + 1) >> 1;
	uint16  pixel 	= 0x8000;
	uint16  chunk 	= 0;

	do 
	{
		if (  map[(int16)info->RowBuf[x >>16] & 1]) 
			chunk |= pixel;
		
		if ( !--width || !(pixel >>= 1)) 
		{
			*(dst++) = chunk;
			chunk    = 0;
			pixel    = 0x8000;
		}
		x += info->IncXfx;
	
	} while ( width);	
#endif		 
}

void raster_D1 ( dec_data * info, void * _dst, APPVar *app)
{
	uint16	*dst	= _dst;
	int16   width 	= info->DthWidth;
	uint32  x		= (info->IncXfx + 1) >> 1;
	uint32 	*map	= info->Pixel;
	uint16	mask	= info->PixMask;
	int16  	*buf	= info->DthBuf;
	int16   ins		= 0;
	uint16	pixel	= 0x8000;
	uint16	chunk	= 0;

	do 
	{
		ins += *buf + map[(int16)info->RowBuf[x >>16] & mask];

		if ( ins < 2040)
			chunk |= pixel;
		else
			ins -= 4080;

		*(buf++) = (ins >>= 2);

		if ( !--width || !(pixel >>= 1)) 
		{
			*(dst++) = chunk;
			chunk    = 0;
			pixel    = 0x8000;
		}
		x += info->IncXfx;
		
	} while ( width);
}

void gscale_D1( dec_data * info, void * _dst, APPVar *app)
{
	uint16 	*dst  	= _dst;
	int16   width 	= info->DthWidth;
	uint32  x     	= ( info->IncXfx + 1) >> 1;
	int16 	*buf   	= info->DthBuf;
	int16   ins   	= 0;
	uint16  pixel 	= 0x8000;
	uint16  chunk 	= 0;
	
	do 
	{
		ins += *buf + ( int16)info->RowBuf[x >>16];
		
		if ( ins < 0x80)
			chunk |= pixel;
		else
			ins -= 0xFF;
		
		*(buf++) = (ins >>= 2);
		
		if ( !--width || !(pixel >>= 1)) 
		{
			*(dst++) = chunk;
			chunk    = 0;
			pixel    = 0x8000;
		}
		x += info->IncXfx;
		
	} while (width);
}

void dither_D1 ( dec_data * info, void * _dst, APPVar *app)
{
	uint16 	*dst  	= _dst;
	int16   width 	= info->DthWidth;
	uint32  x     	= ( info->IncXfx + 1) >> 1;
	int16 	*buf   	= info->DthBuf;
	int16   ins   	= 0;
	uint16  pixel 	= 0x8000;
	uint16  chunk 	= 0;
	
	do 
	{
		uint8  *rgb   = &info->RowBuf[(x >>16) *3];
		ins	+= *buf + ( int16)rgb[0] * 5 + ( int16)rgb[1] * 9 + (( int16)rgb[2] << 1);
		
		if (ins < 2040) 
			chunk |= pixel;
		else
			ins -= 4080;
		
		*(buf++) = (ins >>= 2);
		
		if (!--width || !(pixel >>= 1)) 
		{
			*(dst++) = chunk;
			chunk    = 0;
			pixel    = 0x8000;
		}
		x += info->IncXfx;
		
	} while( width);
}


void raster_D2 ( dec_data * info, void * _dst, APPVar *app)
{
	uint16 	*dst  	= _dst;
	int16   width 	= info->DthWidth;
	uint32  x     	= ( info->IncXfx + 1) >> 1;
	uint32 	*map   	= info->Pixel;
	uint16  mask  	= info->PixMask;
	int16 	*buf   	= info->DthBuf;
	int16   ins   	= 0;
	uint16  pixel 	= 0x8000;
	uint16  chunk 	= 0;

	do 
	{
		ins += *buf + map[( int16)info->RowBuf[x >>16] & mask];
		
		if ( ins < 2040)
			chunk |= pixel;
		else
			ins -= 4080;
		
		*(buf++) = (ins >>= 2);
		
		if (!--width || !(pixel >>= 1)) 
		{
			*(dst++) = chunk;
			*(dst++) = chunk;
			chunk    = 0;
			pixel    = 0x8000;
		}
		x += info->IncXfx;
		
	} while ( width);
}

void gscale_D2( dec_data * info, void * _dst, APPVar *app)
{
	uint16 	*dst  	= _dst;
	int16   width 	= info->DthWidth;
	uint32  x     	= ( info->IncXfx + 1) >> 1;
	int16 	*buf   	= info->DthBuf;
	int16   ins   	= 0;
	uint16  pixel 	= 0x8000;
	uint16  chunk 	= 0;
	
	do 
	{
		ins += *buf + (short)info->RowBuf[x >>16];
		
		if (ins < 0x80)
			chunk |= pixel;
		else
			ins -= 0xFF;
		
		*(buf++) = (ins >>= 2);
		
		if (!--width || !(pixel >>= 1)) 
		{
			*(dst++) = chunk;
			*(dst++) = chunk;
			chunk    = 0;
			pixel    = 0x8000;
		}
		x += info->IncXfx;
		
	} while (width);
}

void dither_D2 ( dec_data * info, void * _dst, APPVar *app)
{
	uint16 	*dst  	= _dst;
	int16   width 	= info->DthWidth;
	uint32  x     	= ( info->IncXfx + 1) >> 1;
	int16 	*buf   	= info->DthBuf;
	int16   ins   	= 0;
	uint16  pixel 	= 0x8000;
	uint16  chunk 	= 0;

	do 
	{
		uint8  *rgb   = &info->RowBuf[( x >>16) * 3];
		ins += *buf + (int16)rgb[0] * 5 + (int16)rgb[1] * 9 + (int16)rgb[2] * 2;
		
		if (ins < 2040)
			chunk |= pixel;
		else
			ins -= 4080;
		
		*(buf++) = (ins >>= 2);
		
		if (!--width || !(pixel >>= 1)) 
		{
			*(dst++) = chunk;
			*(dst++) = chunk;
			chunk    = 0;
			pixel    = 0x8000;
		}
		x += info->IncXfx;
		
	} while ( width);
}


#ifdef USE_ASM
void raster_chunk4 (uint8 *src, uint16* dst, size_t num)
{
	__asm__ volatile (
		"	moveq		#0, d4			\n"
		"	move.b		(%0)+, d4		\n"
		"	move.l		d4, d5			\n"
		"	andi.b		#0x03, d4		\n"
		"	ror.l 		#1, d4			\n"
		"	ror.w		#1, d4			\n"
		"	andi.b		#0x0C, d5		\n"
		"	ror.l 		#3, d5			\n"
		"	ror.w		#1, d5			\n"

		"	subq.l		#2, %2			\n"
		"	bmi			9f				\n"

		"	moveq.l		#1, d1			\n"

		"1: 							\n"
		"	move.b		(%0)+, d2		\n"

		"	moveq.l		#0x03, d3 		\n"
		"	and.b		d2, d3			\n"
		"	ror.l 		#1, d3			\n"
		"	ror.w		#1, d3			\n"
		"	lsr.l		d1, d3			\n"
		"	or.l		d3, d4			\n"
		"	moveq.l		#0x0C, d3 		\n"
		"	and.b		d2, d3			\n"
		"	ror.l 		#3, d3			\n"
		"	ror.w		#1, d3			\n"
		"	lsr.l		d1, d3			\n"
		"	or.l		d3, d5			\n"

		"	addq.w		#1, d1			\n"
		"	dbra		%2, 1b 			\n"
		"9:								\n"
		"	movem.l		d4-d5, (%1)		"
		:
		: "a"(src),"a"(dst), "d"(num)
		: "d1", "d2", "d3", "d4", "d5"
	);
}

void raster_chunk8(uint8 *src, uint16* dst, size_t num)
{
	__asm__ volatile (
	"	moveq		#0, d4			\n"
	"	move.b		(%0)+, d4       \n"
	"	move.l		d4, d5          \n"
	"	move.l		d4, d6          \n"
	"	andi.b		#0x0C, d5       \n"
	"	move.l		d4, d7          \n"
	"	andi.b		#0x30, d6		\n"
	"	andi.b		#0x03, d4       \n"
	"	andi.b		#0xC0, d7		\n"
	"	ror.l 		#3, d5          \n"
	"	ror.l 		#1, d4          \n"
	"	ror.l 		#5, d6          \n"
	"	ror.w		#1, d5          \n"
	"	ror.l 		#7, d7          \n"
	"	ror.w		#1, d4          \n"
	"	ror.w		#1, d6          \n"
	"	ror.w		#1, d7          \n"
	"	subq.l		#2, %2          \n"
	"	bmi			9f              \n"
	"	moveq.l		#1, d1          \n"
	"	1:							\n"
	"	move.b		(%0)+, d2       \n"
	"	moveq.l		#0x03, d3       \n"
	"	and.b		d2, d3          \n"
	"	ror.l 		#1, d3          \n"
	"	ror.w		#1, d3          \n"
	"	lsr.l		d1, d3          \n"
	"	or.l		d3, d4          \n"
	"	moveq.l		#0x0C, d3       \n"
	"	and.b		d2, d3          \n"
	"	ror.l 		#3, d3          \n"
	"	ror.w		#1, d3          \n"
	"	lsr.l		d1, d3          \n"
	"	or.l		d3, d5          \n"
	"	moveq.l		#0x30, d3       \n"
	"	and.b		d2, d3          \n"
	"	ror.l 	    #5, d3          \n"
	"	ror.w		#1, d3          \n"
	"	lsr.l		d1, d3          \n"
	"	or.l		d3, d6          \n"
	"	move.l		#0xC0, d3       \n"
	"	and.b		d2, d3          \n"
	"	ror.l 		#7, d3          \n"
	"	ror.w		#1, d3          \n"
	"	lsr.l		d1, d3          \n"
	"	or.l		d3, d7          \n"

	"	addq.w		#1, d1			\n"
	"	dbra		%2, 1b			\n"
	"	9:							\n"
	"	movem.l		d4-d7, (%1)		"
		:
		: "a"(src),"a"(dst), "d"(num)
		: "d1", "d2", "d3", "d4", "d5", "d6", "d7"
	);
}

#else

#define raster_chunk4(s,d,n)   raster_chunks ( s, d, n, 4)
#define raster_chunk8(s,d,n)   raster_chunks ( s, d, n, 8)

void raster_chunks ( uint8 *src, uint16 *dst, uint16 num, uint16 depth)
{
	uint16   mask  	= 0x8000;
	uint16   *chunk = dst;
	uint8    pixel 	= *src;
	int16    i     	= depth;

	do 
	{
		*( chunk++) = ( pixel & 1 ? mask : 0);
		pixel >>= 1;
	} while (--i);
	
	while (--num) 
	{
		mask >>= 1;
		chunk = dst;
		pixel = *( ++src);
		i     = depth;

		do 
		{
			if ( pixel & 1) *chunk |=  mask;
			else           *chunk &= ~mask;
			chunk++;
			pixel >>= 1;
		} while (--i);
	}
}

#endif

void raster_I4 ( dec_data * info, void * _dst, APPVar *app)
{
	uint16 	*dst   	= _dst;
	int16   width 	= info->DthWidth;
	uint32  x     	= (info->IncXfx +1) >> 1;
	uint8   buf[16];
	int16  	n   	= 16;
	uint8 	*tmp 	= buf;

	do 
	{
		uint16 idx = info->RowBuf[x >>16];
		*(tmp++)  = *( uint8*)&info->Pixel[idx];
		
		if (!--width || !--n) 
		{
			raster_chunk4( buf, dst, tmp - buf);
			dst += 4;
			n    = 16;
			tmp  = buf;
		}
		x += info->IncXfx;
		
	} while ( width);
}

void gscale_I4( dec_data * info, void * _dst, APPVar *app)
{
	uint16 	*dst   	= _dst;
	int16   width 	= info->DthWidth;
	uint32  x     	= (info->IncXfx +1) >> 1;	
	int8 	*dth    = info->DthBuf;
	int16   err    	= 0;
	uint8   buf[16];
	int16  	n   	= 16;
	uint8 	*tmp 	= buf;

	do 
	{
		*(tmp++) = dither_gray (&info->RowBuf[x >>16], &err, &dth, app);
		
		if (!--width || !--n) 
		{
			raster_chunk4 (buf, dst, tmp - buf);
			dst += 4;
			n    = 16;
			tmp  = buf;
		}
		x += info->IncXfx;
		
	} while (width);
}

void dither_I4 ( dec_data * info, void * _dst, APPVar *app)
{
	uint16 	*dst   	= _dst;
	int16   width 	= info->DthWidth;
	uint32  x     	= (info->IncXfx +1) >> 1;	
	int8 	*dth    = info->DthBuf;
	int16   err[3] 	= { 0, 0, 0 };	
	uint8   buf[16];
	int16  	n   	= 16;
	uint8 	*tmp 	= buf;

	do 
	{
		*(tmp++) = dither_true (&info->RowBuf[(x >>16) *3], err, &dth, app);
		
		if (!--width || !--n) 
		{
			raster_chunk4 (buf, dst, tmp - buf);
			dst += 4;
			n    = 16;
			tmp  = buf;
		}
		x += info->IncXfx;
		 
	} while (width);
}


void raster_I8 ( dec_data * info, void * _dst, APPVar *app)
{
#ifdef USE_ASM
	size_t  x     = (info->IncXfx + 1) >> 1;
	__asm__ volatile (
	"	subq.w		#1, %4\n"
	"	swap		%6 				| -> chunk counter\n"
		
	"	1: 							| line loop\n"
	"	clr.w		%6\n"
	"	clr.l		d4\n"
	"	clr.l		d5\n"
	"	clr.l		d6\n"
	"	clr.l		d7\n"
		
	"	5: 							| chunk loop\n"
	"	swap		%4 				| -> value\n"
	"	swap		%6 				| -> mask\n"
	"	move.w		(%3), d3\n"
	"	add.l		%5, (%3)\n"
	"	move.b		(d3.w,%1), d3 	| palette index\n"
	"	and.w		%6, d3\n"
	"	lsl.w		#2, d3\n"
	"	move.b		(d3.w,%2), %4 	| pixel value\n"
	"	swap		%6 				| -> chunk counter\n"
		
	"	moveq.l		#0x03, d3 		|chunks 0/1\n"
	"	and.b		%4, d3\n"
	"	ror.l 		#1, d3\n"
	"	ror.w		#1, d3\n"
	"	lsr.l		%6, d3\n"
	"	or.l		d3, d4\n"
	"	moveq.l		#0x0C, d3 		|chunks 2/3\n"
	"	and.b		%4, d3\n"
	"	ror.l 		#3, d3\n"
	"	ror.w		#1, d3\n"
	"	lsr.l		%6, d3\n"
	"	or.l		d3, d5\n"
	"	moveq.l		#0x30, d3 		|chunks 4/5\n"
	"	and.b		%4, d3\n"
	"	ror.l	 	#5, d3\n"
	"	ror.w		#1, d3\n"
	"	lsr.l		%6, d3\n"
	"	or.l		d3, d6\n"
	"	move.l		#0xC0, d3 		|chunks 6/7\n"
	"	and.b		%4, d3\n"
	"	ror.l 		#7, d3\n"
	"	ror.w		#1, d3\n"
	"	lsr.l		%6, d3\n"
	"	or.l		d3, d7\n"
		
	"	swap		%4 				| -> width\n"
	"	addq.b		#1, %6\n"
	"	btst.b		#4, %6\n"
	"	dbne		%4, 5b 			| chunk loop\n"

	"	movem.l		d4-d7, (%0)\n"
	"	adda.w		#16, %0\n"
	"	subq.w		#1, %4\n"
	"	bpl.b		1b"
		:
		: "a"(_dst),"a"(info->RowBuf),"a"(info->Pixel),"a"(&x),
		/*    %0        %1                %2               %3 */
		  "d"(info->DthWidth),"d"(info->IncXfx),"d"(info->PixMask)
		/*    %4                  %5                %6          */
		: "d3","d4","d5","d6","d7"
	);
#else	
	uint16 	*dst   	= _dst;
	int16   width 	= info->DthWidth;
	uint32  x     	= (info->IncXfx +1) >> 1;
	uint8   buf[16];
	int16  	n   	= 16;
	uint8 	*tmp 	= buf;
	
	do 
	{
		uint16 idx = info->RowBuf[x >>16];
		*(tmp++)  = *(uint8*)&info->Pixel[idx];
		
		if (!--width || !--n) 
		{
			raster_chunk8 (buf, dst, tmp - buf);
			dst += 8;
			n    = 16;
			tmp  = buf;
		}
		x += info->IncXfx;
		
	} while (width);
#endif
}

void gscale_I8( dec_data * info, void * _dst, APPVar *app)
{
	uint16 	*dst   	= _dst;
	int16   width 	= info->DthWidth;
	uint32  x     	= (info->IncXfx + 1) >> 1;	
	int8 	*dth    = info->DthBuf;
	int16   err    	= 0;
	uint8   buf[16];
	int16  	n   	= 16;
	uint8 	*tmp	 = buf;

	do 
	{
		*(tmp++) = dither_gray (&info->RowBuf[x >>16], &err, &dth, app);
		
		if (!--width || !--n) {
			raster_chunk8 (buf, dst, tmp - buf);
			dst += 8;
			n    = 16;
			tmp  = buf;
		}
		x += info->IncXfx;
		
	} while (width);
}

void dither_I8 (dec_data * info, void * _dst, APPVar *app)
{
	uint16 	*dst   	= _dst;
	int16   width 	= info->DthWidth;
	uint32  x     	= (info->IncXfx + 1) >> 1;	
	int8 	*dth    = info->DthBuf;
	int16   err[3] 	= { 0, 0, 0 };
	uint8   buf[16];
	int16  	n   	= 16;
	uint8 	*tmp 	= buf;

	do 
	{
		*(tmp++) = dither_true (&info->RowBuf[(x >>16) *3], err, &dth, app);
		
		if (!--width || !--n) 
		{
			raster_chunk8 (buf, dst, tmp - buf);
			dst += 8;
			n    = 16;
			tmp  = buf;
		}
		x += info->IncXfx;
		 
	} while ( width);
}


void raster_P8 (dec_data * info, void * _dst, APPVar *app)
{
	uint8  	*dst  = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx + 1) >> 1;
	uint32 	*map  = info->Pixel;
	uint16  mask  = info->PixMask;

	do 
	{
		*(dst++) = *( uint8*)&map[( int16)info->RowBuf[x >>16] & mask];
		x += info->IncXfx;
	} while (--width);
}

void gscale_P8( dec_data * info, void * _dst, APPVar *app)
{
	uint8  	*dst  = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx + 1) >> 1;
	int8    *dth  = info->DthBuf;
	int16   err   = 0;

	do 
	{
		*(dst++) = dither_gray (&info->RowBuf[x >>16], &err, &dth, app);
		x += info->IncXfx;
	} while (--width);
}

void dither_P8 (dec_data * info, void * _dst, APPVar *app)
{
	uint8 	*dst  	= _dst;
	int16  	width 	= info->DthWidth;
	uint32 	x     	= (info->IncXfx + 1) >> 1;
	int8 	*dth   	= info->DthBuf;
	int16   err[3] 	= { 0, 0, 0 };

	do 
	{
		*(dst++) = dither_true (&info->RowBuf[(x >>16) *3], err, &dth, app);
		x += info->IncXfx;
	} while (--width);
}


void dither32_P8 (dec_data * info, void * _dst, APPVar *app)
{
	uint8 	*dst  	= _dst;
	int16  	width 	= info->DthWidth;
	uint32 	x     	= (info->IncXfx + 1) >> 1;
	int8 	*dth   	= info->DthBuf;
	int16   err[3] 	= { 0, 0, 0 };

	do 
	{
		*(dst++) = ditherize_32 (&info->RowBuf[(x >>16) << 2], err, &dth, app);
		x += info->IncXfx;
	} while (--width);
}

void raster_15 (dec_data * info, void * _dst, APPVar *app)
{
	uint16 	*dst  = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx + 1) >> 1;
	uint32 	*map  = info->Pixel;
	uint16  mask  = info->PixMask;
	
	do 
	{
		*(dst++) = map[(short)info->RowBuf[x >>16] & mask];
		x += info->IncXfx;
	} while (--width);
}

void gscale_15( dec_data * info, void * _dst, APPVar *app)
{
	uint16 * dst   = _dst;
	int16   width  = info->DthWidth;
	uint32  x      = (info->IncXfx + 1) >> 1;
	
	do 
	{
		uint16 rgb = info->RowBuf[(x >>16)];
		rgb        = ((rgb & 0xF8) <<7) | ((rgb & 0xF8) <<2) | (rgb >>3);
		*(dst++)   = (rgb >> 8) | (rgb << 8);
		x += info->IncXfx;
	} while (--width);
}

void dither_15 (dec_data * info, void * _dst, APPVar *app)
{
	uint16 	*dst  = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx + 1) >> 1;

	do {
		uint8 * rgb = &info->RowBuf[(x >>16) * 3];
		*(dst++)   	= (((uint16)rgb[0] & 0xF8) << 7) | (((uint16)rgb[1] & 0xF8) << 2) | ( rgb[2] >> 3);
		x += info->IncXfx;
	} while (--width);
}

void raster_15r (dec_data * info, void * _dst, APPVar *app)
{
	uint16 	*dst  = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx + 1) >> 1;
	uint32 	*map  = info->Pixel;
	uint16  mask  = info->PixMask;

	do 
	{
		uint16 rgb = map[(short)info->RowBuf[x >>16] & mask];
		*(dst++)  = (rgb >> 8) | (rgb << 8);
		x += info->IncXfx;
	} while (--width);
}

void gscale_15r( dec_data * info, void * _dst, APPVar *app)
{
	uint16 * dst   = _dst;
	int16   width  = info->DthWidth;
	uint32  x      = (info->IncXfx + 1) >> 1;
	
	do 
	{
		uint16 rgb = info->RowBuf[(x >>16)];
		rgb        = ((rgb & 0xF8) <<7) | ((rgb & 0xF8) <<2) | (rgb >>3);
		*(dst++)   = (rgb >> 8) | (rgb << 8);
		x += info->IncXfx;
	} while (--width);
}

void dither_15r (dec_data * info, void * _dst, APPVar *app)
{
	uint16 	*dst  = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx + 1) >> 1;
	
	do 
	{
		uint8 * rgb = &info->RowBuf[(x >>16) * 3];
		uint16  pix = (((uint16)rgb[0] & 0xF8) << 7) | (((uint16)rgb[1] & 0xF8) << 2) | (rgb[2] >> 3);
		*(dst++)   = (pix >> 8) | (pix << 8);
		x += info->IncXfx;
	} while (--width);
}


void raster_16 (dec_data * info, void * _dst, APPVar *app)
{
	uint16 	*dst  = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx + 1) >> 1;
	uint32 	*map  = info->Pixel;
	uint16  mask  = info->PixMask;
	
	do 
	{
		*(dst++) = map[(int16)info->RowBuf[x >>16] & mask];
		x += info->IncXfx;
	} while (--width);
}

void gscale_16( dec_data * info, void * _dst, APPVar *app)
{
#if USE_ASM
	__asm__ volatile
	(
		"	cmp.l       #65536, %[IncXfx]	\n"
		"	beq			1f					\n"
		"	subq.w		#1, %[width]		\n"
		"	move.l 		%[IncXfx], d2		\n"
		"	moveq 		#16, d3				\n"
		"	addq.l		#1, d2				\n"
		"	lsr.l		#1, d2				\n"
		"2:									\n"
		"	move.l		d2, d1				\n"
		"	lsr.l		d3, d1				\n"
		"	move.l		%[src]@(d1.l), d0	\n"
		"	move.b		d0, d1				\n"
		"	lsl.l		#5, d1				\n"
		"	move.b		d0, d1				\n"
		"	lsl.l		#6, d1				\n"
		"	move.b		d0, d1				\n"
		"	lsr.l		#3, d1				\n"
		"	move.w		d1, (%[dst])+		\n"
		"	add.l		%[IncXfx], d2		\n"
		"	dbra		%[width], 2b		\n"
		"	bra			3f					\n"
		"1:									\n"
		"	lsr.w		#1, %[width]		\n"
		"	subq.w		#1, %[width]		\n"
		"4:									\n"
		"	clr.l		d0					\n"
		"	move.w		(%[src])+, d0		\n"
		"	lsl.l		#8, d0				\n"
		"	lsr.w		#8, d0				\n"
		"	move.l		d0, d1				\n"
		"	lsr.l  		#2, d0				\n"
		"	andi.w		#0x3F, d0			\n"
		"	lsl.l		#5, d0				\n"
		"	lsr.l		#3, d1				\n"
		"	andi.w		#0x1F, d1			\n"
		"	or.l		d1, d0				\n"
		"	lsl.l		#8, d1				\n"
		"	lsl.l		#3, d1				\n"
		"	or.l		d1, d0				\n"
		"	move.l		d0, (%[dst])+		\n"
		"	dbra		%[width], 4b		\n"
		"3:									  "
		:
		: [src] "a" (info->RowBuf), [dst] "a" (_dst), [width] "d" (info->DthWidth), [IncXfx] "d" (info->IncXfx)
		: "d0", "d1", "d2", "d3"
	);
#else
	uint16 *dst 	= _dst;
	int16   width 	= info->DthWidth;
	uint32  x     	= (info->IncXfx + 1) >> 1;

	do 
	{
		uint16 rgb = info->RowBuf[(x >>16)];
		*(dst++)  = ((rgb & 0xF8) << 8) | ((rgb & 0xFC) << 3) | (rgb >> 3);

		x += info->IncXfx;
	} while (--width);
#endif
}

void dither_16 (dec_data * info, void * _dst, APPVar *app)
{
#if USE_ASM
	__asm__ volatile
	(
		"	cmp.l       #65536, %[IncXfx]   \n"
		"	beq			1f					\n"
		"	move.l 		%[IncXfx], d2		\n"
		"	moveq 		#16, d0				\n"
		"	addq.l		#1, d2				\n"
		"	subq.w		#1, %[width]		\n"
		"	lsr.l		#1, d2				\n"
		"2:									\n"
		"	move.l		d2, d1				\n"
		"	lsr.l		d0, d1				\n"
		"	mulu.l		#3, d1				\n"
		"	move.l		%[src], a0			\n"
		"	add.l		d1, a0				\n"
		"	move.b		(a0)+, d1			\n"
		"	lsl.l		#5, d1				\n"
		"	move.b		(a0)+, d1			\n"
		"	lsl.l		#6, d1				\n"
		"	move.b		(a0), d1			\n"
		"	lsr.l		#3, d1				\n"
		"	move.w		d1, (%[dst])+		\n"
		"	add.l		%[IncXfx], d2		\n"
		"	dbra		%[width], 2b		\n"
		"	bra			3f					\n"
		"1:									\n"
		"	lsr.w		#1, %[width]		\n"
		"	subq.w		#1, %[width]		\n"
		"4:									\n"
		"	movem.w		(%[src])+, d0-d2	\n"
		"	lsl.l		#5, d0				\n"
		"	lsl.w		#3, d0				\n"
		"	lsl.l		#6, d0				\n"
		"	move.w		d1, d0				\n"
		"	lsl.l		#5, d0				\n"
		"	move.w		d0, d1				\n"
		"	lsl.l		#8, d1				\n"
		"	move.w		d2, d1				\n"
		"	lsl.l		#6, d1				\n"
		"	lsl.w		#2, d1				\n"
		"	lsl.l		#5, d1				\n"
		"	swap		d1					\n"
		"	move.w		d1, d0				\n"
		"	move.l		d0, (%[dst])+		\n"
		"	dbra		%[width], 4b		\n"
		"3:									  "
		:
		: [src] "a" (info->RowBuf), [dst] "a" (_dst), [width] "d" (info->DthWidth), [IncXfx] "d" (info->IncXfx)
		: "d0", "d1", "d2", "a0"
	);
#else
	uint16 *dst   = _dst;
	int16   width = info->DthWidth;
	uint32  x	  = (info->IncXfx + 1) >> 1;

	do
	{
		register uint8 *rgb = &info->RowBuf[(x >> 16) * 3];

		*(dst++) = (((uint16)rgb[0] & 0xF8) << 8) | (((uint16)rgb[1] & 0xFC) << 3) | ( rgb[2] >> 3);
		x += info->IncXfx;
	} while (--width);
#endif
}


void raster_16r (dec_data * info, void * _dst, APPVar *app)
{
	uint16 	*dst  = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx + 1) >> 1;
	uint32 	*map  = info->Pixel;
	uint16  mask  = info->PixMask;
	
	do 
	{
		uint16 rgb = map[(int16)info->RowBuf[x >>16] & mask];
		*(dst++)  = (rgb >> 8) | (rgb << 8);
		x += info->IncXfx;
	} while (--width);
}

void gscale_16r( dec_data * info, void * _dst, APPVar *app)
{
	uint16  *dst   = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx + 1) >> 1;

	do 
	{
		uint16 rgb = info->RowBuf[(x >> 16)];
		rgb        = ((rgb & 0xF8) << 8) | ((rgb & 0xFC) << 3) | (rgb >> 3);
		*(dst++)   = (rgb >> 8) | (rgb << 8);
		x += info->IncXfx;
	} while (--width);
}

void dither_16r (dec_data * info, void * _dst, APPVar *app)
{
	uint16 	*dst  = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx + 1) >> 1;
	
	do 
	{
		uint8 * rgb = &info->RowBuf[(x >>16) *3];
		uint16  pix = ((( uint16)rgb[0] & 0xF8) << 8) | ((( uint16)rgb[1] & 0xFC) << 3) | ( rgb[2] >> 3);
		*(dst++)   = (pix >> 8) | (pix << 8);
		x += info->IncXfx;
	} while (--width);
}


void raster_24 (dec_data * info, void * _dst, APPVar *app)
{
	uint8 	*dst  = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx + 1) >> 1;
	uint32 	*map  = info->Pixel;
	uint16  mask  = info->PixMask;

	do 
	{
		uint8 * rgb = (uint8*)&map[( int16)info->RowBuf[x >> 16] & mask];
		*(dst++) = *(++rgb);
		*(dst++) = *(++rgb);
		*(dst++) = *(++rgb);
		x += info->IncXfx;
	} while (--width);
}

void gscale_24( dec_data * info, void * _dst, APPVar *app)
{
#if USE_ASM
	__asm__ volatile
	(
		"	cmp.l       #65536, %[IncXfx]      	\n"
		"	beq			1f						\n"
		"	move.l 		%[IncXfx], d2			\n"
		"	moveq 		#16, d3					\n"
		"	addq.l		#1, d2					\n"
		"	subq.w		#1, %[width]			\n"
		"	lsr.l		#1, d2					\n"
		"2:										\n"
		"	move.l		d2, d1					\n"
		"	lsr.l		d3, d1					\n"
		"	move.b		%[src]@(d1.l), d0		\n"
		"	move.b		d0, d1					\n"
		"	lsl.l		#8, d0					\n"
		"	move.b		d1, d0					\n"
		"	move.w		d0, (%[dst])+			\n"
		"	move.b		d0, (%[dst])+			\n"
		"	add.l		%[IncXfx], d2			\n"
		"	dbra		%[width], 2b			\n"
		"	bra			3f						\n"
		"1:										\n"
		"	lsr.w		#1, %[width]			\n"
		"	subq.w		#1, %[width]			\n"
		"4:										\n"
		"	move.b		(%[src])+, d1			\n"
		"	move.b		d1, d2					\n"
		"	lsl.l		#8, d2					\n"
		"	move.b		d1, d2					\n"
		"	lsl.l		#8, d2					\n"
		"	move.b		d1, d2					\n"
		"	lsl.l		#8, d2					\n"
		"	move.b		d1, d2					\n"
		"	move.l		d2, (%[dst])+			\n"
		"	move.w		d2, (%[dst])+			\n"
		"	dbra		%[width], 4b		 	\n"
		"3:										  "
		:
		: [src] "a" (info->RowBuf), [dst] "a" (_dst), [width] "d" (info->DthWidth), [IncXfx] "d" (info->IncXfx)
		: "d0", "d1", "d2", "d3"
	);
#else
	uint8  *dst  = _dst;
	int16  width = info->DthWidth;
	uint32 x     = (info->IncXfx + 1) >> 1;

	do 
	{
		uint8 rgb = info->RowBuf[(x >>16)];
		*(dst++) = rgb;
		*(dst++) = rgb;
		*(dst++) = rgb;
		x += info->IncXfx;
	} while (--width);
#endif
}


void dither_24 (dec_data * info, void * _dst, APPVar *app)
{
#if USE_ASM
	__asm__ volatile
	(
		"	cmp.l       #65536, %[IncXfx]      	\n"
		"	beq			1f						\n"
		"	move.l 		%[IncXfx], d2			\n"
		"	moveq 		#16, d3					\n"
		"	addq.l		#1, d2					\n"
		"	subq.w		#1, %[width]			\n"
		"	lsr.l		#1, d2					\n"
		"2:										\n"
		"	move.l		d2, d1					\n"
		"	lsr.l		d3, d1					\n"
		"	mulu.l		#3, d1					\n"
		"	move.l		%[src], a0				\n"
		"	add.l		d1, a0					\n"
		"	move.w		(a0)+, (%[dst])+		\n"
		"	move.b		(a0), (%[dst])+			\n"
		"	add.l		%[IncXfx], d2			\n"
		"	dbra		%[width], 2b			\n"
		"	bra			3f						\n"
		"1:										\n"
		"	lsr.w		#1, %[width]			\n"
		"	subq.w		#1, %[width]			\n"
		"4:										\n"
		"	move.l		(%[src])+, (%[dst])+	\n"
		"	move.w		(%[src])+, (%[dst])+	\n"
		"	dbra		%[width], 4b		 	\n"
		"3:										  "
		:
		: [src] "a" (info->RowBuf), [dst] "a" (_dst), [width] "d" (info->DthWidth), [IncXfx] "d" (info->IncXfx)
		: "d1", "d2", "d3", "a0"
	);
#else
	uint8 	*dst  = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx + 1) >> 1;

	do 
	{
		uint8 * rgb = &info->RowBuf[(x >> 16) * 3];
		*(dst++) = *(rgb++);
		*(dst++) = *(rgb++);
		*(dst++) = *(rgb);
		x += info->IncXfx;
	} while (--width);
#endif
}

void raster_24r (dec_data * info, void * _dst, APPVar *app)
{
	uint8 	*dst  = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx + 1) >> 1;
	uint32 	*map  = info->Pixel;
	uint16  mask  = info->PixMask;

	do 
	{
		uint8 * rgb = (uint8*)&map[(int16)info->RowBuf[x >> 16] & mask];
		*(dst++) = rgb[3];
		*(dst++) = rgb[2];
		*(dst++) = rgb[1];
		x += info->IncXfx;
	} while (--width);
}

void gscale_24r( dec_data * info, void * _dst, APPVar *app)
{
#if USE_ASM
	__asm__ volatile
	(
		"	cmp.l       #65536, %[IncXfx]      	\n"
		"	beq			1f						\n"
		"	move.l 		%[IncXfx], d2			\n"
		"	moveq 		#16, d3					\n"
		"	addq.l		#1, d2					\n"
		"	subq.w		#1, %[width]			\n"
		"	lsr.l		#1, d2					\n"
		"2:										\n"
		"	move.l		d2, d1					\n"
		"	lsr.l		d3, d1					\n"
		"	move.b		%[src]@(d1.l), d0		\n"
		"	move.b		d0, d1					\n"
		"	lsl.l		#8, d0					\n"
		"	move.b		d1, d0					\n"
		"	move.w		d0, (%[dst])+			\n"
		"	move.b		d0, (%[dst])+			\n"
		"	add.l		%[IncXfx], d2			\n"
		"	dbra		%[width], 2b			\n"
		"	bra			3f						\n"
		"1:										\n"
		"	lsr.w		#1, %[width]			\n"
		"	subq.w		#1, %[width]			\n"
		"4:										\n"
		"	move.w		(%[src])+, d1			\n"
		"	move.b		d1, d2					\n"
		"	lsl.l		#8, d2					\n"
		"	move.b		d1, d2					\n"
		"	lsr.l		#8, d1					\n"
		"	move.b		d1, d3					\n"
		"	lsl.l		#8, d3					\n"
		"	move.b		d1, d3					\n"
		"	lsl.l		#8, d3					\n"
		"	move.b		d1, d3					\n"
		"	lsl.l		#8, d3					\n"
		"	move.b		d2, d3					\n"
		"	move.l		d3, (%[dst])+			\n"
		"	move.w		d2, (%[dst])+			\n"
		"	dbra		%[width], 4b		 	\n"
		"3:										  "
		:
		: [src] "a" (info->RowBuf), [dst] "a" (_dst), [width] "d" (info->DthWidth), [IncXfx] "d" (info->IncXfx)
		: "d0", "d1", "d2", "d3"
	);
#else
	uint8  *dst  = _dst;
	int16  width = info->DthWidth;
	uint32 x     = (info->IncXfx + 1) >> 1;

	do
	{
		uint8 rgb = info->RowBuf[(x >> 16)];
		*(dst++) = rgb;
		*(dst++) = rgb;
		*(dst++) = rgb;
		x += info->IncXfx;
	} while (--width);
#endif
}

void dither_24r( dec_data * info, void * _dst, APPVar *app)
{
#if USE_ASM
	__asm__ volatile
	(
		"	cmp.l       #65536, %[IncXfx]      	\n"
		"	beq			1f						\n"
		"	move.l 		%[IncXfx], d2			\n"
		"	moveq 		#16, d3					\n"
		"	addq.l		#1, d2					\n"
		"	subq.w		#1, %[width]			\n"
		"	lsr.l		#1, d2					\n"
		"2:										\n"
		"	move.l		d2, d1					\n"
		"	lsr.l		d3, d1					\n"
		"	mulu.l		#3, d1					\n"
		"	move.l		%[src], a0				\n"
		"	add.l		d1, a0					\n"
		"	move.b		(a0)+, d0				\n"
		"	move.b		(a0)+, d1				\n"
		"	lsl.l		#8, d1					\n"
		"	move.b		d0, d1					\n"
		"	move.b		(a0), (%[dst])+			\n"
		"	move.w		d1, (%[dst])+			\n"
		"	add.l		%[IncXfx], d2			\n"
		"	dbra		%[width], 2b			\n"
		"	bra			3f						\n"
		"1:										\n"
		"	lsr.w		#1, %[width]			\n"
		"	subq.w		#1, %[width]			\n"
		"4:										\n"
		"	move.l		(%[src])+, d2			\n"
		"	move.b		d2, d3					\n"
		"	lsr.l		#8, d2					\n"
		"	move.b		d2, d1					\n"
		"	lsr.l		#8, d2					\n"
		"	lsl.l		#8, d1					\n"
		"	move.b		d2, d1					\n"
		"	lsr.l		#8, d2					\n"
		"	lsl.l		#8, d1					\n"
		"	move.b		d2, d1					\n"
		"	lsl.l		#8, d1					\n"
		"	move.w		(%[src])+, d2			\n"
		"	move.b		d2, d1					\n"
		"	move.b		d3, d2					\n"
		"	move.l		d1, (%[dst])+			\n"
		"	move.w		d2, (%[dst])+			\n"
		"	dbra		%[width], 4b		 	\n"
		"3:										  "
		:
		: [src] "a" (info->RowBuf), [dst] "a" (_dst), [width] "d" (info->DthWidth), [IncXfx] "d" (info->IncXfx)
		: "d0", "d1", "d2", "d3", "a0"
	);
#else
	uint8 	*dst  = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx + 1) >> 1;
	
	do 
	{
		uint8 * rgb = &info->RowBuf[(x >> 16) * 3];
		*(dst++) = rgb[2];
		*(dst++) = rgb[1];
		*(dst++) = rgb[0];
		x += info->IncXfx;
	} while (--width);
#endif
}


void raster_32 (dec_data * info, void * _dst, APPVar *app)
{
	uint32 * dst   = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx + 1) >> 1;
	uint32 	*map  = info->Pixel;
	uint16  mask  = info->PixMask;
	
	do 
	{
		*(dst++) = map[(int16)info->RowBuf[x >> 16] & mask];
		x += info->IncXfx;
	} while (--width);
}

void gscale_32( dec_data * info, void * _dst, APPVar *app)
{
#if USE_ASM
	__asm__ volatile
	(
		"	subq.w		#1, %[width]		\n"
		"	cmp.l       #65536, %[IncXfx]   \n"
		"	beq			1f					\n"
		"	move.l 		%[IncXfx], d2		\n"
		"	addq.l		#1, d2				\n"
		"	moveq 		#16, d1				\n"
		"	lsr.l		#1, d2				\n"
		"2:									\n"
		"	move.l		d2, d3				\n"
		"	lsr.l		d1, d3				\n"
		"	move.b		%[src]@(d3.l), d0	\n"
		"	adda.l		#1, %[dst]			\n"
		"	move.b		d0, (%[dst])+		\n"
		"	move.b		d0, (%[dst])+		\n"
		"	move.b		d0, (%[dst])+		\n"
		"	add.l		%[IncXfx], d2		\n"
		"	dbra		%[width], 2b		\n"
		"	bra			3f					\n"
		"1:									\n"
		"	move.b		(%[src])+, d0		\n"
		"	adda.l		#1, %[dst]			\n"
		"	move.b		d0, (%[dst])+		\n"
		"	move.b		d0, (%[dst])+		\n"
		"	move.b		d0, (%[dst])+		\n"
		"	dbra		%[width], 1b		\n"
		"3:									 "
		:
		: [src] "a" (info->RowBuf), [dst] "a" (_dst), [width] "d" (info->DthWidth), [IncXfx] "d" (info->IncXfx)
		: "d0", "d1", "d2", "d3"
	);
#else
	register uint32  *dst  = _dst;
	register uint8   rgb;
	register int16   width = info->DthWidth;
	register uint32  x     = (info->IncXfx + 1) >> 1;

	do
	{
		rgb = info->RowBuf[(x >> 16)];
		*(dst++)  = ((( rgb << 8) | rgb) << 8) | rgb;
		x += info->IncXfx;
	} while (--width);
#endif
}


void dither_32( dec_data * info, void * _dst, APPVar *app)
{
#if USE_ASM
	__asm__ volatile
	(
		"	subq.w		#1, %[width]				\n"
		"	subq.l		#1, %[src]					\n"
		"	cmp.l       #65536, %[IncXfx]       	\n"
		"	beq			1f							\n"
		"	move.l 		%[IncXfx], d2				\n"
		"	moveq 		#16, d3						\n"
		"	addq.l		#1, d2						\n"
		"	lsr.l		#1, d2						\n"
		"2:	move.l		d2, d1						\n"
		"	lsr.l		d3, d1						\n"
		"	move.l		d1, d0                      \n"
		"	add.l		d1, d1						\n"
		"	add.l		d0, d1						\n"
		"	move.l		%[src]@(d1.l), (%[dst])+	\n"
		"	add.l		%[IncXfx], d2				\n"
		"	dbra		%[width], 2b				\n"
		"	bra			3f							\n"
		"1:	move.l		(%[src]), (%[dst])+			\n"
		"	addq.l		#3, %[src]					\n"
		"	dbra		%[width], 1b			  	\n"
		"3:											  "
		:
		: [src] "a" (info->RowBuf), [dst] "a" (_dst), [width] "d" (info->DthWidth), [IncXfx] "d" (info->IncXfx)
		: "d0", "d1", "d2", "d3"
	);
#else
	register uint32  *dst  = _dst;
	register int16   width = info->DthWidth;
	register uint32  x     = (info->IncXfx + 1) >> 1;
	register uint8   *RowBuf = info->RowBuf;

	do
	{
		register uint32 *rgb = ( uint32*)&RowBuf[(x >> 16) * 3];
		*(dst++) = rgb[0] >> 8;
		x += info->IncXfx;

	} while (--width);
#endif
}

void raster_32r( dec_data * info, void * _dst, APPVar *app)
{
	uint32 * dst   = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx + 1) >> 1;
	uint32 	*map  = info->Pixel;
	uint16  mask  = info->PixMask;
	
	do 
	{
		uint8 * rgb = (uint8*)&map[(int16)info->RowBuf[x >>16] & mask];
		*(dst++) = (((((int32)rgb[3] << 8) | rgb[2]) << 8) | rgb[1]) << 8;
		x += info->IncXfx;
	} while (--width);
}

void gscale_32r( dec_data * info, void * _dst, APPVar *app)
{
	uint32 *dst   = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx + 1) >> 1;

	do 
	{
		uint32 rgb = info->RowBuf[(x >>16)];
		*(dst++)  = (((( rgb << 8) | rgb) << 8) | rgb) << 8;
		x += info->IncXfx;
	} while (--width);
}

void dither_32r (dec_data * info, void * _dst, APPVar *app)
{
	uint32 * dst   = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx + 1) >> 1;
	
	do 
	{
		uint8 * rgb = &info->RowBuf[(x >> 16) * 3];
		*(dst++)   = (((((uint32)rgb[2] << 8) | rgb[1]) << 8) | rgb[0]) << 8;
		x += info->IncXfx; 
	} while (--width);
}


void raster_32z ( dec_data * info, void * _dst, APPVar *app)
{
	uint32 * dst   = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx +1) >> 1;
	uint32 * map   = info->Pixel;
	uint16   mask  = info->PixMask;
	do {
		*(dst++) = map[(int16)info->RowBuf[x >> 16] & mask] << 8;
		x += info->IncXfx;
	} while (--width);
}

void gscale_32z (dec_data * info, void * _dst, APPVar *app)
{
	uint32 * dst   = _dst;
	int16   width = info->DthWidth;
	uint32  x     = (info->IncXfx +1) >> 1;
	do {
		uint32 rgb = info->RowBuf[(x >> 16)];
		*(dst++)  = ((((rgb <<8) | rgb) << 8) | rgb) << 8;
		x += info->IncXfx;
	} while (--width);
}

void dither_32z (dec_data * info, void * _dst, APPVar *app)
{
	uint32  *dst	= _dst;
	int16   width	= info->DthWidth;
	uint32  x    	= (info->IncXfx +1) >> 1;
	do {
		uint8 * rgb = &info->RowBuf[(x >> 16) * 3];
		*(dst++)   = (((((uint32)rgb[0] << 8) | rgb[1]) << 8) | rgb[2]) << 8;
		x += info->IncXfx;
	} while (--width);
}
