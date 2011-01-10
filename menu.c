#include "general.h"
#include "ztext.h"
#include "winfile/WinFile.h"



/*==================================================================================*
 * void set_menu_status:															*
 *		Set correctly the menu.														*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win			-> The window topped.											*
 * 		app			-> Application's descriptor.									*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void set_menu_status( WINDOW *win, APPVar *app)
{
	if( win->handle == 0)
	{	
		/* File Menu */
		mt_menu_ienable( app->menu_rsc, MENU_BAR_NEW_TAB, DISABLE, app->aes_global);		
		mt_menu_ienable( app->menu_rsc, MENU_BAR_CLOSE_WIN, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_CLOSE_TAB, DISABLE, app->aes_global);
		
		/* View Menu */
		mt_menu_ienable( app->menu_rsc, MENU_BAR_ICON, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_LIST, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_UNSORTED, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_BY_NAME, DISABLE, app->aes_global);	
		mt_menu_ienable( app->menu_rsc, MENU_BAR_BY_SIZE, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_BY_DATE, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_TOOLBAR, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_CUSTOMIZE_TOOLBAR, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_PATHBAR, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_SIDEBAR, DISABLE, app->aes_global);	
		mt_menu_ienable( app->menu_rsc, MENU_BAR_TABBAR, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_INFOBAR, DISABLE, app->aes_global);
		
		mt_menu_icheck( app->menu_rsc, MENU_BAR_ICON, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_LIST, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_UNSORTED, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_BY_NAME, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_BY_SIZE, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_BY_DATE, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_TOOLBAR, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_PATHBAR, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_SIDEBAR, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_TABBAR, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_INFOBAR, UNCHECK, app->aes_global);		
	
	}
	else if( win->status & WS_FORM)
	{	
		/* File Menu */
		mt_menu_ienable( app->menu_rsc, MENU_BAR_NEW_TAB, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_CLOSE_WIN, ENABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_CLOSE_TAB, DISABLE, app->aes_global);
		
		/* View Menu */
		mt_menu_ienable( app->menu_rsc, MENU_BAR_ICON, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_LIST, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_UNSORTED, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_BY_NAME, DISABLE, app->aes_global);	
		mt_menu_ienable( app->menu_rsc, MENU_BAR_BY_SIZE, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_BY_DATE, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_TOOLBAR, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_CUSTOMIZE_TOOLBAR, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_PATHBAR, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_SIDEBAR, DISABLE, app->aes_global);	
		mt_menu_ienable( app->menu_rsc, MENU_BAR_TABBAR, DISABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_INFOBAR, DISABLE, app->aes_global);
		
		mt_menu_icheck( app->menu_rsc, MENU_BAR_ICON, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_LIST, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_UNSORTED, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_BY_NAME, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_BY_SIZE, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_BY_DATE, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_TOOLBAR, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_PATHBAR, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_SIDEBAR, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_TABBAR, UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_INFOBAR, UNCHECK, app->aes_global);	
	}		
	else
	{	
		/* File Menu */
		mt_menu_ienable( app->menu_rsc, MENU_BAR_NEW_TAB, ENABLE, app->aes_global);		
		mt_menu_ienable( app->menu_rsc, MENU_BAR_CLOSE_WIN, ENABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_CLOSE_TAB, ENABLE, app->aes_global);		
		
		/* View Menu */
		mt_menu_ienable( app->menu_rsc, MENU_BAR_ICON, ENABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_LIST, ENABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_UNSORTED, ENABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_BY_NAME, ENABLE, app->aes_global);	
		mt_menu_ienable( app->menu_rsc, MENU_BAR_BY_SIZE, ENABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_BY_DATE, ENABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_TOOLBAR, ENABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_CUSTOMIZE_TOOLBAR, ENABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_PATHBAR, ENABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_SIDEBAR, ENABLE, app->aes_global);	
		mt_menu_ienable( app->menu_rsc, MENU_BAR_TABBAR, ENABLE, app->aes_global);
		mt_menu_ienable( app->menu_rsc, MENU_BAR_INFOBAR, ENABLE, app->aes_global);
		
		mt_menu_icheck( app->menu_rsc, MENU_BAR_ICON, ( win->selected_tab->view_mode == ICON_MODE) ? CHECK : UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_LIST, ( win->selected_tab->view_mode == MENU_BAR_LIST) ? CHECK : UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_UNSORTED, ( win->selected_tab->sortingmode == MENU_BAR_UNSORTED) ? CHECK : UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_BY_NAME, ( win->selected_tab->sortingmode == MENU_BAR_BY_NAME) ? CHECK : UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_BY_SIZE, ( win->selected_tab->sortingmode == MENU_BAR_BY_SIZE) ? CHECK : UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_BY_DATE, ( win->selected_tab->sortingmode == MENU_BAR_BY_DATE) ? CHECK : UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_TOOLBAR, ( win->status & WS_TOOLBAR) ? CHECK : UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_PATHBAR, ( win->status & WS_PATHBAR) ? CHECK : UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_SIDEBAR, ( win->status & WS_SIDEBAR) ? CHECK : UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_TABBAR, ( win->status & WS_TABBAR) ? CHECK : UNCHECK, app->aes_global);
		mt_menu_icheck( app->menu_rsc, MENU_BAR_INFOBAR, ( win->status & WS_INFOBAR) ? CHECK : UNCHECK, app->aes_global);
	}			
}



