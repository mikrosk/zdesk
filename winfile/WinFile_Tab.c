#include "general.h"
#include "file/file.h"
#include "file/rename.h"
#include "file/get_disk_size.h"
#include "winfile/WinFile.h"
#include "ztext.h"
#include "misc.h"
#include "menu.h"
#include "evnt_loop.h"
#include "mfdb.h"


/*==================================================================================*
 * TAB *WinFile_NewTab:																*
 *		Create a new tab.															*
 *----------------------------------------------------------------------------------*
 * 	win						-> Pointer to the target window.						*
 * 	current_fullpath_dir	-> Full path to the parent directory.					* 
 * 	new_dir					-> The directory to scan.								*
 * 	tabbar_width			-> The width of the tabs bar( can be -1).				* 
 * 	app						-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:																			* 
 *		The pointer to the tab created.												*
 *==================================================================================*/

TAB *WinFile_NewTab( WINDOW *win, char *current_fullpath_dir, char *new_dir, int16 tabbar_width, APPVar *app)
{
	TAB		*NewTab, *last_tab;
	int32	error;
	

	NewTab = ( TAB*)gmalloc( sizeof( TAB));

	if( NewTab == NULL)
	{
		show_alert( NULL, -ENOMEM, app);
		return NULL;
	}

	NewTab->tab_xpos				= 0;
	NewTab->tab_width				= MAXIMAL_TAB_WIDTH;	
	NewTab->tab_icon				= ICON_FOLDER_MINI;
	NewTab->name_column_width		= 150;
	NewTab->size_column_width		= 65;
	NewTab->date_column_width		= 110;
	NewTab->xpos					= 0;
	NewTab->ypos					= 0;
	NewTab->xpos_max				= 1;
	NewTab->ypos_max				= 1;
	NewTab->icon_size				= STANDARD_ICON_SIZE;
	NewTab->h_u						= NewTab->icon_size + app->hcell + app->y_space + app->y_space + app->y_space;
	NewTab->w_u						= NewTab->icon_size + NewTab->icon_size + 4 + app->x_space + app->x_space;
	NewTab->sortingmode				= app->pref.sortingmode;
	NewTab->sort_dirs_before_files	= app->pref.sort_dirs_before_files;
	NewTab->view_mode				= app->pref.view_mode;
	NewTab->entry					= NULL;
	NewTab->nbr_icons				= 0;
	NewTab->scan_dir				= scan_dir_file;
	NewTab->rename_entry			= rename_entry_file;
	NewTab->get_disk_size			= get_disk_size_file;

	
	error = NewTab->scan_dir( NewTab, current_fullpath_dir, new_dir, app);

	if( error != 0L)
	{
		gfree(( void*)NewTab);
		show_alert( NULL, error, app);
		return NULL;
	}

	if(( win->tab) && !( win->status & WS_TABBAR))
		win->status |= WS_TABBAR;
	
	/* set the status bar informations */
	NewTab->get_disk_size( NewTab, app);

	if( NewTab->nbr_icons == 1)
		sprintf( NewTab->info, app->info_simple, NewTab->total_files_size);
	else
		sprintf( NewTab->info, app->info_multi, NewTab->total_files_size, NewTab->nbr_icons);	
	
	get_directory_name( NewTab->directory, NewTab->name, MAXNAMLEN);

	win->nbr_tabs++;
	
	NewTab->next = NULL;

	if( win->tab == NULL)
		win->tab = NewTab;
	else 
	{
		for( last_tab = win->tab; last_tab->next; last_tab = last_tab->next);
		
		last_tab->next = NewTab;
	}

	zstrncpy( NewTab->name_shown, NewTab->name, TAB_NAME_SHOW_CHARS);
	NewTab->tab_txt_w = get_text_width(( const char *)NewTab->name_shown);
	NewTab->real_txt_w = get_text_width(( const char *)NewTab->name);

	if( NewTab->tab_txt_w > ( MAXIMAL_TAB_WIDTH - MINI_ICON_SIZE - 8))
		NewTab->tab_txt_w = name_shorter( MAXIMAL_TAB_WIDTH - MINI_ICON_SIZE - 8, NewTab->name_shown);	
	
	win->selected_tab = NewTab;

	update_tabs( win, tabbar_width, app);
	
	WinFile_Select_Tab( win, NewTab, app);
	
	return NewTab;
}



