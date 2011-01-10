#include "general.h"
#include "custom_font.h"


/*==================================================================================*
 * draw_text:																		*
 *		draw a text with our custom font on the screen.								*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		xf, yf     			-> the text position.									*
 * 		txt_w     			-> the text width. If -1, the width will be computed.	* 
 * 		color     			-> the text color.										* 
 * 		shadow_color		-> the shadow color. If -1, there will be no shadow.	*  
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

int16 CDECL draw_text( int16 xf, int16 yf, int16 txt_w, int16 color, int16 shadow_color, const char *str, APPVar *app)
{
	int16 txt_width = txt_w;
	int c, character_pixel_skip = 0;	
	
	if( txt_width == -1)
	{
		const char *string = str;
		
		txt_width = 0;
		
		while(( c = *string++) != 0)
		{	
			if( c < 0)
				c += 256;
			
			txt_width += ofwf[c];
		}	
	}
	
	if(( txt_width == 0) || ( str[0] == '\0') || ( txt_width > 1000))
		return txt_width;
	
	app->pxy[0] = 0;
	app->pxy[1] = 0;
	app->pxy[2] = txt_width - 1;
	app->pxy[3] = 7;
	
	app->pic.fd_addr	= app->buffer;
	app->pic.fd_w		= txt_width;
	app->pic.fd_h		= 8;
	app->pic.fd_wdwidth	= ( txt_width >> 4) + (( txt_width % 16) != 0);
	app->pic.fd_nplanes	= 1;
	
	app->src_line_octets = app->pic.fd_wdwidth << 1;
	
	memset( app->buffer, 0, app->src_line_octets << 3);
	
	while((( c = *str++) != 0) && ( character_pixel_skip < txt_width))
	{
		register uint8 *character;
		
		if( c < 0)
			c += 256;
		
		character = ( uint8*)&my_font_8[(c - 1) << 1];
		
		if( character_pixel_skip & 7)
		{
			register uint16 *dest = ( uint16*)( app->buffer + ( character_pixel_skip >> 3));
			register int pixel_shift = 8 - ( character_pixel_skip & 7);			 
			
			*dest |= (( *character++) << pixel_shift);
			dest  += app->pic.fd_wdwidth;
			*dest |= (( *character++) << pixel_shift);
			dest  += app->pic.fd_wdwidth;
			*dest |= (( *character++) << pixel_shift);
			dest  += app->pic.fd_wdwidth;
			*dest |= (( *character++) << pixel_shift);
			dest  += app->pic.fd_wdwidth;
			*dest |= (( *character++) << pixel_shift);
			dest  += app->pic.fd_wdwidth;
			*dest |= (( *character++) << pixel_shift);
			dest  += app->pic.fd_wdwidth;
			*dest |= (( *character++) << pixel_shift);
			dest  += app->pic.fd_wdwidth;
			*dest |= (( *character) << pixel_shift);			
		}
		else
		{
			register uint8 *dest = app->buffer + ( character_pixel_skip >> 3);
			
			*dest = *character++;
			dest += app->src_line_octets;
			*dest = *character++;
			dest += app->src_line_octets;
			*dest = *character++;
			dest += app->src_line_octets;
			*dest = *character++;
			dest += app->src_line_octets;
			*dest = *character++;
			dest += app->src_line_octets;
			*dest = *character++;
			dest += app->src_line_octets;
			*dest = *character++;
			dest += app->src_line_octets;
			*dest = *character;
		}	
		
		character_pixel_skip += ofwf[c];
	}
	
	app->color[1] = WHITE;	
	
	app->pxy[4] = xf;
	app->pxy[6] = xf + txt_width - 1;
	
	if( shadow_color > -1)
	{
		// app->pxy[4] = xf + 1;
		app->pxy[5] = yf + 1;
		// app->pxy[6] = xf + txt_width;
		app->pxy[7] = yf + 8;
		
		app->color[0] = shadow_color;
		
		vrt_cpyfm( app->vdi_handle, 2, app->pxy, &app->pic, &app->screen, app->color);
	}
	
	// app->pxy[4] = xf;
	app->pxy[5] = yf;
	// app->pxy[6] = xf + txt_width - 1;
	app->pxy[7] = yf + 7;
	
	app->color[0] = color;
	
	vrt_cpyfm( app->vdi_handle, 2, app->pxy, &app->pic, &app->screen, app->color);	
	
	return txt_width;
}