/*==================================================================================*
 * void open_new_tab:																*
 *		Open a new tab, if win is NULL, we open a new browser.						*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win			-> The window to handle or NULL.								*
 * 		app			-> Application's descriptor.									*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void open_new_tab( WINDOW *win, APPVar *app)
{
	if( win && ( win->handle != 0) && !( win->status & WS_FORM))
	{		
		switch( app->pref.default_tab_path)
		{
			/* TODO */
			case T_HOME:
			case T_ROOT:

				
			case T_CURRENTDIR:			
				WinFile_NewTab( win, NULL, win->selected_tab->directory, -1, app);
				break;			

			case T_CDRIVE:
			default:
				WinFile_NewTab( win, NULL, "C:\\", -1, app);
				break;
		}		
	}
	else
	{
		switch( app->pref.default_tab_path)
		{
			/* TODO */
			case T_HOME:
			case T_ROOT:
				
				
			case T_CDRIVE:
			case T_CURRENTDIR:	
			default:
				WinFile_New( NULL, "C:\\", -1, -1, -1, -1, app);
				break;				
		}		
	}
}



/*==================================================================================*
 * void close_tab:																	*
 *		We close the tab hosted by the window "win".								*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win			-> The target window.											*
 * 		tab			-> The tab to close, if NULL, we close the selected tab.		* 
 * 		app			-> Application's descriptor.									*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void close_tab( WINDOW *win, TAB *tab, APPVar *app)
{
	if( !win || ( win->handle == 0))
		return;
	
	/* The tab is alone in the window ? we the window */
	if( win->tab->next == NULL)
		win->WinClose( win, app);
	else if( tab == NULL)
		WinFile_Close_Tab( win, win->selected_tab, app);
	else
		WinFile_Close_Tab( win, tab, app);
}