/*==================================================================================*
 * void WinFile_Select_Tab:															*
 *		Select a new tab, update the datas and redraw the window.					*
 *----------------------------------------------------------------------------------*
 * 	win						-> Pointer to the parent window.						*
 * 	tab						-> Pointer to the target tab.							* 
 * 	app						-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Select_Tab( WINDOW *win, TAB *tab, APPVar *app)
{
	mt_wind_set_str( win->handle, WF_NAME, tab->name, app->aes_global);	
	
	win->selected_tab = tab;
	
	set_menu_status( win, app);
	
	if( win->status & WS_OPEN)
	{	
		GRECT work_area;

		win->edit.edition_mode = FALSE;
		
		mt_wind_get_grect( win->handle, WF_WORKXYWH, &work_area, app->aes_global);

		if( win->status & WS_INFOBAR)
		{	
			redraw_infobar( win, &work_area, app);
			work_area.g_h -= INFOBAR_HEIGHT;	
		}	
		
		if( win->status & WS_TOOLBAR)
		{
			work_area.g_y += TOOLBAR_HEIGHT;
			work_area.g_h -= TOOLBAR_HEIGHT;
		}	
		
		work_area.g_x += win->sidebar_width;
		work_area.g_w -= win->sidebar_width;			
		
		*(GRECT*)&app->aes_buff[4] = work_area;
		win->WinRedraw( win, app);

		if( tab->view_mode == LIST_MODE)
		{
			work_area.g_y += COLUMN_TITLE_HEIGHT;
			work_area.g_h -= COLUMN_TITLE_HEIGHT;
		}	
		
		if( win->status & WS_PATHBAR)
		{
			work_area.g_y += PATHBAR_HEIGHT;
			work_area.g_h -= PATHBAR_HEIGHT;
		}			
		
		if( win->status & WS_TABBAR)
			work_area.g_h -= TABBAR_HEIGHT;		
		
		WinFile_Slider( win, ( int32)work_area.g_h, app);
	}
}



/*==================================================================================*
 * WinFile_Close_Tab:																*
 *		Close the tab and free up memory.											*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the parent window.									*
 * 		tab     			-> the tab to close.									* 
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Close_Tab( WINDOW *win, TAB *tab, APPVar *app)
{
	int i, select_another_tab;
	
	/* the selected tab is not the tab to close, we don't need to select another tab */
	if( win->selected_tab != tab)
		select_another_tab = FALSE;
	else
		select_another_tab = TRUE;

		
	for( i = 0 ; i < tab->nbr_icons ; i++)
		delete_mfdb( &tab->entry[i].thumbnail);
	
	if( tab->entry)
		gfree(( void*)tab->entry);
	
	if( win->tab == tab)
	{	
		win->tab = tab->next;
		
		if( select_another_tab)
			win->selected_tab = win->tab;
	}	
	else
	{
		TAB *previous_tab;
		
		for( previous_tab = win->tab; previous_tab->next != tab; previous_tab = previous_tab->next);
		
		previous_tab->next = tab->next;
		
		if( select_another_tab)
			win->selected_tab = previous_tab;
	}
	
	gfree(( void*)tab);
	
	win->nbr_tabs--;
	
	if( win->tab)
	{	
		if(( win->tab->next == NULL) && ( win->status & WS_TABBAR) && ( app->pref.tab_options & TAB_AUTOHIDE))
			win->status &= ~WS_TABBAR;
			
		if( win->status & WS_OPEN)
			update_tabs( win, -1, app);
		
		if( select_another_tab)
			WinFile_Select_Tab( win, win->selected_tab, app);
		else if( win->status & WS_OPEN)
			redraw_tabbar( win, NULL, app);
	}	
	else
		win->selected_tab = NULL;
}



/*==================================================================================*
 * WinFile_Tab_Drag_and_Drop:														*
 *		Drag & Drop a tab. If the tab is inside another windows, we add the tab		*
 *		else we open a new window.													*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the parent window.									*
 * 		tab     			-> the tab to drag.										* 
 * 		mousex, mousey 		-> the mouse position.									* 
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Tab_Drag_and_Drop( WINDOW *win, TAB *tab, int16 mousex, int16 mousey, GRECT *file_area, APPVar *app)
{
	WINDOW *target_win;
	int16 end_pos_x, end_pos_y;

	mt_graf_mouse( FLAT_HAND, NULL, app->aes_global);
	
	if( IS_IN( mousex, mousey, tab->tab_xpos + file_area->g_x, file_area->g_y + file_area->g_h, tab->tab_width, TABBAR_HEIGHT))
		mt_graf_dragbox( tab->tab_width, TABBAR_HEIGHT, tab->tab_xpos + file_area->g_x, file_area->g_y + file_area->g_h, app->x, app->y, app->w, app->h, &end_pos_x, &end_pos_y, app->aes_global);
	else
		mt_graf_dragbox( tab->tab_width, TABBAR_HEIGHT, mousex - ( tab->tab_width >> 1), mousey - ( TABBAR_HEIGHT >> 1), app->x, app->y, app->w, app->h, &end_pos_x, &end_pos_y, app->aes_global);
		
	target_win = WindHandle( app, mt_wind_find( end_pos_x, end_pos_y, app->aes_global));
	
	if( target_win == win) 
		goto end;
	
	if( !target_win || ( target_win->handle == 0) || target_win->status & WS_FORM)
	{
		GRECT win_position;
		
		mt_wind_get_grect( win->handle, WF_CURRXYWH, &win_position, app->aes_global);
		
		win_position.g_x = MAX( app->x, end_pos_x - (( win_position.g_w - tab->tab_width) >> 1));
		
		if(( win_position.g_x + win_position.g_w) > app->w)
			win_position.g_x = app->w - win_position.g_w;

		win_position.g_y = MAX( app->y, end_pos_y - (( win_position.g_h - TABBAR_HEIGHT) >> 1));

		if(( win_position.g_y + win_position.g_h) > ( app->y + app->h))
			win_position.g_y = ( app->y + app->h) - win_position.g_h;		
		
		WinFile_New( NULL, tab->directory, win_position.g_x, win_position.g_y, win_position.g_w, win_position.g_h, app);
	}	
	else 
	{
		while( !mt_wind_set( target_win->handle, WF_TOP, 0, 0, 0, 0, app->aes_global));
		WinFile_NewTab( target_win, NULL, tab->directory, -1, app);
	}
	
	WinFile_Close_Tab( win, tab, app);
	
end:
	mt_graf_mouse( ARROW, NULL, app->aes_global);
}




