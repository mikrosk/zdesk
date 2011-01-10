#include "general.h"
#include "ztext.h"
#include "winfile/WinFile.h"


/*==================================================================================*
 * void WinFile_Column_Sizer:														*
 *		Check if the title column needs to be resized and if necessary,				*
 *		resize it.																	*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win				-> The window to handle.									*
 * 		mousex, mousex	-> The mouse position in the working area.					*
 * 		working_area	-> The working area( file area + column title area).		* 
 * 		app				-> Application's descriptor.								*
 *----------------------------------------------------------------------------------*
 * return:	 																		*
 *		--																			*
 *==================================================================================*/

void WinFile_Column_Sizer( WINDOW *win, int16 mousex, int16 mousey, GRECT *working_area, APPVar *app)
{
	int16 i, new_xpos, mx;
	TAB *tab = win->selected_tab;
	
	mt_graf_mkstate( &app->mx, &app->my, &app->mbut, &app->mkstate, app->aes_global);
	
	if( app->mbut != 1)
		return;
	
	if( IS_IN( mousex, mousey, tab->name_column_width - 3, 1, 6, COLUMN_TITLE_HEIGHT - 2))
	{
		if( app->pref.resize_column_in_realtime == FALSE) 
		{
			mt_graf_dragbox( 1, working_area->g_h, working_area->g_x + tab->name_column_width, working_area->g_y, working_area->g_x + 60, working_area->g_y, working_area->g_w - 62, working_area->g_h, &new_xpos, &i, app->aes_global);
			
			if( new_xpos != working_area->g_x + tab->name_column_width)
			{
				tab->name_column_width = new_xpos - working_area->g_x;
				
				for( i = 0; i < tab->nbr_icons; i++)
				{
					if( tab->entry[i].icon_txt_w != tab->entry[i].real_txt_w)
					{
						zstrncpy( tab->entry[i].name_shown, ( const char *)tab->entry[i].name, ENTRY_NAME_SHOW_CHARS);
						tab->entry[i].icon_txt_w = tab->entry[i].real_txt_w;
					}
					
					if( tab->entry[i].icon_txt_w > tab->name_column_width - ( 17 + app->x_space + app->x_space))
						tab->entry[i].icon_txt_w = name_shorter( tab->name_column_width - ( 17 + app->x_space + app->x_space), tab->entry[i].name_shown);
				}			

				*(GRECT*)&app->aes_buff[4] = *working_area;
				win->WinRedraw( win, app);
			}
		}
		else
		{
			mx = mousex;
		
			do
			{
				if(( app->mx != working_area->g_x + mx) && ( app->mx >= working_area->g_x + 60) && ( app->mx <= working_area->g_x + working_area->g_w - 2))
				{
					mx = app->mx - working_area->g_x;
					
					tab->name_column_width = mx;
				
					for( i = 0; i < tab->nbr_icons; i++)
					{
						if( tab->entry[i].icon_txt_w != tab->entry[i].real_txt_w)
						{
							zstrncpy( tab->entry[i].name_shown, ( const char *)tab->entry[i].name, ENTRY_NAME_SHOW_CHARS);
							tab->entry[i].icon_txt_w = tab->entry[i].real_txt_w;
						}
					
						if( tab->entry[i].icon_txt_w > tab->name_column_width - ( 17 + app->x_space + app->x_space))
							tab->entry[i].icon_txt_w = name_shorter( tab->name_column_width - ( 17 + app->x_space + app->x_space), tab->entry[i].name_shown);
					}			
				
					*(GRECT*)&app->aes_buff[4] = *working_area;
					win->WinRedraw( win, app);
				}
			
				mt_graf_mkstate( &app->mx, &app->my, &app->mbut, &app->mkstate, app->aes_global);	
				
			}while( app->mbut == 1);
		}		
	}
	else if( tab->size_column_width && IS_IN( mousex, mousey, tab->name_column_width + tab->size_column_width - 3, 1, 6, COLUMN_TITLE_HEIGHT - 2))		
	{
		if( app->pref.resize_column_in_realtime == FALSE) 
		{		
			mt_graf_dragbox( 1, working_area->g_h, working_area->g_x + tab->name_column_width + tab->size_column_width, working_area->g_y, working_area->g_x + tab->name_column_width + 62, working_area->g_y, working_area->g_w - ( tab->name_column_width + 64), working_area->g_h, &new_xpos, &i, app->aes_global);
		
			if( new_xpos != working_area->g_x + tab->name_column_width + tab->size_column_width)
			{
				tab->size_column_width = new_xpos - ( working_area->g_x + tab->name_column_width);
			
				*(GRECT*)&app->aes_buff[4] = *working_area;
				win->WinRedraw( win, app);
			}
		}	
		else
		{
			mx = mousex;
			
			do
			{
				if(( app->mx != working_area->g_x + mx) && ( app->mx >= working_area->g_x + tab->name_column_width + 62) && ( app->mx <= working_area->g_x + working_area->g_w - 2))
				{
					mx = app->mx - working_area->g_x;
					
					tab->size_column_width = mx - tab->name_column_width;
					
					*(GRECT*)&app->aes_buff[4] = *working_area;
					win->WinRedraw( win, app);
				}
				
				mt_graf_mkstate( &app->mx, &app->my, &app->mbut, &app->mkstate, app->aes_global);	
				
			}while( app->mbut == 1);
		}			
	}
	else if( tab->date_column_width && IS_IN( mousex, mousey, tab->name_column_width + tab->size_column_width + tab->date_column_width - 3, 1, 6, COLUMN_TITLE_HEIGHT - 2))		
	{	
		if( app->pref.resize_column_in_realtime == FALSE) 
		{			
			mt_graf_dragbox( 1, working_area->g_h, working_area->g_x + tab->name_column_width + tab->size_column_width + tab->date_column_width, working_area->g_y, working_area->g_x + tab->name_column_width + tab->size_column_width + 105, working_area->g_y,  working_area->g_w - ( tab->name_column_width + tab->size_column_width + 107), working_area->g_h, &new_xpos, &i, app->aes_global);
		
			if( new_xpos != working_area->g_x + tab->name_column_width + tab->size_column_width + tab->date_column_width)
			{
				tab->date_column_width = new_xpos - ( working_area->g_x + tab->name_column_width + tab->size_column_width);
		
				*(GRECT*)&app->aes_buff[4] = *working_area;
				win->WinRedraw( win, app);
			}
		}	
		else
		{
			mx = mousex;
			
			do
			{
				if(( app->mx != working_area->g_x + mx) && ( app->mx >= working_area->g_x + tab->name_column_width + tab->size_column_width + 105) && ( app->mx <= working_area->g_x + working_area->g_w - 2))
				{
					mx = app->mx - working_area->g_x;
					
					tab->date_column_width = mx - tab->name_column_width - tab->size_column_width;
					
					*(GRECT*)&app->aes_buff[4] = *working_area;
					win->WinRedraw( win, app);
				}
				
				mt_graf_mkstate( &app->mx, &app->my, &app->mbut, &app->mkstate, app->aes_global);	
				
			}while( app->mbut == 1);
		}		
	}	
}


