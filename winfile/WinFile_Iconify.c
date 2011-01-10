#include "general.h"
#include "ztext.h"
#include "misc.h"
#include "winfile/WinFile.h"
#include "icon/icon.h"


/*==================================================================================*
 * WinFile_Iconify:																	*
 *		WM_ICONIFY event for the WINDOW *win.										*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Iconify( WINDOW *win, APPVar *app)
{
	if( win->status & WS_ICONIFY)
		return;

	win->status |= WS_ICONIFY;

	win->edit.edition_mode = FALSE;

	zstrncpy( win->text_iconified, win->selected_tab->name, 16);

	if(( win->text_iconified_txt_w = get_text_width(( const char *)win->text_iconified)) > 60)
		win->text_iconified_txt_w = name_shorter( 60, win->text_iconified);

	zstrncpy( win->title_iconified, ( const char *)win->selected_tab->directory, 3);

	mt_wind_set( win->handle, WF_ICONIFY, app->aes_buff[4], app->aes_buff[5], app->aes_buff[6], app->aes_buff[7], app->aes_global);
	mt_wind_set_str( win->handle, WF_NAME, win->title_iconified, app->aes_global);
}



/*==================================================================================*
 * WinFile_Uniconify:																*
 *		WM_UNICONIFY event for the WINDOW *win.										*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Uniconify( WINDOW *win, APPVar *app)
{
	if( win->status & WS_ALLICNF) 
	{
		WINDOW *scan;

		for( scan= app->win; scan; scan = scan->next) 
		{
			if( scan != win && scan->status & WS_ALLICNF) 
			{
				mt_wind_open( scan->handle, scan->uniconified_pos.g_x, scan->uniconified_pos.g_y, scan->uniconified_pos.g_w, scan->uniconified_pos.g_h, app->aes_global);
				scan->status |= WS_OPEN;
				scan->status &= ~WS_ALLICNF;
			}
		}
		win->status &= ~WS_ALLICNF;
	}

	win->status &= ~WS_ICONIFY;

	mt_wind_set_str( win->handle, WF_NAME, win->selected_tab->name, app->aes_global);
	mt_wind_set( win->handle, WF_UNICONIFY, app->aes_buff[4], app->aes_buff[5], app->aes_buff[6], app->aes_buff[7], app->aes_global);
}



/*==================================================================================*
 * WinFile_Alliconify:																*
 *		WM_ALLICONIFY event for the WINDOW *win.									*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Alliconify( WINDOW *win, APPVar *app)
{
	WINDOW *scan;

	for( scan= app->win; scan; scan = scan->next) 
	{
		if( scan->status & WS_OPEN && scan != win && scan->handle != 0) 
		{
			scan->edit.edition_mode = FALSE;
			scan->status |= WS_ALLICNF;
			scan->status &= ~WS_OPEN;
			mt_wind_get_grect( scan->handle, WF_CURRXYWH, &scan->uniconified_pos, app->aes_global);
			mt_wind_close( scan->handle, app->aes_global);
		}
	}

	win->status |= WS_ALLICNF;

	WinFile_Iconify( win, app);
}



/*==================================================================================*
 * WinFile_Iconify_Redraw:															*
 *		redraw event for the iconified WINDOW *win.									*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Iconify_Redraw( WINDOW *win, APPVar *app)
{
	GRECT rdraw = *( GRECT*)&app->aes_buff[4], rlist, rwork;
	int16 xtext, ytext;

	mt_wind_get_grect( win->handle, WF_WORKXYWH, &rwork, app->aes_global);

	/* check the graf clipping rectangle */
	if( !rc_intersect(( GRECT*)&app->x, &rdraw))
		return;

	if( !rc_intersect( &rwork, &rdraw))
		return;
	
	while( !mt_wind_update( BEG_UPDATE, app->aes_global));
	v_hide_c( app->vdi_handle);
	
	mt_wind_get_grect( win->handle, WF_FIRSTXYWH, &rlist, app->aes_global);

	while( rlist.g_w && rlist.g_h) 
	{
		if( rc_intersect( &rdraw, &rlist))
		{
			ClipOn( app, &rlist);

			app->pxy[0] = rlist.g_x;
			app->pxy[1] = rlist.g_y;
			app->pxy[2] = app->pxy[0] + rlist.g_w - 1;
			app->pxy[3] = app->pxy[1] + rlist.g_h - 1;
		
			vsf_color( app->vdi_handle, ( int16)app->pref.win_background_color);
			vr_recfl( app->vdi_handle, app->pxy);
			
			ytext = rwork.g_y + rwork.g_h - app->hcell - app->y_space;
			xtext = rwork.g_x + (( rwork.g_w - win->text_iconified_txt_w + 1) >> 1);
		
			app->icon_rsc[ICON_FOLDER].ob_x = rwork.g_x + (( rwork.g_w - app->icon_rsc[ICON_FOLDER].ob_width + 1) >> 1);
			app->icon_rsc[ICON_FOLDER].ob_y = ytext - app->icon_rsc[ICON_FOLDER].ob_height - app->y_space;
			app->icon_rsc[ICON_FOLDER].ob_state = OS_NORMAL;
		
			draw_cicon( ICON_FOLDER, app);
			draw_text( xtext, ytext, win->text_iconified_txt_w, ( int16)app->pref.win_txt_color, -1, ( const char *)win->text_iconified, app);
		}

		mt_wind_get_grect( win->handle, WF_NEXTXYWH, &rlist, app->aes_global);
	}

	ClipOn( app, ( GRECT*)&app->x);
	v_show_c( app->vdi_handle, 1);
	mt_wind_update( END_UPDATE, app->aes_global);
}



