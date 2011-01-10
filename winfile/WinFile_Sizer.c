#include "general.h"
#include "edit.h"
#include "winfile/WinFile.h"

/*==================================================================================*
 * void WinFile_Sizer:																*
 *		this function handle the WM_SIZED event 									*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Sizer( WINDOW *win, APPVar *app)
{
	TAB *tab = win->selected_tab;	
	GRECT file_area, win_area, old_win_area;
	int16 rdw_win = 0;
	int32 old_win_ypos;

	mt_wind_get_grect( win->handle, WF_WORKXYWH, &old_win_area, app->aes_global);
	
	win_area.g_x = MAX( app->aes_buff[6], win->w_min);
	win_area.g_y = MAX( app->aes_buff[7], win->h_min);
	win_area.g_w = MIN( win_area.g_x,  app->w);
	win_area.g_h = MIN( win_area.g_y, app->h);

	mt_wind_set( win->handle, WF_CURRXYWH, app->aes_buff[4], app->aes_buff[5], win_area.g_w, win_area.g_h, app->aes_global);

	WinFile_Calc_Areas( win, &win_area, &file_area, app);
	
	if( win->edit.edition_mode == TRUE)
		exit_edit_mode( win, tab->first_selected, file_area.g_x, file_area.g_y, app);

	old_win_ypos = tab->ypos;

	if(( int32)file_area.g_h > tab->ypos_max * tab->h_u)
		tab->ypos = 0;
	else
		tab->ypos = MIN( tab->ypos, tab->ypos_max - ( int32)file_area.g_h / tab->h_u);

	if( old_win_ypos != tab->ypos)
		rdw_win = 1;
	else if(( int32)file_area.g_w >= (( tab->xpos_max + 1) * tab->w_u) || ((( int32)file_area.g_w < tab->xpos_max * tab->w_u) && tab->xpos_max > 1))
	{
		if ( file_area.g_w < (( tab->nbr_icons + 1) * tab->w_u))
			rdw_win = 1;
	}

	win->status &= ~WS_FULLSIZE;

	WinFile_Slider( win, ( int32)file_area.g_h, app);

	if( rdw_win)
	{		
		if( win->status & WS_TABBAR)
		{	
			file_area.g_h += TABBAR_HEIGHT;
			update_tabs( win, file_area.g_w, app);
		}	
		
		*(GRECT*)&app->aes_buff[4] = file_area;
		win->WinRedraw( win, app);
		
		if( win->status & WS_INFOBAR)
			redraw_infobar( win, &win_area, app);
	}
	else if(( win->status & WS_INFOBAR) || ( win->status & WS_TABBAR))
	{
		if( win_area.g_y + win_area.g_h <= old_win_area.g_y + old_win_area.g_h)
		{	
			if( win->status & WS_INFOBAR)
				redraw_infobar( win, &win_area, app);
			
			if( win->status & WS_TABBAR)
			{	
				update_tabs( win, file_area.g_w, app);
				redraw_tabbar( win, &file_area, app);
			}	
		}			
		else 
		{
			win_area.g_y = old_win_area.g_y + old_win_area.g_h - INFOBAR_HEIGHT;
			win_area.g_h = app->h; /* the redraw function will set the correct height */
			
			if( win->status & WS_INFOBAR)
				win_area.g_y -= INFOBAR_HEIGHT;
			
			if( win->status & WS_TABBAR)
			{	
				win_area.g_y -= INFOBAR_HEIGHT;
				update_tabs( win, file_area.g_w, app);
			}	
			
			*(GRECT*)&app->aes_buff[4] = win_area;
			win->WinRedraw( win, app);		
		}
	}	
}


