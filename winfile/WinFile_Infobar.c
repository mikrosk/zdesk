#include "general.h"
#include "ztext.h"
#include "winfile/WinFile.h"
#include "misc.h"


/*==================================================================================*
 * draw_infobar:																	*
 *		Draw the info bar.															*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		infobar_area		-> the infobar area.									*
 * 		clip     			-> the clipping area.									* 
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

inline void draw_infobar( WINDOW *win, GRECT *infobar_area, GRECT *clip, APPVar *app)
{
	TAB *tab = win->selected_tab;	
	int16 txt_w, txt_y, pxy[4];

	pxy[0] = infobar_area->g_x;
	pxy[1] = infobar_area->g_y;
	pxy[2] = infobar_area->g_x + infobar_area->g_w - 1;
	pxy[3] = infobar_area->g_y;
	
	txt_y  = pxy[1] + (( INFOBAR_HEIGHT - app->hcell + 1) >> 1) + 1;
	
	if( !( win->status & WS_TABBAR))
	{	
		vsl_color( app->vdi_handle, ( int16)app->pref.button_on_dark_color);
		v_pline( app->vdi_handle, 2, pxy);

		pxy[1]++;
	}
		
	pxy[3] += INFOBAR_HEIGHT - 1;
	
	vsf_color( app->vdi_handle, ( int16)app->pref.button_off_background);
	vr_recfl( app->vdi_handle, pxy);	
	
	txt_w = draw_text( infobar_area->g_x + 2, txt_y, -1, ( int16)app->pref.button_off_text_color, ( app->pref.object_text_is_3D) ? ( int16)app->pref.button_off_text_shadow_color : -1, ( const char *)tab->info, app);
		
	if( tab->free_size_txt_w == 0)
		return;
		
	txt_w += 8;
		
	if( txt_w + 2 + tab->free_size_txt_w < infobar_area->g_w)
		pxy[0] = pxy[2] - 2 - tab->free_size_txt_w;
	else
		pxy[0] = infobar_area->g_x + txt_w;
	
	if( pxy[0] >= infobar_area->g_x + infobar_area->g_w - 1)
		return;
	
	draw_text( pxy[0], txt_y, tab->free_size_txt_w, ( int16)app->pref.button_off_text_color, ( app->pref.object_text_is_3D) ? ( int16)app->pref.button_off_text_shadow_color : -1, ( const char *)tab->free_size_txt, app);
}



/*==================================================================================*
 * redraw_infobar:																	*
 *		Set the clipping rectangle and redraw the info bar.							*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		rect     			-> the window area.										*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void redraw_infobar( WINDOW *win, GRECT *rect, APPVar *app)
{
	GRECT rdraw, rlist, infobar_area;
	
	infobar_area.g_x = rect->g_x;
	infobar_area.g_w = rect->g_w;
	infobar_area.g_y = rect->g_y + rect->g_h - INFOBAR_HEIGHT;
	infobar_area.g_h = INFOBAR_HEIGHT;
	
	rdraw = infobar_area;
	
	/* check the graf clipping rectangle */
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
			draw_infobar( win, &infobar_area, &rlist, app);
		}
		
		mt_wind_get_grect( win->handle, WF_NEXTXYWH, &rlist, app->aes_global);
	}
	
	ClipOn( app, ( GRECT*)&app->x);
	v_show_c( app->vdi_handle, 1);
	mt_wind_update( END_UPDATE, app->aes_global);
}



/*==================================================================================*
 * update_infobar:																	*
 *		Update the info bar and redraw it if necessary.								*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		rect     			-> the window area.										*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void update_infobar( WINDOW *win, GRECT *rect, APPVar *app)
{
	TAB *tab;
	
	if( !( win->status & WS_INFOBAR))
		return;
	
	tab = win->selected_tab;
	
	if( tab->first_selected == NULL)
	{
		if( tab->nbr_icons == 1)
			sprintf( tab->info, app->info_simple, tab->total_files_size);
		else
			sprintf( tab->info, app->info_multi, tab->total_files_size, tab->nbr_icons);
	}
	else if( tab->first_selected->next_selected == NULL)
		sprintf( tab->info, app->info_selected_simple, tab->first_selected->tsize);
	else
	{
		int16	total_selected = 0;
		double	selected_files_size = 0;
		char	tsize[32];
		Entry*	ptr = tab->first_selected;

		while( ptr)
		{
			total_selected++;
			selected_files_size += ( double)ptr->file_size;
			
			ptr = ptr->next_selected;
		}
		
		byte_to_text( tsize, selected_files_size);
		sprintf( tab->info, app->info_selected_multi, tsize, total_selected);
	}
	
	redraw_infobar( win, rect, app);
}

