#include "general.h"
#include "ztext.h"
#include "winfile/WinFile.h"



/*==================================================================================*
 * WinFile_Exec_Entry:																*
 *		Execute the action associated at an Entry.									*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		win_area   			-> the window area.										*
 * 		file_area  			-> the file area.										*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Exec_Entry( WINDOW *win, GRECT *win_area, GRECT *file_area, APPVar *app)
{
	int16 i;
	int32 error;
	TAB *tab = win->selected_tab;
	
	
	if( win->handle == 0) /* folder or disk on the desktop */
	{
		mt_graf_mouse( BUSYBEE, NULL, app->aes_global);
		WinFile_New( NULL, tab->first_selected->name, -1, -1, -1, -1, app);
		mt_graf_mouse( ARROW, NULL, app->aes_global);
	}
	else if( tab->first_selected->icon == ICON_FOLDER)
	{
		mt_graf_mouse( BUSYBEE, NULL, app->aes_global);
		
		error = tab->scan_dir( tab, tab->directory, tab->first_selected->name, app);

		if( error != 0L)
		{
			mt_graf_mouse( ARROW, NULL, app->aes_global);
			show_alert( NULL, error, app);
			return;
		}

		get_directory_name( tab->directory, tab->name, MAXNAMLEN);
		
		zstrncpy( tab->name_shown, tab->name, TAB_NAME_SHOW_CHARS);
		
		tab->tab_txt_w = get_text_width(( const char *)tab->name_shown);
		tab->real_txt_w = get_text_width(( const char *)tab->name);
		
		if( tab->tab_txt_w > ( tab->tab_width - MINI_ICON_SIZE - 8))
			tab->tab_txt_w = name_shorter( tab->tab_width - MINI_ICON_SIZE - 8, tab->name_shown);			
		
		mt_wind_set_str( win->handle, WF_NAME, tab->name, app->aes_global);

		tab->ypos = 0;

		WinFile_Update_Icon_Position( win, file_area, app);
		WinFile_Slider( win, ( int32)file_area->g_h, app);

		mt_graf_mouse( ARROW, NULL, app->aes_global);

		if( win->status & WS_TABBAR)
			file_area->g_h += TABBAR_HEIGHT;		
		
		*(GRECT*)&app->aes_buff[4] = *file_area;
		win->WinRedraw( win, app);		
		update_infobar( win, win_area, app);
	}
	else if( tab->first_selected->icon == ICON_BACK)
	{
		mt_graf_mouse( BUSYBEE, NULL, app->aes_global);
		
		error = tab->scan_dir( tab, tab->directory, "..", app);

		if( error != 0L)
		{
			mt_graf_mouse( ARROW, NULL, app->aes_global);
			show_alert( NULL, error, app);
			return;
		}

		tab->ypos = 0;
		
		for( i = 0 ; i < tab->nbr_icons; i++)
			if( strcmp( tab->entry[i].name, ( const char *)tab->name) == 0)
			{
				tab->first_selected = &tab->entry[i];
				break;
			}

		get_directory_name( tab->directory, tab->name, MAXNAMLEN);
		
		zstrncpy( tab->name_shown, tab->name, TAB_NAME_SHOW_CHARS);
		
		tab->tab_txt_w = get_text_width(( const char *)tab->name_shown);
		tab->real_txt_w = get_text_width(( const char *)tab->name);
		
		if( tab->tab_txt_w > ( tab->tab_width - MINI_ICON_SIZE - 8))
			tab->tab_txt_w = name_shorter( tab->tab_width - MINI_ICON_SIZE - 8, tab->name_shown);		
		
		mt_wind_set_str( win->handle, WF_NAME, tab->name, app->aes_global);
		
		WinFile_Update_Icon_Position( win, file_area, app);
		WinFile_Slider( win, ( int32)file_area->g_h, app);

		while( tab->first_selected->txt_pos.g_y + tab->first_selected->txt_pos.g_h > file_area->g_h)
		{
			if( tab->ypos == tab->ypos_max)
				break;

			tab->first_selected->txt_pos.g_h -= tab->h_u;
			tab->ypos++;
		}
		
		mt_graf_mouse( ARROW, NULL, app->aes_global);

		if( win->status & WS_TABBAR)
			file_area->g_h += TABBAR_HEIGHT;
			
		*(GRECT*)&app->aes_buff[4] = *file_area;
		win->WinRedraw( win, app);	
		update_infobar( win, win_area, app);
	}
}

