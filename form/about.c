#include "general.h"
#include "evnt_loop.h"
#include "misc.h"
#include "menu.h"
#include "winfile/WinFile.h"


/*==================================================================================*
 * about_box_close:																	*
 *		Close the about box and free up memory.										*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void about_box_close( WINDOW *win, APPVar *app)
{
	WinFile_Close( win, app);
	app->about_win = NULL;
}



/*==================================================================================*
 * void about_box_redraw:															*
 *		redraw the about box.														*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target WINDOW.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return: -																		*
 *==================================================================================*/ 

void about_box_redraw( WINDOW *win, APPVar *app)
{
	GRECT win_area;
	GRECT r1, rdraw = *( GRECT*)&app->aes_buff[4];
	
	/* check the graf clipping rectangle */
	if( !rc_intersect(( GRECT*)&app->x, &rdraw))
		return;

	mt_wind_get_grect( win->handle, WF_WORKXYWH, &win_area, app->aes_global);
	
	if( !rc_intersect( &win_area, &rdraw))
		return;	
	
	app->about_rsc[ABOUT_ROOT].ob_x = win_area.g_x;
	app->about_rsc[ABOUT_ROOT].ob_y	= win_area.g_y;
	
	while( !mt_wind_update( BEG_UPDATE, app->aes_global));	
	mt_wind_get_grect( win->handle, WF_FIRSTXYWH, &r1, app->aes_global);	
	v_hide_c( app->vdi_handle);

	while( r1.g_w && r1.g_h) 
	{
		if( rc_intersect( &rdraw, &r1))
		{	

			ClipOn( app, &r1);
			mt_objc_draw_grect( app->about_rsc, ROOT, MAX_DEPTH, &r1, app->aes_global);
		}
		
		mt_wind_get_grect( win->handle, WF_NEXTXYWH, &r1, app->aes_global);
	}

	ClipOn( app, ( GRECT*)&app->x);
	v_show_c( app->vdi_handle, 1);
	mt_wind_update( END_UPDATE, app->aes_global);
}



/*==================================================================================*
 * void open_about_box:																*
 *		Open the about box if necessary.											*
 *----------------------------------------------------------------------------------*
 * 	app						-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void open_about_box( APPVar *app)
{
	WINDOW		*win, *last_win;	
	int16		dum, handle;
	char		*win_title;
	
	if( app->about_win)
	{
		app->about_win->WinTop( app->about_win, app);
		return;
	}		
	
	handle = mt_wind_create( MOVER|NAME|CLOSER, app->x, app->y, app->w, app->h, app->aes_global);

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
	win->status				= WS_FORM;
	win->tab				= NULL;
	win->selected_tab		= NULL;
	win->nbr_tabs			= 0;	
	win->edit.edition_mode	= FALSE;

	win->WinRedraw			= about_box_redraw;				
	win->WinIconifyRedraw	= DummyEvnt;			
	win->WinClose			= about_box_close;
	win->WinTop				= WinFile_Top;
	win->WinBottom			= WinFile_Bottom;
	win->WinMove			= WinFile_Move;
	win->WinSize			= DummyEvnt;
	win->WinFull			= DummyEvnt;
	win->WinIconify			= DummyEvnt;
	win->WinUniconify		= DummyEvnt;
	win->WinAlliconify		= DummyEvnt;
	win->WinArrow			= DummyEvnt;
	win->WinVslide			= DummyEvnt;
	win->WinKeyboard		= DummyEvnt;
	win->WinMouse			= DummyEvnt;	

	/* Put the windows in the windows list */
	win->next = NULL;
	
	for( last_win = app->win; last_win->next; last_win = last_win->next);
	
	last_win->next = win;	
	app->about_win = win;

	mt_wind_calc( WC_BORDER, MOVER|NAME|CLOSER, 0, 0, app->about_rsc[ABOUT_ROOT].ob_width, app->about_rsc[ABOUT_ROOT].ob_height, &dum, &dum, &win->w_min, &win->h_min, app->aes_global);

	mt_rsrc_gaddr( R_STRING, ABOUT_TITLE, &win_title, app->aes_global);
	mt_wind_set_str( handle, WF_NAME, win_title, app->aes_global);
	
	if( mt_wind_open( handle, ( app->w > win->w_min) ? ( app->w - win->w_min) >> 1 : 0, ( app->h - app->y > win->h_min) ? ( app->h - app->y - win->h_min) >> 1 : 0, win->w_min, win->h_min, app->aes_global) == 0)
	{
		win->WinClose( win, app);
		show_alert( NULL, EWINDOW, app);
		return;
	}

	win->status |= WS_OPEN;
	
	set_menu_status( win, app);
}




