#include "general.h"
#include "appl.h"
#include "misc.h"
#include "menu.h"
#include "winfile/WinFile.h"


/*==================================================================================*
 * WINDOW *WindHandle:																*
 *		We give the window's handle and the function return the WINDOW struct.		*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		app			-> Application's descriptor.									*
 * 		handle		-> The window's handle.											* 
 *----------------------------------------------------------------------------------*
 * return:	 																		*
 *      The WINDOW struct. of the window.											*
 *==================================================================================*/

WINDOW *WindHandle( APPVar *app, int16 handle)
{
	register WINDOW *win;

	for( win = app->win; win; win = win->next)
		if( win->handle == handle)
			break;

	return win;
}



/*==================================================================================*
 * void EvntLoop:																	*
 *		Wait and send the AES messages to the application.							*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		app			-> Application's descriptor.									*
 *----------------------------------------------------------------------------------*
 * return:	 																		*
 *      --																			*
 *==================================================================================*/

void EvntLoop( APPVar *app) 
{
	int16 top, dum, evnt_res, quit = 0;
	WINDOW *win;

	while( !quit)
	{
		evnt_res = mt_evnt_multi( MU_MESAG|MU_TIMER|MU_KEYBD|MU_BUTTON,
				258, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, app->aes_buff, 100L,
				&app->mx, &app->my, &app->mbut, &app->mkstate, &app->keybd,
				&app->nb_click, app->aes_global);

		
		/* to do: AP_DRAGDROP, SH_WDRAW, WM_NEWTOP, WM_UNTOPPED, WM_ONTOP */ 

	
		/* handle message events */
		if( evnt_res & MU_MESAG)
		{
			if( app->aes_buff[0] == AP_TERM)
				quit = 1;
			else if( app->aes_buff[0] == MN_SELECTED)
			{
				mt_menu_tnormal( app->menu_rsc, app->aes_buff[3], UNHIGHLIGHT, app->aes_global);

				mt_wind_get( 0, WF_TOP, &top, &dum, &dum, &dum, app->aes_global);
				win = WindHandle( app, top);
				
				switch( app->aes_buff[4])
				{
					case MENU_BAR_ABOUT:
						open_about_box( app);
						break;					
					case MENU_BAR_NEW_BROWSER:
						open_new_tab( NULL, app);
						break;
					case MENU_BAR_NEW_TAB:
						open_new_tab( win, app);
						break;		
					case MENU_BAR_CLOSE_WIN:
						if( win && ( win->handle != 0))
							win->WinClose( win, app);
						break;						
					case MENU_BAR_CLOSE_TAB:
						close_tab( win, NULL, app);
						break;
					case MENU_BAR_INFO:
						open_info_box( win, app);	
						break;						
					case MENU_BAR_LIST:
						show_as_list( win, app);
						break;
					case MENU_BAR_ICON:
						show_as_icon( win, app);
						break;
					case MENU_BAR_INFOBAR:
						show_infobar( win, app);
						break;	
					case MENU_BAR_TABBAR:	
						show_tabbar( win, app);
						break;
					case MENU_BAR_QUIT:
						quit = 1;
						break;
					default:
						break;
				}	
			}
			else
			{
				win = WindHandle( app, app->aes_buff[3]);

				if( win)
				{
					switch( app->aes_buff[0])
					{
						case WM_REDRAW:
							if( win->status & WS_ICONIFY)
								win->WinIconifyRedraw( win, app);
							else
								win->WinRedraw( win, app);
							break;
						case WM_TOPPED:
							win->WinTop( win, app);
							break;
						case WM_BOTTOMED:
							win->WinBottom( win, app);
							break;	
						case WM_SIZED:
							win->WinSize( win, app);
							break;	
						case WM_ICONIFY:
							win->WinIconify( win, app);
							break;
						case WM_UNICONIFY:
							win->WinUniconify( win, app);
							break;
						case WM_ALLICONIFY:
							win->WinAlliconify( win, app);
							break;
						case WM_MOVED:
							win->WinMove( win, app);
							break;
						case WM_FULLED:
							win->WinFull( win, app);
							break;
						case WM_CLOSED:
							win->WinClose( win, app);
							break;
						case WM_ARROWED:
							win->WinArrow( win, app);
							break;
						case WM_VSLID:
							win->WinVslide( win, app);
							break;
						default:
							break;
					}
				}
			}
		}

		/* handle mouse button events */
		if( evnt_res & MU_BUTTON)
		{
			/* window under the mouse pointer */
			win = WindHandle( app, mt_wind_find( app->mx, app->my, app->aes_global));
	
			if( win)
			{
				if( win->status & WS_ICONIFY)
				{
					if(( app->mbut & LEFT_BUTTON) && ( app->nb_click == 2))
					{
						mt_wind_get_grect( win->handle, WF_UNICONIFY, ( GRECT*)&app->aes_buff[4], app->aes_global);
						win->WinUniconify( win, app);
					}
				}
				else if((( win->status & WS_FORM) && ( app->mbut & LEFT_BUTTON)) || !( win->status & WS_FORM))
					win->WinMouse( win, app);
			}
		}

		/* handle keyboard events */
		if( evnt_res & MU_KEYBD)
		{
			mt_wind_get( 0, WF_TOP, &top, &dum, &dum, &dum, app->aes_global);
			win = WindHandle( app, top);
			 
			if( win) /* && !menu_exec_cmd( app) */
				win->WinKeyboard( win, app);
		}
		
		/* handle timer events */
		if( evnt_res & MU_TIMER)
		{
			static int16 current_mouse = ARROW;

			int16 new_mouse_form = suitable_mouse_form( app);
				
			if( new_mouse_form != current_mouse)
			{
				mt_graf_mouse( new_mouse_form, NULL, app->aes_global);
				current_mouse = new_mouse_form;
			}
		}
	}

	ApplExit( app);
}


