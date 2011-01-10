#include "general.h"
#include "winfile/WinFile.h"


/*==================================================================================*
 * void WinFile_Calc_Areas:															*
 *		Calculate the full and the file area of a window.							*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win			-> The window to handle.										*
 * 		full_area	-> The full area.												*
 * 		file_area	-> The file area.												* 
 * 		app			-> Application's descriptor.									*
 *----------------------------------------------------------------------------------*
 * returns: 																		*
 *      --																			*
 *==================================================================================*/

void WinFile_Calc_Areas( WINDOW *win, GRECT *full_area, GRECT *file_area, APPVar *app)
{
	if( full_area)
	{	
		mt_wind_get_grect( win->handle, WF_WORKXYWH, full_area, app->aes_global);

		file_area->g_x = full_area->g_x + win->sidebar_width;
		file_area->g_y = full_area->g_y;
		file_area->g_w = full_area->g_w - win->sidebar_width;	
		file_area->g_h = full_area->g_h;
	}
	else 
	{
		mt_wind_get_grect( win->handle, WF_WORKXYWH, file_area, app->aes_global);
		
		file_area->g_x += win->sidebar_width;
		file_area->g_w -= win->sidebar_width;	
	}

	if( win->status & WS_INFOBAR)
		file_area->g_h -= INFOBAR_HEIGHT;	
		
	if( win->status & WS_TOOLBAR)
	{
		file_area->g_y += TOOLBAR_HEIGHT;
		file_area->g_h -= TOOLBAR_HEIGHT;
	}	
		
	if( win->selected_tab->view_mode == LIST_MODE)
	{
		file_area->g_y += COLUMN_TITLE_HEIGHT;
		file_area->g_h -= COLUMN_TITLE_HEIGHT;
	}	
		
	if( win->status & WS_PATHBAR)
	{
		file_area->g_y += PATHBAR_HEIGHT;
		file_area->g_h -= PATHBAR_HEIGHT;
	}			
		
	if( win->status & WS_TABBAR)
		file_area->g_h -= TABBAR_HEIGHT;		
}

