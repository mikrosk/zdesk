#include "general.h"
#include "ztext.h"
#include "misc.h"
#include "edit.h"
#include "icon/icon.h"
#include "file/sort.h"
#include "file/file.h"
#include "winfile/WinFile.h"


/*==================================================================================*
 * void rename_dir_in_fullpath:														*
 *		If a directory is in a full path, we remane it.								*
 *----------------------------------------------------------------------------------*
 * 	path_to_dir:	The path to the directory.										*
 * 	dir:			The directory.													*
 * 	fullpath:		The full path.													* 
 *----------------------------------------------------------------------------------*
 * return: -1 is the dir is not in the full path else the dir positon in the path.	* 
 *==================================================================================*/

int	rename_dir_in_fullpath( WINDOW *win, TAB *tab, char *path_to_dir, char *dir, char *fullpath, char *new_name, APPVar *app)
{
	char *c1 = fullpath, *c2 = path_to_dir, *dir_position_txt, txt_buffer[MAX_PATH];
	int dir_position_nbr = 0;
	
	while( *c1 != '\0' && *c1 == *c2)
	{
		c1++; 
		c2++;
		
		if( *c1 == '\\')
			dir_position_nbr++;
	}
	
	if(( *c2 != '\0') || ( *c1 != dir[0]))
		return -1;
	
	c2 = dir;
	dir_position_txt = c1;
	
	while( *c1 != '\0' && *c1 == *c2)
	{
		c1++; 
		c2++;
	}
	
	if(( *c2 != '\0') || ( *c1 != '\\'))
		return -1;	
	
	// ---------
	
	// "dir" is the last directory in the path? We rename the tab
	if( c1[1] == '\0')
	{
		strcpy( tab->name, new_name);
		zstrncpy( tab->name_shown, tab->name, TAB_NAME_SHOW_CHARS);
		
		tab->tab_txt_w = get_text_width(( const char *)tab->name_shown);
		tab->real_txt_w = get_text_width(( const char *)tab->name);
		
		if( tab->tab_txt_w > ( tab->tab_width - MINI_ICON_SIZE - 8))
			tab->tab_txt_w = name_shorter( tab->tab_width - MINI_ICON_SIZE - 8, tab->name_shown);			
		
		if( tab == win->selected_tab)
			mt_wind_set_str( win->handle, WF_NAME, tab->name, app->aes_global);
	
		if( win->status & WS_TABBAR)
			redraw_tabbar( win, NULL, app);
	}	
	
	strcpy( txt_buffer, c1);
	strcpy( dir_position_txt, new_name);	
	strcat( dir_position_txt, txt_buffer);

	return dir_position_nbr;
}



/*==================================================================================*
 * static void change_entry_name:													*
 *		We change the entry name													*
 *----------------------------------------------------------------------------------*
 * win     				-> the target window.										*
 * tab					-> the target tab.											*
 * app					-> Application's descriptor.								*
 *----------------------------------------------------------------------------------*
 * return: -																		*
 *==================================================================================*/

static void change_entry_name( char *new_name, TAB *tab, Entry *entry, APPVar *app)
{
	zstrncpy( entry->name, new_name, MAXNAMLEN);
	zstrncpy( entry->name_shown, ( const char *)entry->name, ENTRY_NAME_SHOW_CHARS);
	
	if( !( entry->status & ES_LINK))
		set_icon( entry->name, entry, app);
	
	entry->icon_txt_w = entry->real_txt_w = get_text_width(( const char *)entry->name);
	
	if(( tab->view_mode == ICON_MODE) && ( entry->icon_txt_w > tab->w_u - ( app->x_space << 1)))
		entry->icon_txt_w = name_shorter( tab->w_u - ( app->x_space << 1), entry->name_shown);
	else if(( tab->view_mode == LIST_MODE) && ( entry->icon_txt_w > tab->name_column_width - ( entry->txt_pos.g_x + 1)))
		entry->icon_txt_w = name_shorter( tab->name_column_width - ( entry->txt_pos.g_x + 1), entry->name_shown);
}



/*==================================================================================*
 * rename_entry:																	*
 *		the ENTER key is pressed in edition mode, so we rename the entry if			*
 *		necessary and we update the tabs.											*
 *----------------------------------------------------------------------------------*
 * win     				-> the target window.										*
 * entry 				-> the target entry.										*
 * window_y, window_x 	-> the window's position.									*
 * app					-> Application's descriptor.								*
 *----------------------------------------------------------------------------------*
 * return: -																		*
 *==================================================================================*/

