#include "general.h"
#include "winfile/WinFile.h"


void optimal_position( WINDOW *win, GRECT *win_position, APPVar *app)
{
	WINDOW *scan;
	int16 w, h, default_x, default_y, entries_in_width, entries_in_height;
	GRECT scan_area;
	
	
	if( win->selected_tab->view_mode == LIST_MODE)
	{	
		entries_in_height = 1;
		
		w = win->selected_tab->name_column_width + win->selected_tab->size_column_width + win->selected_tab->date_column_width;
		h = win->selected_tab->h_u + win->selected_tab->h_u + COLUMN_TITLE_HEIGHT;
		
		while( entries_in_height < win->selected_tab->nbr_icons)
		{
			entries_in_height++;
			h += win->selected_tab->h_u;
		}
	}
	else
	{	
		entries_in_height = 2;
		entries_in_width = 2;
		
		w = win->selected_tab->w_u << 1;
		h = win->selected_tab->h_u << 1;
		
		while( entries_in_width * entries_in_height < win->selected_tab->nbr_icons)
		{
			if((( entries_in_width * win->selected_tab->w_u) < ( app->w >> 1)) && entries_in_width <= entries_in_height)
			{	
				entries_in_width++;
				w += win->selected_tab->w_u;
			}
			else 
			{
				entries_in_height++;
				h += win->selected_tab->h_u;
			}
		}
	}
	
	w += win->sidebar_width;
	
	if( win->status & WS_INFOBAR)
		h += INFOBAR_HEIGHT;	
	
	if( win->status & WS_TOOLBAR)
		h += TOOLBAR_HEIGHT;
	
	if( win->status & WS_PATHBAR)
		h += PATHBAR_HEIGHT;
	
	if( win->status & WS_TABBAR)
		h += TABBAR_HEIGHT;		
	
	mt_wind_calc( WC_BORDER, WINATTRIB, app->x + app->win->selected_tab->w_u, app->y + app->win->selected_tab->h_u + 22, w, h, &win_position->g_x, &win_position->g_y, &win_position->g_w, &win_position->g_h, app->aes_global);
	

	default_x = win_position->g_x;
	default_y = win_position->g_y;
	

	for( scan = app->win; scan; scan = scan->next)
	{
		if(( scan->handle == 0) || !( scan->status & WS_OPEN))
			continue;
		
		mt_wind_get_grect( scan->handle, WF_CURRXYWH, &scan_area, app->aes_global);

		if(( scan_area.g_x == win_position->g_x) && ( scan_area.g_y == win_position->g_y))
		{
			if( win_position->g_x + win->w_min + 22 < app->w)
				win_position->g_x += 22;
			else
				win_position->g_x = default_x;
			
			if( win_position->g_y + win->h_min + 22 < app->h)
				win_position->g_y += 22;
			else
				win_position->g_y = default_y;			
		}	
	}	
	

	if( win_position->g_w > app->w - win_position->g_x)
		win_position->g_w = app->w - win_position->g_x;
	
	if( win_position->g_h > app->h - win_position->g_y)
		win_position->g_h = app->h - win_position->g_y;
	
}



/*==================================================================================*
 * void WinFile_New:																*
 *		Create a new window.														*
 *----------------------------------------------------------------------------------*
 * 	current_fullpath_dir	-> Full path to the parent directory.					* 
 * 	new_dir					-> The directory to scan.								*
 * 	app						-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_New( char *current_fullpath_dir, char *new_dir, int16 x, int16 y, int16 w, int16 h, APPVar *app)
{
	WINDOW		*win, *last_win;
	int16		dum, handle, widgets_height = 0;
	GRECT		win_position;
	

	handle = mt_wind_create( WINATTRIB, app->x, app->y, app->w, app->h, app->aes_global);

	if( handle < 0)
	{
		show_alert( NULL, EWINDOW, app);
		return;
	}

	win = ( WINDOW*)gmalloc( sizeof( WINDOW));

	if( win == NULL)
	{
		mt_wind_delete( handle, app->aes_global);
		show_alert( NULL, -ENOMEM, app);
		return;
	}

	win->handle				= handle;
	win->status				= 0;
	win->sidebar_width		= 0;
	win->tab				= NULL;
	win->selected_tab		= NULL;
	win->nbr_tabs			= 0;
	win->edit.edition_mode	= FALSE;

	win->WinRedraw			= WinFile_Redraw;				
	win->WinIconifyRedraw	= WinFile_Iconify_Redraw;			
	win->WinClose			= WinFile_Close;
	win->WinTop				= WinFile_Top;
	win->WinBottom			= WinFile_Bottom;
	win->WinMove			= WinFile_Move;
	win->WinSize			= WinFile_Sizer;
	win->WinFull			= WinFile_Full;
	win->WinIconify			= WinFile_Iconify;
	win->WinUniconify		= WinFile_Uniconify;
	win->WinAlliconify		= WinFile_Alliconify;
	win->WinArrow			= WinFile_Arrow;
	win->WinVslide			= WinFile_Vslide;
	win->WinKeyboard		= WinFile_Keyb;
	win->WinMouse			= WinFile_Mouse;	

	if( app->pref.show_toolbar)
	{	
		win->status |= WS_TOOLBAR;
		widgets_height += TOOLBAR_HEIGHT;
	}	
	
	if( app->pref.show_pathbar)
	{	
		win->status |= WS_PATHBAR;
		widgets_height += PATHBAR_HEIGHT;
		
	}	
	
	if( app->pref.show_sidebar)
	{	
		win->status |= WS_SIDEBAR;
		win->sidebar_width = DEFAULT_SIDEBAR_WIDTH;
	}
		
	if( app->pref.show_tabbar)
	{	
		win->status |= WS_TABBAR;
		widgets_height += TABBAR_HEIGHT;
	}
		
	if( app->pref.show_infobar)
	{	
		win->status |= WS_INFOBAR;
		widgets_height += INFOBAR_HEIGHT;
	}
		
	if( !WinFile_NewTab( win, current_fullpath_dir, new_dir, 150, app))
	{
		gfree(( void*)win);
		mt_wind_delete( handle, app->aes_global);
		return;
	}	

	/* Put the windows in the windows list */
	win->next = NULL;
	
	for( last_win = app->win; last_win->next; last_win = last_win->next);
	
	last_win->next = win;	
	
	mt_wind_calc( WC_BORDER, WINATTRIB, 0, 0, STANDARD_ICON_SIZE + STANDARD_ICON_SIZE + 4 + app->x_space + app->x_space + MINIMAL_SIDEBAR_WIDTH, STANDARD_ICON_SIZE + app->hcell + app->y_space + app->y_space + app->y_space + TOOLBAR_HEIGHT + PATHBAR_HEIGHT + TABBAR_HEIGHT + INFOBAR_HEIGHT, &dum, &dum, &win->w_min, &win->h_min, app->aes_global);

	if( x == -1)
		optimal_position( win, &win_position, app);
	else 
	{
		win_position.g_x = x;
		win_position.g_y = y;
		win_position.g_w = w;
		win_position.g_h = h;
	}

	if( mt_wind_open( handle, win_position.g_x, win_position.g_y, win_position.g_w, win_position.g_h, app->aes_global) == 0)
	{
		WinFile_Close( win, app);
		show_alert( NULL, EWINDOW, app);
		return;
	}

	win->status |= WS_OPEN;

	while( !mt_wind_get_grect( handle, WF_WORKXYWH, &win_position, app->aes_global));
	WinFile_Slider( win, ( int32)( win_position.g_h - widgets_height), app);
}




