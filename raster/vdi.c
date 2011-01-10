#include "general.h"
#include "pic_load.h"
#include "raster/raster.h"
#include "raster/color.h"
#include "raster/pixel.h"
#include <mint/cookie.h>

int16 vdi_init( APPVar *app)
{
	int16	 	plane, out[273] = { -1, }; 
	int16		work_in[11], work_out[57], 
				fill_pattern[16] = { 0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA, 0x5555};
	boolean	  	reverse, z_trail;
	int32		dum;

	if( Getcookie( C_EdDI, &dum) == C_NOTFOUND)
		return( -1);

	vq_extnd( app->aes_handle, 1, work_out);

	work_in[0] = Getrez() + 2;
	for( dum = 1; dum < 10; work_in[dum ++] = 1);
	work_in[10] = 2;

	app->vdi_handle = app->aes_handle;
	app->nplanes 	= work_out[ 4];

	v_opnvwk( work_in, &app->vdi_handle, work_out);
	
	vq_scrninfo( app->vdi_handle, out);

	vsl_udsty( app->vdi_handle, 0xAAAA);	
	vsf_udpat( app->vdi_handle, fill_pattern, 1);
	
	plane = (( uint16)out[4] == 0x8000u ? 15 : out[2]);	/* bits per pixel used */
	reverse = ( out[16] < out[48]);						/* intel crap...       */
	z_trail = ( out[48] > 0);							/* bits are shifted to the right side */

 	if( plane <= 8)
	{
		save_colors( app);
		memcpy( app->raster.pixel_val, out + 16, 512);
	}

	app->raster.planar = 0;

	if ( plane == 1) 
	{	/* monochrome */
		app->raster.cnvpal_color = cnvpal_1_2;
		app->raster.raster_cmap  = raster_D1;
		app->raster.raster_true  = dither_D1;
		app->raster.raster_gray  = gscale_D1;
	}
	else if ( out[0] == 0) /* Planar */
	{
		app->raster.planar = 1;

		switch ( plane)
		{
			case 2:
				app->raster.cnvpal_color = cnvpal_1_2;
				app->raster.raster_cmap  = raster_D2;
				app->raster.raster_true  = dither_D2;
				app->raster.raster_gray  = gscale_D2;
				break;
			case 4:
				app->raster.cnvpal_color = cnvpal_4_8;
				app->raster.raster_cmap  = raster_I4;
				app->raster.raster_true  = dither_I4;
				app->raster.raster_gray  = gscale_I4;
				break;
			case 8:
				app->raster.cnvpal_color = cnvpal_4_8;
				app->raster.raster_cmap  = raster_I8;
				app->raster.raster_true  = dither_I8;
				app->raster.raster_gray  = gscale_I8;
				break;
		}
	}
	else if ( out[0] == 2) /* chunky */
		switch ( plane)
		{
			case  8:
				app->raster.cnvpal_color = cnvpal_4_8;
				app->raster.raster_cmap  = raster_P8;
				app->raster.raster_true  = dither_P8;
				app->raster.raster_gray  = gscale_P8;
				break;

			case 15: 
				if (!(out[14] & 0x02)) 
				{
					app->raster.cnvpal_color = cnvpal_15;

					if (reverse) 
					{
						app->raster.raster_cmap = raster_15r;
						app->raster.raster_gray = gscale_15r;
						app->raster.raster_true = dither_15r;
					} 
					else 
					{
						app->raster.raster_cmap = raster_15;
						app->raster.raster_gray = gscale_15;
						app->raster.raster_true = dither_15;
					}

					break;
				}

			case 16:
				app->raster.cnvpal_color = cnvpal_high;

				if (reverse) 
				{
					app->raster.raster_cmap = raster_16r;
					app->raster.raster_gray = gscale_16r;
					app->raster.raster_true = dither_16r;
					app->raster.getPixel	= getPixel_16r;
					app->raster.setPixel	= setPixel_16r;
				} 
				else 
				{
					app->raster.raster_cmap = raster_16;
					app->raster.raster_gray = gscale_16;
					app->raster.raster_true = dither_16;
					app->raster.getPixel	= getPixel_16;
					app->raster.setPixel	= setPixel_16;					
				}

				break;

			case 24:
				app->raster.cnvpal_color = cnvpal_true;

				if (reverse) 
				{
					app->raster.raster_cmap = raster_24r;
					app->raster.raster_gray = gscale_24r;
					app->raster.raster_true = dither_24r;
					app->raster.getPixel	= getPixel_24r;
					app->raster.setPixel	= setPixel_24r;					
				} 
				else 
				{
					app->raster.raster_cmap = raster_24;
					app->raster.raster_gray = gscale_24;
					app->raster.raster_true = dither_24;
					app->raster.getPixel	= getPixel_24;
					app->raster.setPixel	= setPixel_24;					
				}

				break;

			case 32:
				app->raster.cnvpal_color = cnvpal_true;

				if (reverse) 
				{
					app->raster.raster_cmap = raster_32r;
					app->raster.raster_gray = gscale_32r;
					app->raster.raster_true = dither_32r;
					app->raster.getPixel	= getPixel_32r;
					app->raster.setPixel	= setPixel_32r;					
				} 
				else if( z_trail) 
				{
					app->raster.raster_cmap = raster_32z;
					app->raster.raster_gray = gscale_32z;
					app->raster.raster_true = dither_32z;
					app->raster.getPixel	= getPixel_32z;
					app->raster.setPixel	= setPixel_32z;					
				} 
				else 
				{
					app->raster.raster_cmap = raster_32;
					app->raster.raster_gray = gscale_32;
					app->raster.raster_true = dither_32;
					app->raster.getPixel	= getPixel_32;
					app->raster.setPixel	= setPixel_32;					
				}

				break;	
		}

	if( plane == 4 || plane == 8) 
	{
		uint32	*dst = app->raster.cube216;
		uint32	r, g, b;

		for (r = 0x000000uL; r <= 0xFF0000uL; r += 0x330000uL) 
		{
			for (g = 0x000000uL; g <= 0x00FF00uL; g += 0x003300uL) 
			{
				for (b = 0x000000uL; b <= 0x0000FFuL; b += 0x000033uL) 
					*(dst++) = color_lookup( r | g | b, app->raster.pixel_val, app);
			}
		}

		dst = app->raster.graymap;

		for ( g = 0x000000uL; g <= 0xF8F8F8uL; g += 0x080808uL) 
			*( dst++) = color_lookup( g | (( g >> 5) & 0x030303uL), app->raster.pixel_val, app);
	}

	return( 0);
}