void rename_entry( WINDOW *win, GRECT *file_area, APPVar *app)
{
	WINDOW	*win_loop;
	TAB		*tab = win->selected_tab, *tab_loop;
	GRECT	win_loop_file_area, win_loop_full_area;
	int32	error;
	int		i, infobar_must_be_updated = FALSE;
	char	old_name[MAXNAMLEN];
	
	if( win->edit.buffer[0] == '\0')
	{
		exit_edit_mode( win, tab->first_selected, file_area->g_x, file_area->g_y, app);
		return;
	}
	
	for( i = 0; i < tab->nbr_icons; i++)
	{	
		if( strcasecmp(( const char *)tab->entry[i].name, ( const char *)win->edit.buffer) == 0)
		{
			exit_edit_mode( win, tab->first_selected, file_area->g_x, file_area->g_y, app);
			return;
		}
	}	
	
	/* TODO: For a FTP dir or an archive, we don't need to do that */
	mychdir( tab->directory);
	
	error = tab->rename_entry( tab, win->edit.buffer, app);
	
	if( error != 0L)
	{
		exit_edit_mode( win, tab->first_selected, file_area->g_x, file_area->g_y, app);
		show_alert( NULL, error, app);
		return;
	}
	
	strcpy( old_name, tab->first_selected->name);
	
	change_entry_name( win->edit.buffer, tab, tab->first_selected, app);
	
	if(( tab->sortingmode == MENU_BAR_BY_NAME) && ( tab->nbr_icons > 1) && ( win->handle != 0))
	{
		int16 old_pos = return_entry_nbr( tab, tab->first_selected);
		
		sort_entries( tab);
		
		for( i = 0; i < tab->nbr_icons; i++)
		{
			if( strcmp(( const char *)tab->entry[i].name, ( const char *)win->edit.buffer) == 0)
				break;
		}	
		
		if( i != old_pos)
		{
			tab->first_selected = &tab->entry[i];
			win->edit.edition_mode = FALSE;
			
			WinFile_Update_Icon_Position( win, file_area, app);
			
			if( tab->first_selected->box_pos.g_y < 0)
			{
				while( tab->first_selected->box_pos.g_y < 0)
				{
					if( tab->ypos == 0)
						break;
					
					tab->first_selected->box_pos.g_y += tab->h_u;
					tab->ypos--;
				}
				
				WinFile_Slider( win, ( int32)file_area->g_h, app);
			}
			else if( tab->first_selected->box_pos.g_y + tab->first_selected->box_pos.g_h > file_area->g_h)
			{	
				while( tab->first_selected->box_pos.g_y + tab->first_selected->box_pos.g_h > file_area->g_h)
				{
					if( tab->ypos == tab->ypos_max)
						break;
					
					tab->first_selected->box_pos.g_y -= tab->h_u;
					tab->ypos++;
				}
				
				WinFile_Slider( win, ( int32)file_area->g_h, app);
			}
			
			*(GRECT*)&app->aes_buff[4] = *file_area;
			win->WinRedraw( win, app);
		}
		else
			exit_edit_mode( win, tab->first_selected, file_area->g_x, file_area->g_y, app);
	}
	else
		exit_edit_mode( win, tab->first_selected, file_area->g_x, file_area->g_y, app);
	

	for( win_loop = app->win; win_loop; win_loop = win_loop->next)
	{
		for( tab_loop = win_loop->tab; tab_loop; tab_loop = tab_loop->next)
		{	
			if( tab_loop == tab)
				continue;
			
			if( strcmp(( const char *)tab_loop->directory, ( const char *)tab->directory) == 0)
			{
				for( i = 0; i < tab_loop->nbr_icons; i++)
				{	
					if( strcmp(( const char *)tab_loop->entry[i].name, ( const char *)old_name) == 0)
					{
						WinFile_Calc_Areas( win_loop, &win_loop_full_area, &win_loop_file_area, app);
						 
						change_entry_name( win->edit.buffer, tab_loop, &tab_loop->entry[i], app);

						if(( tab_loop->sortingmode == MENU_BAR_BY_NAME) && ( tab_loop->nbr_icons > 1) && ( win_loop->handle != 0))
						{
							tab_loop->ypos = 0;
							
							if( tab_loop->first_selected)
							{
								while( tab_loop->first_selected)
									remove_selected_entry( tab_loop, tab_loop->first_selected);
								
								infobar_must_be_updated = TRUE;
							}
							
							sort_entries( tab_loop);
							
							if( tab_loop == win_loop->selected_tab)
							{
								if( infobar_must_be_updated == TRUE)
									update_infobar( win_loop, &win_loop_full_area, app);
									
								redraw_area( win_loop, 0, 0, &win_loop_file_area, app);
								WinFile_Slider( win_loop, ( int32)win_loop_file_area.g_h, app);
							}
							
							infobar_must_be_updated = FALSE;
						}
						else if( tab_loop == win_loop->selected_tab) 
							redraw_area( win_loop, win_loop_file_area.g_x, win_loop_file_area.g_y, &tab_loop->entry[i].box_pos, app);
						
						break;
					}
				}
			}
			else
			{ 
				rename_dir_in_fullpath( win_loop, tab_loop, tab->directory, old_name, tab_loop->directory, win->edit.buffer, app);
			}
		}
	}
}


