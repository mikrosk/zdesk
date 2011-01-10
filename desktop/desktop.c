#include "general.h"
#include "mfdb.h"
#include "pic_load.h"
#include "desktop/desktop.h"
#include "winfile/WinFile.h"
#include "icon/icon.h"
#include "misc.h"


static int16 __CDECL redraw_desktop( PARMBLK *pblk) 
{
	int16 x, y;
	APPVar *app = (APPVar *)pblk->pb_parm;

	ClipOn( app, ( GRECT*)&pblk->pb_xc);

	/* ------------------- */

	if( app->desktop_picture.fd_addr)
	{
		app->pxy[0] = app->pxy[1] = 0;
		app->pxy[2] = app->desktop_picture.fd_w - 1;
		app->pxy[3] = app->desktop_picture.fd_h - 1;

		app->color[0] = BLACK;
		app->color[1] = WHITE;

		for( y = 0; y < pblk->pb_h; y += app->desktop_picture.fd_h)
		{
			for( x = 0; x < pblk->pb_w; x += app->desktop_picture.fd_w - 1)
			{
				app->pxy[4] = pblk->pb_x + x;
				app->pxy[5] = pblk->pb_y + y;
				app->pxy[6] = app->pxy[4] + app->pxy[2];
				app->pxy[7] = app->pxy[5] + app->pxy[3];

				if( app->desktop_picture.fd_nplanes == 1)
					vrt_cpyfm( app->vdi_handle, MD_REPLACE, app->pxy, &app->desktop_picture, &app->screen, app->color);
				else
					vro_cpyfm( app->vdi_handle, S_ONLY, app->pxy, &app->desktop_picture, &app->screen);
			}
		}
	}
	else
	{
		app->pxy[0] = pblk->pb_xc;
		app->pxy[1] = pblk->pb_yc;
		app->pxy[2] = pblk->pb_xc + pblk->pb_wc - 1;
		app->pxy[3] = pblk->pb_yc + pblk->pb_hc - 1;	

		vsf_color( app->vdi_handle, ( int16)app->pref.desk_background_color);
		vr_recfl( app->vdi_handle, app->pxy);
	}	

	for( x = 0; x < app->win->tab->nbr_icons; x++)
	{
		Entry *entry = &app->win->tab->entry[x];

		if( !check_selected_entry( app->win->tab, entry))
		{
			draw_icon( app->win, entry, FALSE, entry->icn_pos.g_x + app->x, entry->icn_pos.g_y + app->y, app);
			draw_text( entry->txt_pos.g_x + app->x, entry->txt_pos.g_y + app->y, entry->icon_txt_w, BLACK, -1, ( const char *)entry->name_shown, app);
		}
		else
		{
			draw_icon( app->win, entry, TRUE, entry->icn_pos.g_x + app->x, entry->icn_pos.g_y + app->y, app);
			
			if( app->win->edit.edition_mode == FALSE)
			{
				app->pxy[0] = entry->txt_pos.g_x + app->x - 1;
				app->pxy[1] = entry->txt_pos.g_y + app->y - 2;
				app->pxy[2] = app->pxy[0] + entry->txt_pos.g_w + 2;
				app->pxy[3] = app->pxy[1] + entry->txt_pos.g_h + 3;

				vsf_color( app->vdi_handle, ( int16)app->pref.win_highlight_color);
				vr_recfl( app->vdi_handle, app->pxy);
				draw_text( entry->txt_pos.g_x + app->x, entry->txt_pos.g_y + app->y, entry->icon_txt_w, ( int16)app->pref.win_txt_highlight_color, -1, ( const char *)entry->name_shown, app);
			}
		}
	}


	/* ------------------- */

	ClipOn( app, ( GRECT*)&app->x);

	return( pblk->pb_currstate & (OS_DISABLED|OS_CROSSED|OS_CHECKED|OS_OUTLINED|OS_SHADOWED));
}



/*==================================================================================*
 * Desktop_Close:																	*
 *		Restore the AES desktop and free the memory used.							*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the desktop window.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * returns: 																		*
 *      --																			*
 *==================================================================================*/

void Desktop_Close( WINDOW *win, APPVar *app)
{
	int16  i;
	OBJECT *ob;
	TAB *tab = win->tab;
	
	mt_rsrc_gaddr( R_TREE, DESKTOP, &ob, app->aes_global);
	
	app->win = win->next;

	mt_wind_set( 0, WF_NEWDESK, 0, 0, 0, 0, app->aes_global);

	delete_mfdb( &app->desktop_picture);

	for( i = 0 ; i < tab->nbr_icons ; i++)
		delete_mfdb( &tab->entry[i].thumbnail);

	if( tab->entry)
		gfree(( void*)tab->entry);

	gfree(( void*)tab);
	gfree(( void*)ob[DESKTOP_BACK].ob_spec.userblk);
	gfree(( void*)win);
}



/*==================================================================================*
 * WINDOW *Desktop_Init:															*
 *		Initialise the desktop.														*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		app			-> Application's descriptor.									*
 *----------------------------------------------------------------------------------*
 * returns: 																		*
 *      The WINDOW pointer.															*
 *==================================================================================*/

