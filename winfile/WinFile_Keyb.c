#include "general.h"
#include "misc.h"
#include "rename_entry.h"
#include "ztext.h"
#include "edit.h"
#include "winfile/WinFile.h"



/*==================================================================================*
 * void WinFile_Keyb:																*
 *		Handle the keyboard event.													*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win			-> The window to handle.										*
 * 		app			-> Application's descriptor.									*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Keyb( WINDOW *win, APPVar *app)
{
	GRECT win_area, file_area;
	TAB *tab = win->selected_tab;
	
	if( tab->nbr_icons == 0)
		return;

	WinFile_Calc_Areas( win, &win_area, &file_area, app);

	/* Several files selected ? If a navigation key is pressed( UP, DOWN, LEFT or RIGHT Key), we clear the selection list and we quit the function */
	if( tab->first_selected && tab->first_selected->next_selected) 
	{	
		if((( app->keybd >> 8) == SC_UPARW) || (( app->keybd >> 8) == SC_DNARW) || (( app->keybd >> 8) == SC_LFARW) || (( app->keybd >> 8) == SC_RTARW))
		{
			Entry *entry_ptr;
			
			while( tab->first_selected->next_selected)
			{
				entry_ptr = tab->first_selected->next_selected;
				remove_selected_entry( tab, tab->first_selected->next_selected);
				redraw_area( win, file_area.g_x, file_area.g_y, &entry_ptr->box_pos, app);
			}
			
			update_infobar( win, &win_area, app);
		}
	}
	else if( win->edit.edition_mode == TRUE)
	{
		static char allowed_char[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZÇäàâáÄê°çãåóñ£ÅÖ†ÉÑÜëÅî§èí•0123456789 &\"'(›!)-_`ú^[]*{},?;.=+%#|@~<>";
		int16 c = app->keybd & 0x00FF;
		int32 max_len = Dpathconf( tab->directory, 3);
		
		switch( app->keybd >> 8)
		{
			case SC_RETURN:
			case SC_ENTER:
				rename_entry( win, &file_area, app);
				break;

			case SC_LFARW:
				if( curs_left( &win->edit))
					redraw_icon_txt( win, tab->first_selected, file_area.g_x, file_area.g_y, app);
				break;

			case SC_RTARW:
				if( curs_right( &win->edit))
					redraw_icon_txt( win, tab->first_selected, file_area.g_x, file_area.g_y, app);
				break;

			case SC_UPARW:
			case SC_DWARW:
			case SC_ESC:
				exit_edit_mode( win, tab->first_selected, file_area.g_x, file_area.g_y, app);
				break;

			case SC_DEL:
				if( curs_right( &win->edit) == 0)
					break;
			case SC_BACK:
				if( char_del( &win->edit) != 0)
					redraw_icon_txt( win, tab->first_selected, file_area.g_x, file_area.g_y, app);
				break; 

			default:
				if( win->edit.len < max_len && ( win->edit.len < MAXNAMLEN - 1) && strchr( allowed_char, c)
				   && ( win->edit.len < 126) /* work around to an Aranym bug( or the aranym.sys module):
											  even if Dpathconf() returns that the OS support up to 256 characters,
											  if we rename a file with a name of more than 127 characters,
											  strange things happen:
											  1) The name is truncated to 127 characters.
											  2) We can rename the file again. */
				   )
				{
					if( char_put( &win->edit, c))
					   redraw_icon_txt( win, tab->first_selected, file_area.g_x, file_area.g_y, app);
				}
				break; 
		}
	}
	else
	{
		int16 res, selection;

		res = selection = return_entry_nbr( tab, tab->first_selected);
		
		switch( app->keybd >> 8)
		{
			case SC_UPARW:
				if( selection == -1)
					selection = 0;
				else if(( tab->view_mode == ICON_MODE) && ( selection >= ( int16)tab->xpos_max))
					selection = selection - ( int16)tab->xpos_max;
				else if(( tab->view_mode == LIST_MODE) && ( selection > 0))
					selection--;		
				
				goto common;
				
			case SC_DWARW:
				if( selection == -1)
					selection = 0;
				else if(( tab->view_mode == ICON_MODE) && ( selection < tab->nbr_icons - tab->icons_last_line && selection + ( int16)tab->xpos_max <  tab->nbr_icons))
					selection = selection + ( int16)tab->xpos_max;
				else if(( tab->view_mode == LIST_MODE) && ( selection < tab->nbr_icons - 1))
					selection++;	
						
				goto common;
				
			case SC_LFARW:
				if( tab->view_mode == LIST_MODE)
					break;

				if( selection == -1)
					selection = 0;
				else if( selection > 0)
					selection--;
				
				goto common;	
				
			case SC_RTARW:
				if( tab->view_mode == LIST_MODE)
					break;						
				if( selection == -1)
					selection = 0;
				else if( selection < tab->nbr_icons - 1)
					selection++;
				
			common:
				
				if( tab->first_selected == &tab->entry[selection])
					return;
				
				tab->first_selected = &tab->entry[selection];
				
				if( tab->first_selected->box_pos.g_y < 0 && win->handle != 0)
				{
					if( tab->first_selected->box_pos.g_y + tab->h_u >= 0)
					{
						if( res >= 0)
							redraw_area( win, file_area.g_x, file_area.g_y, &tab->entry[res].box_pos, app);
						
						app->aes_buff[4] = WA_UPLINE;
						win->WinArrow( win, app);
 					}
					else
					{
						while( tab->first_selected->box_pos.g_y < 0)
						{
							if( tab->ypos == 0)
								break;
							
							tab->first_selected->box_pos.g_y += tab->h_u;
							tab->ypos--;
						}						
						
						*(GRECT*)&app->aes_buff[4] = file_area;
						win->WinRedraw( win, app);
						WinFile_Slider( win, ( int32)file_area.g_h, app);
					}
				}
				else if( tab->first_selected->box_pos.g_y + tab->first_selected->box_pos.g_h > file_area.g_h && win->handle != 0)
				{
					if( tab->first_selected->box_pos.g_y + tab->first_selected->box_pos.g_h - tab->h_u <= file_area.g_h)
					{
						app->aes_buff[4] = WA_DNLINE;
						win->WinArrow( win, app);
						
						if( res >= 0)
							redraw_area( win, file_area.g_x, file_area.g_y, &tab->entry[res].box_pos, app);
						
						redraw_area( win, file_area.g_x, file_area.g_y, &tab->first_selected->box_pos, app);
					}
					else
					{
						while( tab->first_selected->box_pos.g_y + tab->first_selected->box_pos.g_h > file_area.g_h)
						{
							if( tab->ypos == tab->ypos_max)
								break;
							
							tab->first_selected->box_pos.g_y -= tab->h_u;
							tab->ypos++;
						}
						
						*(GRECT*)&app->aes_buff[4] = file_area;
						win->WinRedraw( win, app);
						WinFile_Slider( win, ( int32)file_area.g_h, app);
					}
				}
				else
				{
					if( res >= 0)
						redraw_area( win, file_area.g_x, file_area.g_y, &tab->entry[res].box_pos, app);
					
					redraw_area( win, file_area.g_x, file_area.g_y, &tab->first_selected->box_pos, app);
				}
				
				update_infobar( win, &win_area, app);
				
				break;
				
			case SC_RETURN:
			case SC_ENTER:
				if( !tab->first_selected)
					break;		
				
				WinFile_Exec_Entry( win, &win_area, &file_area, app);
				break;
				
			case SC_F3:
			{
				int16 i, ii, pxy[4], c = 0, char_width, char_height, cell_width, cell_height;
				char character[2];
				
				vst_point( app->vdi_handle, 10, &char_width, &char_height, &cell_width, &cell_height);
				
				pxy[0] = app->x + app->w - ( 32 * cell_width);
				pxy[2] = pxy[0] + ( 32 * cell_width) - 1;
				pxy[1] = app->y + app->h - ( 17 * cell_height);
				pxy[3] = pxy[1] + ( 17 * cell_height) - 1;
				
				vsf_color( app->vdi_handle, WHITE);
				vr_recfl( app->vdi_handle, pxy);
				
				pxy[1] += cell_height;
				
				for( i = 0; i < 8; i++)
				{
					pxy[0] = app->x + app->w - ( 32 * cell_width);
					
					for( ii = 0; ii < 32; ii++)
					{
						sprintf( character, "%c", c++);
						v_gtext( app->vdi_handle, pxy[0], pxy[1], character);
						pxy[0] += cell_width;
					}
					
					pxy[1] += cell_height;
				}
				
				c = 0;
				
				for( i = 0; i < 8; i++)
				{
					pxy[0] = app->x + app->w - ( 32 * cell_width);
						
					for( ii = 0; ii < 32; ii++)
					{
						sprintf( character, "%c", c++);
						zdebug( "%d   %d", character[0], c);
						draw_text( pxy[0], pxy[1], -1, BLACK, -1, character, app);	
						pxy[0] += 8;
					}
					
					pxy[1] += 10;
				}					
			}
				
				break;						

			case SC_F4:
			{
				int16 pxy[4];
				char txt_buffer[1024];
				
				pxy[0] = app->x + app->w - 256;
				pxy[2] = pxy[0] + 255;
				pxy[1] = app->y + 256;
				pxy[3] = pxy[1] + 200;
				
				vsf_color( app->vdi_handle, WHITE);
				vr_recfl( app->vdi_handle, pxy);
				
				sprintf( txt_buffer, "title( %ld): %s ", strlen( tab->name), tab->name);
				draw_text( pxy[0] + 2, pxy[1] + 5, -1, BLACK, -1, ( const char *)txt_buffer, app);
				sprintf( txt_buffer, "directory( %ld): %s ", strlen( tab->directory), tab->directory);
				draw_text( pxy[0] + 2, pxy[1] + 15, -1, BLACK, -1, ( const char *)txt_buffer, app);				
				
			}				

			break;	

			case SC_F5:
			{
				/* if( app->pref.tab_options & TAB_AUTOHIDE)
					zdebug("autohide true");
				else
					zdebug("autohide false");
				*/
				zdebug( "directory=%s   entry=%s   ", tab->directory, ( tab->first_selected == NULL) ? "none" : tab->first_selected->name);

//				int i;		
				
//				zdebug("ok");
				
				// for( i = 0; i < 256; i++)
//					WinFile_New( NULL, "H:\\gif\\", app);
				// WinFile_NewTab( win, NULL, app->pref.default_tab_path, file_area.g_w, app);		
			}							
			break;
			
				
			default:
				break;
	 	}
	}
}

