#include "general.h"
#include "codecs.h"
#include "prefs.h"
#include "misc.h"
#include "desktop/desktop.h"
#include "raster/vdi.h"
#include <mint/cookie.h>
#include "ztext.h"


/*==================================================================================*
 * void ApplExit:																	*
 *		The global exit's function.													*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		app		-> Application's descriptor.										*
 *----------------------------------------------------------------------------------*
 * returns: 																		*
 *      --																			*
 *==================================================================================*/

void ApplExit( APPVar *app)
{
    while( app->win)
        app->win->WinClose( app->win, app);

	mt_menu_bar( app->menu_rsc, 0, app->aes_global);

	codecs_quit( app);

	prefs_write( &app->pref, app);

	mt_rsrc_free( app->aes_global);

	v_clsvwk( app->vdi_handle);

	mt_appl_exit( app->aes_global);

	gfree( app);

	gMemReport();
}


/*==================================================================================*
 * void ApplInit:																	*
 *		The global initialisation's function.										*
 *----------------------------------------------------------------------------------*
 * input:																			*
 *		--																			*
 *----------------------------------------------------------------------------------*
 * returns: 																		*
 *      APPvar pointer if ok else NULL.												*
 *==================================================================================*/

APPVar *ApplInit( void)
{
	APPVar	*app;
	int32	dum, error = -EERROR;
	
	app = ( APPVar*)gmalloc( sizeof( APPVar));

	if( app == NULL) 
		return NULL;

	if( mt_appl_init( app->aes_global) == -1) 
	{
		gfree( app);
		return NULL;
	}

	app->aes_handle = mt_graf_handle( NULL, NULL, NULL, NULL, app->aes_global);
	
	if( Getcookie( C_MiNT, &dum) == C_NOTFOUND)
	{
		error = EMINT;
		goto abort;
	}

	if( vdi_init( app) < 0)
	{
		error = EEDDI;
		goto abort;
	}
	
	// app->pipe = NULL;

	mt_menu_register( app->aes_global[2], "  zDesk ", app->aes_global);

	if( mt_rsrc_load( "zdesk.rsc", app->aes_global) == 0)
	{
		v_clsvwk( app->vdi_handle);
		error = ERSC;
		goto abort;
	}
	
	Pdomain( 1);

	app->zdesk_path[0] = 'A' + Dgetdrv();
	app->zdesk_path[1] = ':';
	Dgetpath( app->zdesk_path + 2, 0);
	
	prefs_read( &app->pref, app);

	error = codecs_init( app);

	if( error < 0L)
		show_alert( NULL, ECODEC, app);

	app->x_space			= 2;
	app->y_space 			= 4;
	app->hcell				= 8;

    app->desktop_picture.fd_addr	= NULL;

    app->screen.fd_addr		= NULL;
	app->screen.fd_w		= 0;
	app->screen.fd_h		= 0;
	app->screen.fd_wdwidth	= 0;
	app->screen.fd_stand	= 0;
	app->screen.fd_nplanes	= 0;
	app->screen.fd_r1		= 0;
	app->screen.fd_r2		= 0;
	app->screen.fd_r3		= 0;
	app->pic.fd_stand		= 0;
	app->pic.fd_r1			= 0;
	app->pic.fd_r2			= 0;
	app->pic.fd_r3			= 0;

	mt_rsrc_gaddr( R_TREE, DESKTOP, &app->desktop_rsc, app->aes_global);	
	mt_rsrc_gaddr( R_TREE, ICON, &app->icon_rsc, app->aes_global);
	mt_rsrc_gaddr( R_TREE, MENU_BAR, &app->menu_rsc, app->aes_global);
	mt_rsrc_gaddr( R_TREE, ABOUT, &app->about_rsc, app->aes_global);

	strcpy( app->about_rsc[ABOUT_VERSION].ob_spec.tedinfo->te_ptext, ZDESK_VERSION);
	sprintf( app->about_rsc[ABOUT_DATE].ob_spec.tedinfo->te_ptext, "%s, %s", __DATE__, __TIME__);	
	
	mt_rsrc_gaddr( R_STRING, INFO_SIMPLE, &app->info_simple, app->aes_global);
	mt_rsrc_gaddr( R_STRING, INFO_MULTI, &app->info_multi, app->aes_global);
	mt_rsrc_gaddr( R_STRING, INFO_SELECTED_SIMPLE, &app->info_selected_simple, app->aes_global);
	mt_rsrc_gaddr( R_STRING, INFO_SELECTED_MULTI, &app->info_selected_multi, app->aes_global);
	mt_rsrc_gaddr( R_STRING, AVAILABLE, &app->info_available, app->aes_global);
	mt_rsrc_gaddr( R_STRING, TNAME, &app->name_title, app->aes_global);
	mt_rsrc_gaddr( R_STRING, TSIZE, &app->size_title, app->aes_global);
	mt_rsrc_gaddr( R_STRING, TDATE, &app->date_title, app->aes_global);
	mt_rsrc_gaddr( R_STRING, FTODAY, &app->file_today, app->aes_global);
	mt_rsrc_gaddr( R_STRING, FYESTERDAY, &app->file_yesterday, app->aes_global);
	
	mt_wind_get( 0, WF_WORKXYWH, &app->x, &app->y, &app->w, &app->h, app->aes_global);
	
	mt_menu_bar( app->menu_rsc, 1, app->aes_global);

	app->size_title_txt_w = get_text_width(( const char *)app->size_title);
	app->date_title_txt_w = get_text_width(( const char *)app->date_title);
	
	ClipOn( app, ( GRECT*)&app->x);

	app->infobox = NULL;
	app->about_win = NULL;
	app->win = Desktop_Init( app);

	if( app->win == NULL)
	{
		ApplExit( app);
		return NULL;
	}

	return app;

	abort:

	show_alert( NULL, error, app);
	mt_appl_exit( app->aes_global);
	gfree( app);
	return NULL;

}

