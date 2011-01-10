#include "general.h"
#include "evnt_loop.h"


/*==================================================================================*
 * void DummyEvnt:																	*
 *		Dummy Event.																*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void DummyEvnt( WINDOW *win, APPVar *app)
{
	return;
}



/*==================================================================================*
 * void ClipOn:																		*
 *		Set clipping area for drawing.												*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		app					-> Application's descriptor.							*
 * 		r     				-> the area to clip.									*
 *----------------------------------------------------------------------------------*
 * return: --																		*
 *==================================================================================*/

inline void ClipOn( APPVar *app, GRECT *r)
{
	PXY pxy[2];

	pxy[0].p_x = MAX( app->x, r->g_x);
	pxy[0].p_y = MAX( app->y, r->g_y);
	pxy[1].p_x = MIN( app->x + app->w - 1, r->g_x + r->g_w - 1);
	pxy[1].p_y = MIN( app->y + app->h - 1, r->g_y + r->g_h - 1);

	vs_clip_pxy( app->vdi_handle, pxy);
}



/*==================================================================================*
 * int16 ApplWrite:																	*
 *		Send a message to a GEM process.											*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		app					-> Application's descriptor.							*
 * 		to     				-> AES id of the targeted process.						*
 * 		msg 				-> message to send.										*
 *		selected			-> the icon is selected ?								*
 * 		b3, b4, b5, b6, b7	-> parameters.											*
 *----------------------------------------------------------------------------------*
 * return: the value returned by appl_write().										*
 *==================================================================================*/

int16 ApplWrite( APPVar *app, int16 to, int16 msg, int16 b3, int16 b4, int16 b5, int16 b6, int16 b7) 
{
	int16 buf[8];

	buf[0] = msg;
	buf[1] = app->aes_global[2];
	buf[2] = 0;
	buf[3] = b3;
	buf[4] = b4;
	buf[5] = b5;
	buf[6] = b6;
	buf[7] = b7;

	return mt_appl_write( to, 16, buf, app->aes_global);
}



/*==================================================================================*
 * void redraw_area:																*
 *		redraw a window area.														*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target WINDOW.									*
 * 		area 				-> the area to redraw.									*
 * 		window_y, window_x 	-> the window's position.								*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return: -																		*
 *==================================================================================*/ 

void redraw_area( WINDOW *win, int16 window_x, int16 window_y, GRECT *area, APPVar *app)
{
    GRECT r1, rdraw = *area;

	rdraw.g_x += window_x;
	rdraw.g_y += window_y;

	if( win->handle != 0)
	{
		*(GRECT*)&app->aes_buff[4] = rdraw;
		win->WinRedraw( win, app);
	}
	else
	{
/*		We are always the owner of the desktop?
		int16 dum, id;

		mt_wind_get( 0, WF_OWNER, &id, &dum, &dum, &dum, app->aes_global);

		if( id != app->aes_global[2])
			return;
*/
		v_hide_c( app->vdi_handle);
		mt_wind_get_grect( 0, WF_FIRSTXYWH, &r1, app->aes_global);
	
		while( r1.g_w && r1.g_h) 
		{
			if( rc_intersect( &rdraw, &r1)) 
				mt_objc_draw_grect( app->desktop_rsc, ROOT, MAX_DEPTH, &r1, app->aes_global);
	
			mt_wind_get_grect( 0, WF_NEXTXYWH, &r1, app->aes_global);
		}
	
		v_show_c( app->vdi_handle, 1);
	}
}




/*==================================================================================*
 * int16 suitable_mouse_form:														*
 *		Look if the mouse in over a resizable object and return the suitable mouse 	*
 *		form.																		*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		app			-> Application's descriptor.									*
 *----------------------------------------------------------------------------------*
 * return:	 																		*
 *      the suitable mouse form.													*
 *==================================================================================*/

int16 suitable_mouse_form( APPVar *app)
{
	int16 top, dum, xpos;
	WINDOW *win;
	TAB *tab;
	GRECT win_area;
	
	mt_wind_get( 0, WF_TOP, &top, &dum, &dum, &dum, app->aes_global);
	
	win = WindHandle( app, top);
	
	if( win == NULL)
		return ARROW;
	
	if(( win->status & WS_ICONIFY) || ( win->status & WS_FORM) || ( win->handle == 0))
		return ARROW;
	
	tab = win->selected_tab;
	
	if( tab->view_mode == LIST_MODE)
	{
		mt_wind_get_grect( win->handle, WF_WORKXYWH, &win_area, app->aes_global);
	
		xpos = win->sidebar_width + tab->name_column_width;
	
		if(( xpos >= win_area.g_w) || ( win_area.g_x + xpos >= app->w))
			return ARROW;
	
		if( IS_IN( app->mx, app->my, win_area.g_x + xpos - 3, win_area.g_y + 1, 6, COLUMN_TITLE_HEIGHT - 2))
			return 275;
	
		if( tab->size_column_width == 0)
			return ARROW;	
	
		xpos += tab->size_column_width;
	
		if(( xpos >= win_area.g_w) || ( win_area.g_x + xpos >= app->w))
			return ARROW;
	
		if( IS_IN( app->mx, app->my, win_area.g_x + xpos - 3, win_area.g_y + 1, 6, COLUMN_TITLE_HEIGHT - 2))
			return 275;	
	
		if( tab->date_column_width == 0)
			return ARROW;	
	
		xpos += tab->date_column_width;
	
		if(( xpos >= win_area.g_w) || ( win_area.g_x + xpos >= app->w))
			return ARROW;
	
		if( IS_IN( app->mx, app->my, win_area.g_x + xpos - 3, win_area.g_y + 1, 6, COLUMN_TITLE_HEIGHT - 2))
			return 275;	
	}
	
	
	return ARROW;
}



