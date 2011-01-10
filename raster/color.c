#include "general.h"
#include "pic_load.h"
#include "raster/color.h"


static uint8 saturation (uint8 * rgb)
{
	uint8 satr;

	if( rgb[0] >= rgb[1]) 
	{
		if( rgb[1] >= rgb[2])   
			satr = rgb[0] - rgb[2];
		else if (rgb[0] > rgb[2]) 
			satr = rgb[0] - rgb[1];
		else
			satr = rgb[2] - rgb[1];
	} 
	else if (rgb[1] >= rgb[2]) 
	{
		if (rgb[0] >= rgb[2])
	    	satr = rgb[1] - rgb[2];
		else
			satr = rgb[1] - rgb[0];
	} 
	else 
		satr = rgb[2]  - rgb[0];

	return satr;
}


void save_colors( APPVar *app)
{
	int16 i, colors = 1 << app->nplanes;
	int16 coltab[3];

	for( i = 0; i < colors; i++)
	{
		vq_color( app->vdi_handle, i, COLOR_ACTUAL, coltab);
		app->raster.screen_colortab[i].red   = ((((int32)coltab[0] << 8 ) - coltab[0]) + 500) / 1000;
		app->raster.screen_colortab[i].green = ((((int32)coltab[1] << 8 ) - coltab[1]) + 500) / 1000;
		app->raster.screen_colortab[i].blue  = ((((int32)coltab[2] << 8 ) - coltab[2]) + 500) / 1000;
		app->raster.screen_colortab[i].satur = saturation ( &app->raster.screen_colortab[i].red);
	}
}


int16 remap_color (int32 value, APPVar *app)
{
	int16 red   = ((uint8*)&value)[1];
	int16 green = ((uint8*)&value)[2];
	int16 blue  = ((uint8*)&value)[3];
	int16 satur = saturation (((uint8*)&value) +1);
	
	int16  best_fit  = 0;
	uint16 best_err  = 0xFFFFu;
	int16  max_color = 1 << app->nplanes;
	
	int16 i = 0;
	
	value = ( value & 0x00FFFFFFl) | (( int32)satur << 24);
	
	do 
	{
		if ( *( int32*)&app->raster.screen_colortab[i] == value) 
		{
			/* gotcha! */
			best_fit = i;
			break;
		} 
		else 
		{
			uint16 err = 
			( red > app->raster.screen_colortab[i].red ? red - app->raster.screen_colortab[i].red : app->raster.screen_colortab[i].red - red)
			+ (green > app->raster.screen_colortab[i].green ? green - app->raster.screen_colortab[i].green : app->raster.screen_colortab[i].green - green)
			+ (blue  > app->raster.screen_colortab[i].blue  ? blue  - app->raster.screen_colortab[i].blue  : app->raster.screen_colortab[i].blue - blue)
			+ (satur > app->raster.screen_colortab[i].satur ? satur - app->raster.screen_colortab[i].satur : app->raster.screen_colortab[i].satur - satur);
			
			if (err <= best_err) 
			{
				best_err = err;
				best_fit = i;
			}
		}
	} while (++i < max_color);
	
	return best_fit;
}


uint32 color_lookup ( uint32 rgb, int16 *trans, APPVar *app)
{
	uint8 idx = ((rgb & ~0xFFuL) == ~0xFFuL ? rgb : remap_color( rgb, app));
	return ( (( int32)(trans ? trans[idx] : idx) << 24) | (*(int32*)&app->raster.screen_colortab[idx] & 0x00FFFFFFuL));
}


void cnvpal_mono ( img_info * info, dec_data * data, APPVar *app)
{
	data->Pixel[0] = G_WHITE;
	data->Pixel[1] = G_BLACK;
}


void cnvpal_1_2 (img_info * info, dec_data * data, APPVar *app)
{
	register int32 i;

	for( i = 0; i < info->colors; i++)
		data->Pixel[i] = ( int16)info->palette[i].red * 5 + ( int16)info->palette[i].green * 9 + (( int16)info->palette[i].blue << 1);
}


void cnvpal_4_8( img_info * info, dec_data * data, APPVar *app)
{
	register int32 i;

	for( i = 0; i < info->colors; i++)
	{
	    uint32 rgb = (((( int32)info->palette[i].red << 8) | info->palette[i].green) << 8) | info->palette[i].blue;
		data->Pixel[i] = rgb | (( int32)app->raster.pixel_val[remap_color( rgb, app)] << 24);
	}
}


void cnvpal_15( img_info * info, dec_data * data, APPVar *app)
{
	register int32 i;

	for( i = 0; i < info->colors; i++)
		data->Pixel[i] = (( int16)( info->palette[i].red & 0xF8) << 7) | (( int16)( info->palette[i].green & 0xF8) << 2) | ( info->palette[i].blue >> 3);
}


void cnvpal_high( img_info * info, dec_data * data, APPVar *app)
{
	register int32 i;

	for( i = 0; i < info->colors; i++)
		data->Pixel[i] = (( int16)( info->palette[i].red & 0xF8) << 8) | (( int16)( info->palette[i].green & 0xFC) << 3) | ( info->palette[i].blue >> 3);
}


void cnvpal_true ( img_info * info, dec_data * data, APPVar *app)
{
	register int32 i;
	
	for( i = 0; i < info->colors; i++)
		data->Pixel[i] = (((( int32)info->palette[i].red << 8) | info->palette[i].green) << 8) | info->palette[i].blue;
}
