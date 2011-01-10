#include "general.h"
#include "ztext.h"
#include "misc.h"
#include "icon/icon.h"


/*==================================================================================*
 * draw_pathbar:																	*
 *		Draw the path bar.															*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		pathbar_area		-> the path bar area.									*
 * 		clip     			-> the clipping area.									* 
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

inline void draw_pathbar( WINDOW *win, GRECT *pathbar_area, GRECT *clip, APPVar *app)
{
	int16 pxy[12], text_color;

	vsl_color( app->vdi_handle, ( int16)app->pref.button_on_dark_color);

	pxy[0] = pathbar_area->g_x;
	pxy[1] = pathbar_area->g_y + pathbar_area->g_h - 1;	
	pxy[2] = pxy[0] + pathbar_area->g_w - 1;
	pxy[3] = pxy[1];
	v_pline( app->vdi_handle, 2, pxy);	
	
	/*
	for( tab = win->tab; tab; tab = tab->next)
	{	
		if( tab->tab_xpos == -1)
			continue;
			
		pxy[0] = tabbar_area->g_x + tab->tab_xpos;
		pxy[1] = tabbar_area->g_y + 1;
		pxy[2] = pxy[0];
		pxy[3] = tabbar_area->g_y + tabbar_area->g_h - 1;
			
		if( tab == win->selected_tab)
		{	
			vsl_color( app->vdi_handle, ( int16)app->pref.button_off_background);
			v_pline( app->vdi_handle, 2, pxy);

			pxy[0] += tab->tab_width - 1;
			pxy[2] = pxy[0];
			v_pline( app->vdi_handle, 2, pxy);
				
			pxy[0] = tabbar_area->g_x + tab->tab_xpos + 2;
			pxy[2] = pxy[0] + tab->tab_width - 5;

			vsf_color( app->vdi_handle, ( int16)app->pref.win_background_color);
				
			if( app->pref.tab_on_highlight_color > -1)
			{
				pxy[1]--;
				pxy[3] -= 3;
				vr_recfl( app->vdi_handle, pxy);
					
				pxy[1] = pxy[3] + 1;
				pxy[3] += 2;
				vsf_color( app->vdi_handle, ( int16)app->pref.tab_on_highlight_color);
			}
			else 
			{
				pxy[1]--;
				pxy[3]--;
			}

			text_color = ( int16)app->pref.win_txt_color;
		}	
		else
		{	
			vsl_color( app->vdi_handle, ( int16)app->pref.button_off_background);
			pxy[4] = pxy[0] + tab->tab_width - 1;
			pxy[5] = pxy[3];
			pxy[6] = pxy[4];
			pxy[7] = pxy[1];
			v_pline( app->vdi_handle, 4, pxy);

			vsl_color( app->vdi_handle, ( int16)app->pref.button_on_dark_color);
			pxy[0]++;
			pxy[2] = pxy[0];
			pxy[3]--;
			pxy[4]--;
			pxy[5] = pxy[3];
			pxy[6] = pxy[4];
			v_pline( app->vdi_handle, 4, pxy);				
				
			vsf_color( app->vdi_handle, ( int16)app->pref.tab_off_background_color);
			pxy[0]++;
			pxy[2] = pxy[4] - 1;
			pxy[3]--;
				
			text_color = ( int16)app->pref.tab_off_text_color;
		}
			
		vr_recfl( app->vdi_handle, pxy);
			
		pxy[0] = tabbar_area->g_x + tab->tab_xpos + 4;
			
		app->icon_rsc[tab->tab_icon].ob_x = pxy[0];
		app->icon_rsc[tab->tab_icon].ob_y = tabbar_area->g_y + 1;
		app->icon_rsc[tab->tab_icon].ob_state = OS_NORMAL;
			
		draw_cicon( tab->tab_icon, app);
					
		draw_text( pxy[0] + MINI_ICON_SIZE + 1 + (( tab->tab_width - MINI_ICON_SIZE - 8 - tab->tab_txt_w) >> 1), tabbar_area->g_y + 6, tab->tab_txt_w, text_color, -1, ( const char *)tab->name_shown, app);		
	}
	
	if( win->status & WS_TABHIDDEN)
	{
		static uint16 arrow_bitmap[9] = { 0x0200, 0x0300, 0x0F80, 0x0FC0, 0x0FE0, 0x0FC0, 0x0F80, 0x0300, 0x0200}; 
		
		pxy[0] = pxy[1] = 0;
		pxy[2] = 15;
		pxy[3] = 8;
		pxy[4] = tabbar_area->g_x + tabbar_area->g_w - 18;
		pxy[5] = tabbar_area->g_y + 5;
		pxy[6] = pxy[4] + pxy[2];
		pxy[7] = pxy[5] + pxy[3];
		
		app->pic.fd_w		= 16;
		app->pic.fd_h		= 9;
		app->pic.fd_wdwidth	= 1;
		app->pic.fd_nplanes	= 1;
		app->pic.fd_addr	= &arrow_bitmap;

		if( win->status & WS_LAST_TAB_IS_SELECTED)
		{
			app->color[0] = ( int16)app->pref.win_txt_color;
			app->color[1] = ( int16)app->pref.win_background_color;
		}
		else
		{
			app->color[0] = ( int16)app->pref.tab_off_text_color;	
			app->color[1] = ( int16)app->pref.tab_off_background_color;
		}
		
		vrt_cpyfm( app->vdi_handle, MD_REPLACE, pxy, &app->pic, &app->screen, app->color);
	}
	else if( win->selected_tab->tab_width == MAXIMAL_TAB_WIDTH)
*/	{
		vsf_color( app->vdi_handle, ( int16)app->pref.button_off_background);
		pxy[0] = pathbar_area->g_x ;
		pxy[1] = pathbar_area->g_y;
		pxy[2] = pxy[0] + pathbar_area->g_w;
		pxy[3] = pxy[1] + pathbar_area->g_h - 2;
		vr_recfl( app->vdi_handle, pxy);
	}
}


