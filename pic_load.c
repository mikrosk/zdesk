#include "general.h"
#include "codecs.h"
#include "mfdb.h"
#include "ztext.h"
#include "raster/color.h"
#include "raster/raster.h"
#include "raster/vdi.h"
#include <math.h>


/*==================================================================================*
 * pic_setup:																		*
 *			.																		*
 *----------------------------------------------------------------------------------*
 * return: 	-																		*
 *==================================================================================*/

static int pic_setup( MFDB *img, dec_data *data, APPVar *app)
{
	int16	n_planes = (( data->info.planes == 1 && data->info.components == 1) ?  1: app->nplanes);
	uint16	display_w, display_h;
	size_t	src_line_size;

	if( data->info.thumbnail && ( data->info.width > 48 || data->info.height > 32) && ( !app->pref.smooth_thumbnail || n_planes < 16))
	{
		double factor	 = MAX((( double)data->info.height / 32.0), (( double)data->info.width / 48.0));
		double precise_x = ( double)data->info.width  / factor;
		double precise_y = ( double)data->info.height / factor;

		display_w 		= ( uint16)( precise_x > 0 ? precise_x : 16);
		display_h 		= ( uint16)( precise_y > 0 ? precise_y : 16);
		data->IncXfx    = ((( uint32)data->info.width  << 16) + ( display_w >> 1)) / display_w;
		data->IncYfx    = ((( uint32)data->info.height << 16) + ( display_h >> 1)) / display_h;
	}
	else
	{
		display_w 		= data->info.width;
		display_h 		= data->info.height;
		data->IncXfx    = 0x10000uL;
		data->IncYfx	= 0x10000uL;
	}

	if( !init_mfdb( img, display_w, display_h, n_planes))
		return( 0);

	src_line_size = ( data->info.width + 64) * data->info.components;

	data->RowBuf = ( uint8*)app->ldg_mem.ldg_malloc( src_line_size, ( int32)app->aes_global[2]);

	if( !data->RowBuf)
		return ( 0);

	if(( data->info.planes == 1 && data->info.components == 1) || app->nplanes > 8)
		data->DthBuf = NULL;
	else
	{
		size_t size = ( display_w + 15) * 3;

		if(( data->DthBuf = app->ldg_mem.ldg_malloc( size, ( int32)app->aes_global[2])) == NULL)
			return ( 0);
			
		memset( data->DthBuf, 0, size);
	}

	data->DthWidth 	= display_w;
	data->PixMask  	= ( 1 << data->info.planes) - 1;
	data->LnSize   	= img->fd_wdwidth * n_planes;

	if( data->info.planes == 1 && data->info.components == 1) 
	{
		cnvpal_mono( &data->info, data, app);
		app->raster.raster = raster_mono;
	}
	else
	{
		if( data->info.indexed_color)
		{
			app->raster.cnvpal_color( &data->info, data, app);
			app->raster.raster = app->raster.raster_cmap;
		}
		else
			app->raster.raster = ( data->info.components >= 3 ? app->raster.raster_true : app->raster.raster_gray);
	}

	return( 1);
}



/*==================================================================================*
 * read_img:																		*
 *			Read the picture and put the data in the MFDB.							*
 *----------------------------------------------------------------------------------*
 * return: 	-																		*
 *==================================================================================*/

static inline void read_img( MFDB *img, dec_data *data, APPVar *app)
{
	uint16 		*dst = ( uint16*)img->fd_addr;
	int16  		y_dst = img->fd_h, y, img_h = data->info.height;
	int32		line_size = data->LnSize;
	uint8  		*buf = data->RowBuf;
	uint32 		scale = ( data->IncYfx + 1) >> 1;

	if( data->info.orientation == DOWN_TO_UP)
	{
		dst += data->LnSize * ( y_dst - 1);
		line_size = -data->LnSize;
	}

	data->info.page_wanted = 0;

	for( y = 1; y <= img_h; y++)
	{
		if( !app->codecs.decoder_read( &data->info, buf))
			break;

		while(( scale >> 16) < y)
		{
			app->raster.raster( data, dst, app);
			dst   += line_size;
			scale += data->IncYfx;
			if (!--y_dst) break;
		}
	}
}



/*==================================================================================*
 * quit_img:																		*
 *			Close the codec and free the memory.									*
 *----------------------------------------------------------------------------------*
 * return: 	-																		*
 *==================================================================================*/

