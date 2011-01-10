#include "general.h"
#include "edit.h"
#include "menu.h"
#include "winfile/WinFile.h"
#include "evnt_loop.h"



/*==================================================================================*
 * WinFile_Bottom:																	*
 *		WM_BOTTOM event for the WINDOW *win.										*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Bottom( WINDOW *win, APPVar *app)
{
	int16 top, dum;
	WINDOW *top_win;
	
	if( win->edit.edition_mode == TRUE)
	{
		GRECT rect;
		
		WinFile_Calc_Areas( win, NULL, &rect, app);
		exit_edit_mode( win, win->selected_tab->first_selected, rect.g_x, rect.g_y, app);	
	}

	mt_wind_set( win->handle, WF_BOTTOM, 0, 0, 0, 0, app->aes_global);

	mt_wind_get( 0, WF_TOP, &top, &dum, &dum, &dum, app->aes_global);
	top_win = WindHandle( app, top);
		
	if( top_win)
	{
		if((( win->status & WS_FORM) && !( top_win->status & WS_FORM)) || (!( win->status & WS_FORM) && ( top_win->status & WS_FORM)))
			set_menu_status( top_win, app);
	}
	else
		set_menu_status( app->win, app);
}



/*==================================================================================*
 * WinFile_Top:																		*
 *		WM_TOPPED event for the WINDOW *win.										*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Top( WINDOW *win, APPVar *app)
{
	mt_wind_set( win->handle, WF_TOP, 0, 0, 0, 0, app->aes_global);
	set_menu_status( win, app);		
}



/*==================================================================================*
 * WinFile_Move:																	*
 *		WM_MOVED event for the WINDOW *win.											*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Move( WINDOW *win, APPVar *app)
{
	mt_wind_set( win->handle, WF_CURRXYWH, app->aes_buff[4], app->aes_buff[5], app->aes_buff[6], app->aes_buff[7], app->aes_global);
	win->status &= ~WS_FULLSIZE;
}



/*==================================================================================*
 * WinFile_Full:																	*
 *		WM_FULLED event for the WINDOW *win.										*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Full( WINDOW *win, APPVar *app) 
{
	TAB *tab = win->selected_tab;
	GRECT file_area;
	
	win->edit.edition_mode = FALSE;
	
	if( win->status & WS_FULLSIZE) 
	{
		mt_wind_get_grect( win->handle, WF_PREVXYWH, &file_area, app->aes_global);
		mt_wind_set_grect( win->handle, WF_CURRXYWH, &file_area, app->aes_global);
	} else
		mt_wind_set( win->handle, WF_CURRXYWH, app->x, app->y, app->w, app->h, app->aes_global);
	
	win->status ^= WS_FULLSIZE;

	WinFile_Calc_Areas( win, ( GRECT*)&app->aes_buff[4], &file_area, app);

	update_tabs( win, app->aes_buff[6], app);
	
	win->WinRedraw( win, app);

	if(( int32)file_area.g_h > tab->ypos_max * tab->h_u)
		tab->ypos = 0;
	else
		tab->ypos = MIN( tab->ypos, tab->ypos_max - ( int32)file_area.g_h / tab->h_u);

	WinFile_Slider( win, ( int32)file_area.g_h, app);
}