/*==================================================================================*
 * redraw_tabbar:																	*
 *		Set the clipping rectangle and redraw the tabs bar.							*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		file_area     		-> the file area( can be NULL).							*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/
/*	   
void redraw_tabbar( WINDOW *win, GRECT *file_area, APPVar *app)
{
	GRECT rdraw, rlist, tabbar_area;
	
	if( file_area)
	{	
		tabbar_area.g_x = file_area->g_x;
		tabbar_area.g_w = file_area->g_w;
		tabbar_area.g_y = file_area->g_y + file_area->g_h;
		tabbar_area.g_h = TABBAR_HEIGHT;
	}	
	else
	{
		GRECT full_area;
		
		mt_wind_get_grect( win->handle, WF_WORKXYWH, &full_area, app->aes_global);
		
		tabbar_area.g_x = full_area.g_x + win->sidebar_width;
		tabbar_area.g_w = full_area.g_w - win->sidebar_width;
		tabbar_area.g_y = full_area.g_y + full_area.g_h - TABBAR_HEIGHT;
		tabbar_area.g_h = TABBAR_HEIGHT;
		
		if( win->status & WS_INFOBAR)
			tabbar_area.g_y -= INFOBAR_HEIGHT;
	}


	rdraw = tabbar_area;

	if( !rc_intersect(( GRECT*)&app->x, &rdraw))
		return;

	while( !mt_wind_update( BEG_UPDATE, app->aes_global));
		v_hide_c( app->vdi_handle);

	mt_wind_get_grect( win->handle, WF_FIRSTXYWH, &rlist, app->aes_global);

	while( rlist.g_w && rlist.g_h) 
	{
		if( rc_intersect( &rdraw, &rlist))
		{
			ClipOn( app, &rlist);
			draw_tabbar( win, &tabbar_area, &rlist, app);
		}

		mt_wind_get_grect( win->handle, WF_NEXTXYWH, &rlist, app->aes_global);
	}

	ClipOn( app, ( GRECT*)&app->x);
	v_show_c( app->vdi_handle, 1);
	mt_wind_update( END_UPDATE, app->aes_global);
}
*/
	   
	   
/*==================================================================================*
 * update_tabs:																		*
 *		Update the size, position and texts of the tabs.							*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		tabbar_area_width	-> the width of the tabs area( if -1, the function gets	*
 *							   it itself).											* 
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/
/*
void update_tabs( WINDOW *win, int16 tabbar_area_width, APPVar *app)
{
	int16 dum, xpos = 0, i, optimal_width, wanted_width, tab_width;
	TAB *tab, *last_visible = NULL;
	
	if( tabbar_area_width == -1)
	{
		mt_wind_get( win->handle, WF_WORKXYWH, &dum, &dum, &tab_width, &dum, app->aes_global);
		tab_width -= win->sidebar_width;
	}	
	else		
		tab_width = tabbar_area_width;

	win->status &= ~WS_TABHIDDEN;
	win->status &= ~WS_LAST_TAB_IS_SELECTED;
	
	optimal_width = tab_width / win->nbr_tabs;
	
	if( optimal_width >= MAXIMAL_TAB_WIDTH)
		wanted_width = MAXIMAL_TAB_WIDTH;
	else if( optimal_width <= MINIMAL_TAB_WIDTH)
	{	
		int16 extra_space;
		
		for( i = win->nbr_tabs; i * MINIMAL_TAB_WIDTH > tab_width; i--);
		
		extra_space = tab_width - ( i * MINIMAL_TAB_WIDTH);
		wanted_width = MINIMAL_TAB_WIDTH + ( extra_space / i);
	}	
	else
		wanted_width = optimal_width;

	
	for( tab = win->tab; tab; tab = tab->next)
	{
		if(( xpos + wanted_width) <= tab_width)
		{
			last_visible = tab;
			tab->tab_xpos = xpos;

			if((( tab->next == NULL) || (( xpos + wanted_width + wanted_width) > tab_width)) && ( wanted_width < MAXIMAL_TAB_WIDTH))
			{	
				wanted_width = tab_width - xpos;
				
				if( tab == win->selected_tab)
					win->status |= WS_LAST_TAB_IS_SELECTED;
			}	

			if( tab->tab_width != wanted_width)
			{
				if( tab->tab_txt_w != tab->real_txt_w)
				{
					zstrncpy( tab->name_shown, ( const char *)tab->name, TAB_NAME_SHOW_CHARS);
					tab->tab_txt_w = tab->real_txt_w;
				}
				
				if( tab->tab_txt_w > ( wanted_width - MINI_ICON_SIZE - 8))
					tab->tab_txt_w = name_shorter( wanted_width - MINI_ICON_SIZE - 8, tab->name_shown);
			}
			
			tab->tab_width = wanted_width;
		}
		else
		{
			tab->tab_xpos = -1;
			tab->tab_width = wanted_width;
			win->status |= WS_TABHIDDEN;
		}
		
		xpos += wanted_width;
	}
	
	if( win->selected_tab->tab_xpos == -1)
	{
		if( last_visible)
		{	
			tab = win->selected_tab;
			
			tab->tab_xpos = last_visible->tab_xpos;
			last_visible->tab_xpos = -1;

			if( last_visible->tab_width < MAXIMAL_TAB_WIDTH)
				tab->tab_width = tab_width - tab->tab_xpos;
			else
				tab->tab_width = last_visible->tab_width;			
			
			if( tab->tab_txt_w != tab->real_txt_w)
			{
				zstrncpy( tab->name_shown, ( const char *)tab->name, TAB_NAME_SHOW_CHARS);
				tab->tab_txt_w = tab->real_txt_w;
			}
			
			if( tab->tab_txt_w > ( tab->tab_width - MINI_ICON_SIZE - 8))
				tab->tab_txt_w = name_shorter( tab->tab_width - MINI_ICON_SIZE - 8, tab->name_shown);			
		}	
		else
			win->selected_tab->tab_xpos = 0;
		
		win->status |= WS_LAST_TAB_IS_SELECTED;
	}
}
*/


