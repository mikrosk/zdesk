#include "general.h"
#include "mfdb.h"
#include "menu.h"
#include "evnt_loop.h"
#include "winfile/WinFile.h"


/*==================================================================================*
 * WinFile_Close:																	*
 *		Close the WINDOW *win and free up memory.									*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Close( WINDOW *win, APPVar *app)
{
	/* close the window if necessary */
	if( win->status & WS_OPEN)
	{	
		mt_wind_close( win->handle, app->aes_global);
		win->status &= ~WS_OPEN;
	}	

	/* delete the window in the AES list */
	mt_wind_delete( win->handle, app->aes_global);

	/* remove the tabs */
    while( win->tab)
        WinFile_Close_Tab( win, win->tab, app);
	
	/* remove the window from the internal list */
	if( app->win == win) 
		app->win = win->next;
	else 
	{
		WINDOW *previous_win;

		for( previous_win = app->win; previous_win->next != win; previous_win = previous_win->next);

		previous_win->next = win->next;
	}

	/* set the menu items for the current top window if necessary */
	if( app->win && ( app->win->handle == 0) && ( app->win->next == NULL))
		set_menu_status( app->win, app);	
	else
	{	
		int16 top, dum;
		WINDOW *top_win;
		
		mt_wind_get( 0, WF_TOP, &top, &dum, &dum, &dum, app->aes_global);
		top_win = WindHandle( app, top);
		
		if( top_win)
		{
			if((( win->status & WS_FORM) && !( top_win->status & WS_FORM)) || (!( win->status & WS_FORM) && ( top_win->status & WS_FORM)))
				set_menu_status( top_win, app);
		}	
	}

	/* release the memory */
	gfree(( void*)win);
}

