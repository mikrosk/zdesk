#include "general.h"
#include "ztext.h"
#include "edit.h"
#include "winfile/WinFile.h"


/*==================================================================================*
 * WinFile_Update_Icon_Position:													*
 *		Update icons position.														*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		rect 				-> the files area.										* 
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Update_Icon_Position( WINDOW *win, GRECT *rect, APPVar *app)
{
	int16 	i, x, y;
	TAB *tab				= win->selected_tab;
	tab->xpos_max 			= 0;
	tab->icons_last_line 	= 0;
	tab->ypos_max 			= 1;
	x 						= 0;
	y 						= -(( int16)tab->ypos * tab->h_u);

	
	if( tab->view_mode == ICON_MODE)	
	{
		for( i = 0; i < tab->nbr_icons; i++)
		{
			if(( tab->entry[i].status & ES_HIDDEN) && !app->pref.show_hidden)
				continue;

			if(( x + tab->w_u > rect->g_w) && ( i > 0))
			{
				x = 0;
				y += tab->h_u;		
				tab->ypos_max++;
				tab->icons_last_line = 1;
			}
			else
			{
				tab->icons_last_line++;

				if(( int16)tab->xpos_max < tab -> icons_last_line)
					tab->xpos_max++;
			}
		
			/* save the case position */
			tab->entry[i].box_pos.g_x = x;
			tab->entry[i].box_pos.g_y = y;
			tab->entry[i].box_pos.g_w = tab->w_u;
			tab->entry[i].box_pos.g_h = tab->h_u;
	
			/* save the icon position */
			if( tab->entry[i].thumbnail.fd_addr) 
			{
				tab->entry[i].icn_pos.g_w = tab->entry[i].thumbnail.fd_w;
				tab->entry[i].icn_pos.g_h = tab->entry[i].thumbnail.fd_h;
			}
			else 
			{
				tab->entry[i].icn_pos.g_w = tab->icon_size;
				tab->entry[i].icn_pos.g_h = tab->icon_size;
			}
			
			tab->entry[i].icn_pos.g_x = x + (( tab->w_u - tab->entry[i].icn_pos.g_w + 1) >> 1);
			tab->entry[i].icn_pos.g_y = y + app->y_space + (( tab->icon_size - tab->entry[i].icn_pos.g_h + 1) >> 1);
			
			/* save the text position */
			tab->entry[i].txt_pos.g_y = y + tab->h_u - app->y_space - app->hcell;
			tab->entry[i].txt_pos.g_h = app->hcell;

			if(( win->edit.edition_mode == TRUE) && tab->first_selected == &tab->entry[i])
			{
				tab->entry[i].txt_pos.g_w = get_text_width(( const char *)win->edit.buffer);
				tab->entry[i].txt_pos.g_x = x + (( tab->w_u - tab->entry[i].txt_pos.g_w + 1) >> 1);
			}
			else 
			{
				tab->entry[i].txt_pos.g_x = x + (( tab->w_u - tab->entry[i].icon_txt_w + 1) >> 1);
				tab->entry[i].txt_pos.g_w = tab->entry[i].icon_txt_w;
			}

			x += tab->w_u;
		}
	}
	else
	{
		for( i = 0; i < tab->nbr_icons; i++)
		{
			if(( tab->entry[i].status & ES_HIDDEN) && !app->pref.show_hidden)
				continue;
			
			if( i > 0)
			{
				y += tab->h_u;		
				tab->ypos_max++;
				tab->icons_last_line = 1;
			}
			
			/* save the case position */
			tab->entry[i].box_pos.g_x	= 0;
			tab->entry[i].box_pos.g_y	= y;
			tab->entry[i].box_pos.g_w	= rect->g_w;
			tab->entry[i].box_pos.g_h	= tab->h_u;
			
			/* save the icon position */
			tab->entry[i].icn_pos.g_x	= app->x_space;
			tab->entry[i].icn_pos.g_y	= y + (( tab->h_u - tab->icon_size - 1) >> 1);		
			tab->entry[i].icn_pos.g_w	= tab->icon_size;
			tab->entry[i].icn_pos.g_h	= tab->icon_size;
			
			/* save the text position */
			tab->entry[i].txt_pos.g_x	= tab->entry[i].icn_pos.g_w + app->x_space + app->x_space;
			tab->entry[i].txt_pos.g_y	= y + (( tab->h_u - app->hcell) >> 1);
			tab->entry[i].txt_pos.g_h	= app->hcell;
			
			if(( win->edit.edition_mode == TRUE) && tab->first_selected == &tab->entry[i])
				tab->entry[i].txt_pos.g_w = get_text_width(( const char *)win->edit.buffer);
			else
				tab->entry[i].txt_pos.g_w = tab->entry[i].icon_txt_w;
		}
	}
}


