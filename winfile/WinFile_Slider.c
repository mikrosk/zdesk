#include "general.h"
#include "misc.h"
#include "winfile/WinFile.h"
#include "edit.h"


/*==================================================================================*
 * void move_screen:																*
 *		Move a part of the screen memory.											*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		app					-> Application's descriptor.							*
 * 		screen 				-> The part of the screen to move.						*
 * 		dy	 				-> y position.											*
 *----------------------------------------------------------------------------------*
 * return: --																		*
 *==================================================================================*/

static void move_screen( APPVar *app, GRECT *screen, int16 dy) 
{
	GRECT g;

	/* get intersection with screen area */
	mt_wind_get_grect( 0, WF_CURRXYWH, &g, app->aes_global);

	rc_intersect( &g, screen);

	app->pxy[0] = screen->g_x;
	app->pxy[1] = screen->g_y;
	app->pxy[2] = app->pxy[0] + screen->g_w - 1;
	app->pxy[3] = app->pxy[1] + screen->g_h - 1;
	app->pxy[4] = app->pxy[0];
	app->pxy[5] = app->pxy[1] + dy;
	app->pxy[6] = app->pxy[2];
	app->pxy[7] = app->pxy[3] + dy;

	vro_cpyfm( app->vdi_handle, S_ONLY, app->pxy, &app->screen, &app->screen);
}



/*==================================================================================*
 * void move_work_rect:																*
 *		Execute a 'smooth' scrolling.												*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		app					-> Application's descriptor.							*
 * 		win     			-> the target window.									*
 * 		rect 				-> The screen's area.									*
 * 		dy	 				-> y position.											*
 *----------------------------------------------------------------------------------*
 * return: --																		*
 *==================================================================================*/

void move_work_rect( APPVar *app, WINDOW *win, GRECT *rect, int16 dy) 
{
	GRECT r1, r2;

	while( !mt_wind_update( BEG_UPDATE, app->aes_global));
	v_hide_c( app->vdi_handle);
	mt_wind_get_grect( win->handle, WF_FIRSTXYWH, &r1, app->aes_global);

	while( r1.g_w && r1.g_h)
	{
		if( !rc_intersect( rect, &r1))
		{
			mt_wind_get_grect( win->handle, WF_NEXTXYWH, &r1, app->aes_global);
			continue;
		}

		if( abs(dy) < r1.g_h)
		{
			ClipOn( app, &r1);

			r2 = r1; /* save */
	
			if (dy > 0) 
			{
				r1.g_y += dy;
				r1.g_h -= dy;
			} 
			else
				r1.g_h += dy;
			
			move_screen( app, &r1, -dy);

			if (dy)
			{
				r1 = r2 ;  /* restore */

				if (dy > 0) 
				{
					r1.g_y += r1.g_h - dy;
					r1.g_h = dy;
				} 
				else
					r1.g_h = - dy;

				*(GRECT*)&app->aes_buff[4] = r1;
				win->WinRedraw( win, app);
			}
		}
		else
		{
			*(GRECT*)&app->aes_buff[4] = r1;
			win->WinRedraw( win, app);
		}

		mt_wind_get_grect( win->handle, WF_NEXTXYWH, &r1, app->aes_global);
	}

	ClipOn( app, ( GRECT*)&app->x);
	v_show_c( app->vdi_handle, 1);
	mt_wind_update( END_UPDATE, app->aes_global);
}



/*==================================================================================*
 * WinFile_Slider:																	*
 *		Compute and set the vertical slider size.									*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		win_height     		-> window's height.										*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Slider( WINDOW *win, int32 win_height, APPVar *app)
{
	TAB *tab = win->selected_tab;	
	int32 size, pos = 0;
	int32 page = win_height / ( int32)tab->h_u;

	if( tab->ypos_max < page)
		size = 1000L;
	else
		size = MAX( 50L, win_height * 1000L / ( int32)( tab->ypos_max * tab->h_u));

	mt_wind_set( win->handle, WF_VSLSIZE, ( int16)size , 0, 0, 0, app->aes_global);

	if( tab->ypos_max > page)
		pos = tab->ypos * 1000L / ( int32)( tab->ypos_max - page);
	else if( tab->ypos)
		tab->ypos = 0;

	mt_wind_set( win->handle, WF_VSLIDE, ( int16)pos , 0, 0, 0, app->aes_global);
}



/*==================================================================================*
 * void move_work:																	*
 *		Setup the 'smooth' scrolling.												*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		app					-> Application's descriptor.							*
 * 		win     			-> the target window.									*
 * 		rect 				-> The screen's area.									*
 * 		dy	 				-> y position.											*
 *----------------------------------------------------------------------------------*
 * return: --																		*
 *==================================================================================*/

