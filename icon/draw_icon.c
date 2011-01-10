#include "general.h"
#include "ztext.h"
#include "misc.h"


static void CDECL draw_monoicon( int16 icon, APPVar *app)
{
	int16 tmp_color;
	ICONBLK *iconblk = &app->icon_rsc[icon].ob_spec.ciconblk->monoblk;

	if( app->icon_rsc[icon].ob_state & OS_SELECTED)
	{
		app->color[1] = ((iconblk->ib_char) >> 8) & 0xf;
		tmp_color = app->color[0] = ((iconblk->ib_char) >> 12) & 0xf;
	}
	else
	{
		app->color[1] = ((iconblk->ib_char) >> 12) & 0xf;
		tmp_color = app->color[0] = ((iconblk->ib_char) >> 8) & 0xf;
	}

	app->pic.fd_addr = iconblk->ib_pmask;
	vrt_cpyfm( app->vdi_handle, MD_TRANS, app->pxy, &app->pic, &app->screen, app->color);

	app->pic.fd_addr = iconblk->ib_pdata;
	app->color[0] = app->color[1];
	app->color[1] = tmp_color;
	vrt_cpyfm( app->vdi_handle, MD_TRANS, app->pxy, &app->pic, &app->screen, app->color);
}



void CDECL draw_cicon( int16 icon, APPVar *app)
{
/*	static unsigned short dismskb[1600];
	static MFDB Mmask, Mddm = { dismskb, 160, 160, 10, 1, 1, 0, 0, 0}; */
	MFDB Mmask, Mddm = { app->buffer, 160, 160, 10, 1, 1, 0, 0, 0};	
	
	ICONBLK *iconblk = &app->icon_rsc[icon].ob_spec.ciconblk->monoblk;
	CICON	*c = app->icon_rsc[icon].ob_spec.ciconblk->mainlist, *best_cicon = NULL;

	while( c)
	{
		if( c->num_planes <= app->nplanes && ( !best_cicon || ( c->num_planes > best_cicon->num_planes)))
			best_cicon = c;
	
		c = c->next_res;	
	}

	app->pxy[0] = app->pxy[1] = 0;
	app->pxy[2] = app->pxy[6] = iconblk->ib_wicon - 1;
	app->pxy[3] = app->pxy[7] = iconblk->ib_hicon - 1;
	app->pxy[4] = iconblk->ib_xicon + app->icon_rsc[icon].ob_x;
	app->pxy[5] = iconblk->ib_yicon + app->icon_rsc[icon].ob_y;

	app->pic.fd_w		= iconblk->ib_wicon;
	app->pic.fd_h		= iconblk->ib_hicon;
	app->pic.fd_wdwidth	= ( iconblk->ib_wicon + 15) >> 4;
	app->pic.fd_nplanes	= 1;

	Mmask = app->pic;

	if( best_cicon == NULL)
	{
		draw_monoicon( icon, app);
		return;
	}

	if(( app->icon_rsc[icon].ob_state & OS_SELECTED) && best_cicon->sel_data)
	{
		app->pic.fd_addr = best_cicon->sel_data;
		Mmask.fd_addr = best_cicon->sel_mask;
	}
	else
	{
		app->pic.fd_addr = best_cicon->col_data;
		Mmask.fd_addr = best_cicon->col_mask;
	}

	app->color[0] = WHITE;
	app->color[1] = BLACK;

	vrt_cpyfm( app->vdi_handle, MD_TRANS, app->pxy, &Mmask, &app->screen, app->color);

	app->pic.fd_nplanes = app->nplanes;

	vro_cpyfm( app->vdi_handle, app->nplanes > 8 ? S_AND_D : S_OR_D, app->pxy, &app->pic, &app->screen);

	if(( app->icon_rsc[icon].ob_state & OS_SELECTED) && ( best_cicon->sel_data == NULL))
	{
		int i, j, sc;
		uint16 *s, *d, m;
		uint32 sm = 0xAAAA5555L;

		app->color[0] = BLACK;
		app->color[1] = WHITE;

		s = Mmask.fd_addr;
		d = Mddm.fd_addr;
		sc = Mddm.fd_wdwidth - Mmask.fd_wdwidth;

		for( i = 0; i < iconblk->ib_hicon; i++)
		{
			m = sm;
			for( j = 0; j < app->pic.fd_wdwidth; j++)
				*d++ = *s++ & m;

			sm = ( sm << 16) | ( sm >> 16);
			d += sc;
		}

		vrt_cpyfm( app->vdi_handle, MD_TRANS, app->pxy, &Mddm, &app->screen, app->color);
	}
}



/*==================================================================================*
 * void draw_icon:																	*
 *		draw an icon in a window.													*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     	-> the target WINDOW.											*
 * 		entry 		-> the entry to draw.											*
 *		selected	-> the icon is selected ?										*
 * 		x, y		-> Destination.													*
 * 		app			-> Application's descriptor.									*
 *----------------------------------------------------------------------------------*
 * return: -																		*
 *==================================================================================*/

void CDECL draw_icon( WINDOW *win, Entry *entry, boolean selected, int16 x, int16 y, APPVar *app)
{
	if(( win->selected_tab->view_mode == ICON_MODE) && entry->thumbnail.fd_addr)
    {
		/* Source Rectangle */
		app->pxy[0] = app->pxy[1] = 0;
		app->pxy[2] = entry->thumbnail.fd_w - 1;
		app->pxy[3] = entry->thumbnail.fd_h - 1;

		/* destination Rectangle */
		app->pxy[4] = x;
		app->pxy[5] = y;
		app->pxy[6] = x + app->pxy[2];
		app->pxy[7] = y + app->pxy[3];

		if( entry->thumbnail.fd_nplanes == 1)
	    {
			app->color[0] = BLACK;
			app->color[1] = WHITE;
			vrt_cpyfm( app->vdi_handle, MD_REPLACE, app->pxy, &entry->thumbnail, &app->screen, app->color);
		}
		else
			vro_cpyfm( app->vdi_handle, S_ONLY, app->pxy, &entry->thumbnail, &app->screen);
	}
	else
    {
		int16 icon = ( win->selected_tab->view_mode == ICON_MODE) ? entry->icon : entry->mini_icon;

		if( selected == TRUE)
        	app->icon_rsc[icon].ob_state = OS_SELECTED;
		else
        	app->icon_rsc[icon].ob_state = OS_NORMAL;

		app->icon_rsc[icon].ob_x = x;
		app->icon_rsc[icon].ob_y = y;

		draw_cicon( icon, app);
	}

	if( entry->status & ES_LINK)
    {
		app->icon_rsc[ICON_LINK].ob_x = x;
		app->icon_rsc[ICON_LINK].ob_y = y + win->selected_tab->icon_size - MINI_ICON_SIZE;

		draw_cicon( ICON_LINK, app);
	}
	else if( entry->status & ES_LOCKED)
    {
		app->icon_rsc[ICON_LOCKED].ob_x = x;
		app->icon_rsc[ICON_LOCKED].ob_y = y + win->selected_tab->icon_size - MINI_ICON_SIZE;

		draw_cicon( ICON_LOCKED, app);
	}
	else if( entry->status & ES_DISABLED)
    {
		app->icon_rsc[ICON_DISABLED].ob_x = x;
		app->icon_rsc[ICON_DISABLED].ob_y = y + win->selected_tab->icon_size - MINI_ICON_SIZE;
		
		draw_cicon( ICON_DISABLED, app);
	}	
}