static void quit_img( dec_data *data, APPVar *app)
{
	app->codecs.decoder_quit( &data->info);

	if( data->DthBuf) 
	   	app->ldg_mem.ldg_free( data->DthBuf, ( int32)app->aes_global[2]);

	if( data->RowBuf) 
	   app->ldg_mem.ldg_free( data->RowBuf, ( int32)app->aes_global[2]);

	app->ldg_mem.ldg_free( data, ( int32)app->aes_global[2]);
}



/*==================================================================================*
 * get_pic_info:																	*
 * 			Find and initialize the right codec.									*
 *----------------------------------------------------------------------------------*
 * file			-> The file to load.												*
 * info			-> The struct. where to put the information. 		 				*
 * app			-> appication's descriptor.											*
 *----------------------------------------------------------------------------------*
 * return:	TRUE if ok, FALSE if error.												*
 *==================================================================================*/

static int get_pic_info( const char *file, img_info	*info, APPVar *app)
{
	int 	i, j;
	char 	*extention_ptr, *plugin_name, extention[4];

	extention_ptr = strrchr( file + 1, ( int)'.');
	
	if( extention_ptr == NULL)
		return FALSE;
	
	extention_ptr++;
	
	if( extention_ptr[0] == '\0')
		return FALSE;
	
	zstrncpy( extention, extention_ptr, 4);
	strupr( extention);
	
	/* We check if a plug-ins can do the job */
	for( i = 0; i < app->codecs.codecs_nbr; i++)
	{
		plugin_name = app->codecs.codec_ptr[i]->infos;

		for( j = 0; j < app->codecs.codec_ptr[i]->user_ext; j++)
		{
			if( strncmp( extention, plugin_name, 3) == 0)
			{
				if( !( app->codecs.decoder_init = ldg_find( "reader_init", app->codecs.codec_ptr[i]))
				 || !( app->codecs.decoder_read = ldg_find( "reader_read", app->codecs.codec_ptr[i]))
				 || !( app->codecs.decoder_quit = ldg_find( "reader_quit", app->codecs.codec_ptr[i])))
					return FALSE;

				return app->codecs.decoder_init( file, info);
			}

			plugin_name += 3;
		}
	}

	/* I wish that it will never append ! */
	return FALSE;
}



/*==================================================================================*
 * pic_load:																		*
 *			Load and convert a picture in screen format.							*
 *----------------------------------------------------------------------------------*
 * file			-> The file to load.												*
 * thumbnail	-> thumbnail mode ?													*
 * img			-> The MFDB where to put the picture.		 		 				*
 * app			-> application's descriptor.										*
 *----------------------------------------------------------------------------------*
 * return:	'0' if ok else GEMDOS error code.										*
 *==================================================================================*/

int pic_load( const char *file, int16 thumbnail, MFDB *img, APPVar *app)
{
	dec_data *data;

	if( app->codecs.codecs_nbr == 0)
		return 0;

	data = ( dec_data *)app->ldg_mem.ldg_malloc( sizeof( dec_data), ( int32)app->aes_global[2]);

	if( data == NULL)
		return -ENOMEM;

	/* We initialise some variables needed by the codecs */
	data->info.background_color	= 0xFFFFFF;
	data->info.thumbnail		= thumbnail;

	if( get_pic_info( file, &data->info, app) == FALSE)
	{
		app->ldg_mem.ldg_free( data, ( int32)app->aes_global[2]);
		return -1;
	}

	if( !pic_setup( img, data, app))
	{
		quit_img( data, app);
		delete_mfdb( img);
		return -ENOMEM;
	}

	read_img( img, data, app);

	quit_img( data, app);

	if( thumbnail && ( img->fd_w > 48 || img->fd_h > 32) && app->pref.smooth_thumbnail && img->fd_nplanes >= 16)
	{
		MFDB 	resized_image;
		double	precise_x, precise_y, factor;
		uint16	display_w, display_h;

		factor	  = MAX((( double)img->fd_h / 32.0), (( double)img->fd_w / 48.0));
		precise_x = ( double)img->fd_w / factor;
		precise_y = ( double)img->fd_h / factor;
		display_w = ( uint16)( precise_x > 0 ? precise_x : 16);
		display_h = ( uint16)( precise_y > 0 ? precise_y : 16);

		init_mfdb( &resized_image, display_w, display_h, img->fd_nplanes);

		smooth_resize( img, &resized_image, app);

		gfree( img->fd_addr);

		img->fd_addr	= resized_image.fd_addr;
		img->fd_w 		= resized_image.fd_w;
		img->fd_h 		= resized_image.fd_h;
		img->fd_wdwidth	= resized_image.fd_wdwidth;
	}

	return 0;
}