static void move_work( APPVar *app, WINDOW *win, GRECT *work, int16 dy)
{
	GRECT screen;

	if( !dy) return;

	WinFile_Slider( win, ( int32)work->g_h, app);

	mt_wind_get_grect( 0, WF_CURRXYWH, &screen, app->aes_global);

	if( rc_intersect( &screen, work))
		move_work_rect( app, win, work, dy);
}



/*==================================================================================*
 * WinFile_Vslide:																	*
 *		WM_VSLID event for the WINDOW *win.											*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Vslide( WINDOW *win, APPVar *app)
{
	int32 	pos;
	GRECT 	rect;
	int16 	dy;
	TAB *tab = win->selected_tab;	

	WinFile_Calc_Areas( win, NULL, &rect, app);
	
	pos = ( int32)( tab->ypos_max - rect.g_h / tab->h_u) * ( int32)app->aes_buff[4] / 1000L;

	if( pos < 0) 
		pos = 0;

	dy = ( int16)(( pos - tab->ypos) * tab->h_u);

	tab->ypos =  ( int32)pos;

	if( win->edit.edition_mode == TRUE)
		exit_edit_mode( win, tab->first_selected, rect.g_x, rect.g_y, app);	
	
/*	if( dy) */
		move_work( app, win, &rect, dy);
}



static void dnpgd( WINDOW *win, TAB *tab, GRECT *rect, APPVar *app)
{
	int16 	dy;
	int32 	old_pos = tab->ypos, page = ( int32)( rect->g_h / tab->h_u);

	if( tab->ypos < tab->ypos_max - page)
	{
		tab->ypos = MIN( tab->ypos_max, tab->ypos) + page;
		tab->ypos = MIN( tab->ypos, tab->ypos_max - page);
		dy = ( int16)(( tab->ypos - old_pos) * tab->h_u);
		move_work( app, win, rect, dy);
	}
}


static void uppgd( WINDOW *win, TAB *tab, GRECT *rect, APPVar *app)
{
	int32 	pos;
	int16 	dy;

	if( tab->ypos > 0L)
	{
		pos = MAX( 0L, tab->ypos - rect->g_h / tab->h_u);
		dy = ( int16)(( pos - tab->ypos) * tab->h_u);
		tab->ypos = pos;
		move_work( app, win, rect, dy);
	}
}


static void dnlnd( WINDOW *win, TAB *tab, GRECT *rect, APPVar *app)
{
	int32 page = ( int32)( rect->g_h / tab->h_u);

	if(( tab->ypos < tab->ypos_max - page) && ( tab->ypos_max > page))
	{
		tab->ypos++;
		move_work( app, win, rect, tab->h_u);
	}
}



static void uplnd( WINDOW *win, TAB *tab, GRECT *rect, APPVar *app)
{
	if( tab->ypos > 0L)
	{
		tab->ypos--;
		move_work( app, win, rect, -tab->h_u);
	}
}



/*==================================================================================*
 * WinFile_Arrow:																	*
 *		WM_ARROWED event for the WINDOW *win.										*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Arrow( WINDOW *win, APPVar *app)
{
	GRECT rect;
	TAB *tab = win->selected_tab;	
	int16 key = app->aes_buff[4];

	WinFile_Calc_Areas( win, NULL, &rect, app);

	if( win->edit.edition_mode == TRUE)
		exit_edit_mode( win, tab->first_selected, rect.g_x, rect.g_y, app);	
	
	switch( key)
	{
		case WA_UPPAGE:
			uppgd( win, tab, &rect, app);
			break;

		case WA_DNPAGE:
			dnpgd( win, tab, &rect, app);
			break;

		case WA_UPLINE:
			uplnd( win, tab, &rect, app);
			break;

		case WA_DNLINE:
			dnlnd( win, tab, &rect, app);
			break;
	}
}