WINDOW *Desktop_Init( APPVar *app)
{
	WINDOW	*desk;
	TAB		*desk_tab;
	int32	error = -EERROR;
	USERBLK	*user;
	OBJECT	*ob;

	mt_rsrc_gaddr( R_TREE, DESKTOP, &ob, app->aes_global);
	
	desk = ( WINDOW*)gmalloc( sizeof( WINDOW));

	if( desk == NULL)
	{
		show_alert( NULL, -ENOMEM, app);
		return NULL;
	}

    user = ( USERBLK*)gmalloc( sizeof( USERBLK));

    if( user == NULL) 
	{
		gfree(( void*)desk);
		show_alert( NULL, -ENOMEM, app);
		return NULL;
	}

	desk_tab = ( TAB*)gmalloc( sizeof( TAB));
	
	if( desk_tab == NULL)
	{
		gfree(( void*)user);
		gfree(( void*)desk);
		show_alert( NULL, -ENOMEM, app);
		return NULL;
	}
	
	
    user->ub_code = redraw_desktop;
    user->ub_parm = ( int32)app;

	ob[DESKTOP_BACK].ob_x				= app->x;
	ob[DESKTOP_BACK].ob_y				= app->y;
	ob[DESKTOP_BACK].ob_width			= app->w;
	ob[DESKTOP_BACK].ob_height			= app->h;
    ob[DESKTOP_BACK].ob_type			= G_USERDEF;
    ob[DESKTOP_BACK].ob_spec.userblk	= user;

	desk_tab->name[0]					= '\0';	
	desk_tab->name_shown[0]				= '\0';
	desk_tab->tab_txt_w					= 0;	
	desk_tab->name_column_width			= 0;
	desk_tab->size_column_width			= 0;
	desk_tab->date_column_width			= 0;
	desk_tab->xpos						= 0;
	desk_tab->ypos						= 0;
	desk_tab->ypos_max					= 1;
	desk_tab->h_u						= STANDARD_ICON_SIZE + app->hcell + app->y_space + app->y_space + app->y_space;
	desk_tab->w_u						= 68 + app->x_space + app->x_space;
	desk_tab->xpos_max					= ( app->x + app->w) / desk_tab->w_u;
	desk_tab->sortingmode				= MENU_BAR_BY_NAME;
	desk_tab->sort_dirs_before_files	= FALSE;
	desk_tab->view_mode					= ICON_MODE;
	desk_tab->icon_size					= STANDARD_ICON_SIZE;
	desk_tab->icons_last_line			= 0;
	desk_tab->entry						= NULL;
	desk_tab->first_selected			= NULL;	
	desk_tab->nbr_icons					= 0;	
	desk_tab->total_files_size[0]		= '\0';
	desk_tab->info[0]					= '\0';
	desk_tab->free_size_txt[0]			= '\0';
	desk_tab->free_size_txt_w			= 0;
	desk_tab->scan_dir					= NULL;
	desk_tab->rename_entry				= NULL;
	desk_tab->get_disk_size				= NULL;		
	desk_tab->next						= NULL;
	
	desk->handle						= 0;
	desk->w_min							= 0;
	desk->h_min							= 0;
	desk->status						= WS_OPEN;
	desk->sidebar_width					= 0;
	desk->tab							= desk_tab;
	desk->selected_tab					= desk_tab;
	desk->nbr_tabs						= 1;	
	desk->edit.edition_mode				= FALSE;	
	desk->WinRedraw						= DummyEvnt;				
	desk->WinIconifyRedraw				= DummyEvnt;			
	desk->WinClose						= Desktop_Close;
	desk->WinTop						= DummyEvnt;
	desk->WinBottom						= DummyEvnt;
	desk->WinMove						= DummyEvnt;
	desk->WinSize						= DummyEvnt;
	desk->WinFull						= DummyEvnt;
	desk->WinIconify					= DummyEvnt;
	desk->WinUniconify					= DummyEvnt;
	desk->WinAlliconify					= DummyEvnt;
	desk->WinArrow						= DummyEvnt;
	desk->WinVslide						= DummyEvnt;
	desk->WinKeyboard					= DummyEvnt;
	desk->WinMouse						= WinFile_Mouse;		
	desk->next							= NULL;
	

/*	desk->scan_dir				= scan_dir_file;
	desk->rename_entry			= rename_entry_file;
*/

	error = scan_desktop( desk_tab, app);

	if( error != 0)
	{
		gfree(( void*)desk_tab);
		gfree(( void*)user);
		gfree(( void*)desk);
		show_alert( NULL, error, app);
		return NULL;
	}

	if( app->pref.desk_pic[0] != '\0')
		pic_load(( const char *)app->pref.desk_pic, FALSE, &app->desktop_picture, app);

	app->win = desk;
	
	mt_wind_set( 0, WF_NEWDESK, ADR(ob), 0, 0, app->aes_global);

	redraw_area( desk, 0, 0, ( GRECT*)&app->x, app);

	return desk; 
}