/*==================================================================================*
 * void show_as_list:																*
 *		Show the files as list.														*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win			-> The window to handle.										*
 * 		app			-> Application's descriptor.									*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void show_as_list( WINDOW *win, APPVar *app)
{
	GRECT file_area;
	int i;
	TAB *tab;

	if( !win || ( win->handle == 0) || ( win->selected_tab->view_mode == LIST_MODE))
		return;	
	
	tab = win->selected_tab;
	
	WinFile_Calc_Areas( win, NULL, &file_area, app);

	tab->view_mode	= LIST_MODE;
	tab->icon_size	= MINI_ICON_SIZE;
	tab->h_u		= MINI_ICON_SIZE + 2; // 1 + icon + 1
	tab->w_u		= app->w; // In LIST_MODE, we set the win->w_u parameter to the maximum.
	tab->ypos		= 0;	

	for( i = 0; i < tab->nbr_icons; i++)
	{
		if( tab->entry[i].icon_txt_w != tab->entry[i].real_txt_w)
		{
			zstrncpy( tab->entry[i].name_shown, ( const char *)tab->entry[i].name, ENTRY_NAME_SHOW_CHARS);
			tab->entry[i].icon_txt_w = tab->entry[i].real_txt_w;
		}
		
		if( tab->entry[i].icon_txt_w > tab->name_column_width - ( MINI_ICON_SIZE + 1 + app->x_space + app->x_space))
			tab->entry[i].icon_txt_w = name_shorter( tab->name_column_width - ( MINI_ICON_SIZE + 1 + app->x_space + app->x_space), tab->entry[i].name_shown);
	}
	
	*(GRECT*)&app->aes_buff[4] = file_area;
	win->WinRedraw( win, app);
	WinFile_Slider( win, ( int32)file_area.g_h - COLUMN_TITLE_HEIGHT, app);
	
	mt_menu_icheck( app->menu_rsc, MENU_BAR_LIST, CHECK, app->aes_global);
	mt_menu_icheck( app->menu_rsc, MENU_BAR_ICON, UNCHECK, app->aes_global);					
}



/*==================================================================================*
 * void show_as_icon:																*
 *		Show the files as icon.														*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win			-> The window to handle.										*
 * 		app			-> Application's descriptor.									*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void show_as_icon( WINDOW *win, APPVar *app)
{
	GRECT file_area;
	int i;
	TAB *tab;
	
	if( !win || ( win->handle == 0) || ( win->selected_tab->view_mode == ICON_MODE))
		return;
	
	tab = win->selected_tab;
	
	tab->view_mode	= ICON_MODE;
	tab->icon_size	= STANDARD_ICON_SIZE;
	tab->h_u		= tab->icon_size + app->hcell + app->y_space + app->y_space + app->y_space;
	tab->w_u		= tab->icon_size + tab->icon_size + 4 + app->x_space + app->x_space;					
	tab->ypos		= 0;
	
	WinFile_Calc_Areas( win, NULL, &file_area, app);	
	
	for( i = 0; i < tab->nbr_icons; i++)
	{
		if( tab->entry[i].icon_txt_w != tab->entry[i].real_txt_w)
		{
			zstrncpy( tab->entry[i].name_shown, ( const char *)tab->entry[i].name, ENTRY_NAME_SHOW_CHARS);
			tab->entry[i].icon_txt_w = tab->entry[i].real_txt_w;
		}
		
		if( tab->entry[i].icon_txt_w > tab->w_u - ( app->x_space << 1))
			tab->entry[i].icon_txt_w = name_shorter( tab->w_u - ( app->x_space << 1), tab->entry[i].name_shown);
	}
	
	*(GRECT*)&app->aes_buff[4] = file_area;
	win->WinRedraw( win, app);
	WinFile_Slider( win, ( int32)file_area.g_h, app);
	
	mt_menu_icheck( app->menu_rsc, MENU_BAR_ICON, CHECK, app->aes_global);
	mt_menu_icheck( app->menu_rsc, MENU_BAR_LIST, UNCHECK, app->aes_global);				
}



/*==================================================================================*
 * void show_infobar:																*
 *		Show the infobar( 'status bar' entry in the menu).							*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win			-> The window to handle.										*
 * 		app			-> Application's descriptor.									*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void show_infobar( WINDOW *win, APPVar *app)
{
	GRECT full_area, file_area;
	
	if( !win || ( win->handle == 0))
		return;
	
	if( win->status & WS_INFOBAR)
	{
		win->status &= ~WS_INFOBAR;
		mt_menu_icheck( app->menu_rsc, MENU_BAR_INFOBAR, UNCHECK, app->aes_global);
		
		WinFile_Calc_Areas( win, &full_area, &file_area, app);
		
		full_area.g_y += ( full_area.g_h - INFOBAR_HEIGHT);
		full_area.g_h = INFOBAR_HEIGHT;
		
		if( win->status & WS_TABBAR)
		{
			full_area.g_y -= TABBAR_HEIGHT;
			full_area.g_h += TABBAR_HEIGHT;
		}	
		
		*(GRECT*)&app->aes_buff[4] = full_area;
		win->WinRedraw( win, app);
	}
	else
	{
		win->status |= WS_INFOBAR;
		mt_menu_icheck( app->menu_rsc, MENU_BAR_INFOBAR, CHECK, app->aes_global);
		
		WinFile_Calc_Areas( win, &full_area, &file_area, app);

		if( win->status & WS_TABBAR)
			redraw_tabbar( win, &file_area, app);

		update_infobar( win, &full_area, app);
	}

	WinFile_Slider( win, ( int32)file_area.g_h, app);
}



/*==================================================================================*
 * void show_tabbar:																*
 *		Show the tabbar.															*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win			-> The window to handle.										*
 * 		app			-> Application's descriptor.									*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void show_tabbar( WINDOW *win, APPVar *app)
{
	GRECT file_area, full_area;
	int16 height;
	
	if( !win || ( win->handle == 0))
		return;
	
	if( win->status & WS_TABBAR)
	{
		win->status &= ~WS_TABBAR;
		mt_menu_icheck( app->menu_rsc, MENU_BAR_TABBAR, UNCHECK, app->aes_global);
		WinFile_Calc_Areas( win, &full_area, &file_area, app);
		height = file_area.g_h;	
		
	}
	else
	{
		WinFile_Calc_Areas( win, &full_area, &file_area, app);
		height = file_area.g_h - TABBAR_HEIGHT;
		win->status |= WS_TABBAR;
		mt_menu_icheck( app->menu_rsc, MENU_BAR_TABBAR, CHECK, app->aes_global);
	}
	
	file_area.g_y += file_area.g_h - TABBAR_HEIGHT;
	file_area.g_h = TABBAR_HEIGHT;
	
	*(GRECT*)&app->aes_buff[4] = file_area;
	win->WinRedraw( win, app);	
	
	if( win->status & WS_INFOBAR)
		redraw_infobar( win, &full_area, app);
	
	WinFile_Slider( win, ( int32)height, app);
}


