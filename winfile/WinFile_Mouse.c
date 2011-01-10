#include "general.h"
#include "ztext.h"
#include "menu.h"
#include "misc.h"
#include "edit.h"
#include "winfile/WinFile.h"
#include "file/sort.h"


/*==================================================================================*
 * void WinFile_Mouse:																*
 *		Handle the mouse event														*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win			-> The window to handle.										*
 * 		app			-> Application's descriptor.									*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Mouse( WINDOW *win, APPVar *app)
{
	TAB *tab = win->selected_tab;	
	GRECT	win_area, file_area;
	int16	mousex, mousey, already_selected = FALSE, i = 0, in_txt = FALSE, in_icn = FALSE, in_tab = FALSE, in_file_area = FALSE;
	Entry	*entry_ptr = NULL;

	WinFile_Calc_Areas( win, &win_area, &file_area, app);

	mousex = app->mx - file_area.g_x;
	mousey = app->my - file_area.g_y;	

	
	/* The mouse click is over the column title */
	if(( tab->view_mode == LIST_MODE) && IS_IN( mousex, mousey + COLUMN_TITLE_HEIGHT, 0, 0, file_area.g_w, COLUMN_TITLE_HEIGHT))
	{
		int16 new_sortingmode = tab->sortingmode;

		mousey += COLUMN_TITLE_HEIGHT;
		
		if( IS_IN( mousex, mousey, 0, 0, tab->name_column_width - 3, COLUMN_TITLE_HEIGHT))
		{
			new_sortingmode = MENU_BAR_BY_NAME;
		}
		else if( tab->size_column_width && IS_IN( mousex, mousey, tab->name_column_width + 3, 0, tab->size_column_width - 6, COLUMN_TITLE_HEIGHT))
		{
			new_sortingmode = MENU_BAR_BY_SIZE;
		}	
		else if( tab->date_column_width && IS_IN( mousex, mousey, tab->name_column_width + tab->size_column_width + 3, 0, tab->date_column_width - 6, COLUMN_TITLE_HEIGHT))
		{
			new_sortingmode = MENU_BAR_BY_DATE;
		}				
		
		if( new_sortingmode != tab->sortingmode)
		{
			win->edit.edition_mode = FALSE;
			
			if( tab->first_selected)
			{
				while( tab->first_selected)
					remove_selected_entry( tab, tab->first_selected);
				
				update_infobar( win, &win_area, app);
			}
			
			tab->sortingmode = new_sortingmode;
			sort_entries( tab);
			
			file_area.g_y -= COLUMN_TITLE_HEIGHT;
			file_area.g_h += COLUMN_TITLE_HEIGHT;	
			
			*(GRECT*)&app->aes_buff[4] = file_area;
			win->WinRedraw( win, app);
		}
		else
		{
			if( win->edit.edition_mode == TRUE)
				exit_edit_mode( win, tab->first_selected, file_area.g_x, file_area.g_y, app);
			
			file_area.g_y -= COLUMN_TITLE_HEIGHT;
			file_area.g_h += COLUMN_TITLE_HEIGHT;	
			
			WinFile_Column_Sizer( win, mousex, mousey, &file_area, app);	
		}
	}
	else if( IS_IN( mousex, mousey, 0, 0, file_area.g_w, file_area.g_h))
	{
		in_file_area = TRUE;
	
		for( i = 0; i < tab->nbr_icons; i++)
		{
			if( IS_IN( mousex, mousey, tab->entry[i].txt_pos.g_x, tab->entry[i].txt_pos.g_y, tab->entry[i].txt_pos.g_w, tab->entry[i].txt_pos.g_h))
			{
				in_txt = TRUE;
				break;
			}
			else
			{
				GRECT *icon_pos = ( tab->view_mode == ICON_MODE) ? &tab->entry[i].icn_pos : &tab->entry[i].box_pos;
				
				if( IS_IN( mousex, mousey, icon_pos->g_x, icon_pos->g_y, icon_pos->g_w, icon_pos->g_h))
				{
					in_icn = TRUE;
					break;
				}
			}
		}
	}
	else if(( win->status & WS_TABBAR) && IS_IN( mousex, mousey, 0, file_area.g_h, file_area.g_w, TABBAR_HEIGHT))
	{	
		if(( win->status & WS_TABHIDDEN) && IS_IN( mousex, mousey, file_area.g_w - 17, file_area.g_h, 16, TABBAR_HEIGHT))
			open_tab_popup( win, file_area.g_x + file_area.g_w - 19, file_area.g_y + file_area.g_h + TABBAR_HEIGHT - 2, app);
		else
		{
			for( tab = win->tab; tab; tab = tab->next)
			{
				if( tab->tab_xpos == -1)
					continue;
				
				if( IS_IN( mousex, mousey, tab->tab_xpos, file_area.g_h, tab->tab_width, TABBAR_HEIGHT))
				{
					if(( app->nb_click == 2) && ( app->pref.tab_options & TAB_CLOSE_WITH_DOUBLE_CLICK) && win->tab->next)
						WinFile_Close_Tab( win, tab, app);
					else if( tab != win->selected_tab)
					{
						if( tab->tab_xpos + tab->tab_width >= file_area.g_w)
							win->status |= WS_LAST_TAB_IS_SELECTED;
						else
							win->status &= ~WS_LAST_TAB_IS_SELECTED;
						
						WinFile_Select_Tab( win, tab, app);
					}

					in_tab = TRUE;
				
					break;
				}
			}

			if(( in_tab == FALSE) && ( app->nb_click == 2))
				open_new_tab( win, app);
		}
	}


	if(( app->mkstate & ( K_LSHIFT|K_RSHIFT)) && in_file_area)
	{
		if( win->edit.edition_mode == TRUE)
			exit_edit_mode( win, tab->first_selected, file_area.g_x, file_area.g_y, app);

		if( in_icn || in_txt)
		{
			if( !check_selected_entry( tab, &tab->entry[i]))
				add_selected_entry( tab, &tab->entry[i]);
			else
				remove_selected_entry( tab, &tab->entry[i]);

			redraw_area( win, file_area.g_x, file_area.g_y, &tab->entry[i].box_pos, app);

			update_infobar( win, &win_area, app);
		}
	}
	else if( in_txt)
	{
		if(( win->edit.edition_mode == TRUE) && ( tab->first_selected == &tab->entry[i]))
			return;

		mt_graf_mkstate( &app->mx, &app->my, &app->mbut, &app->mkstate, app->aes_global);
		
		if(( app->nb_click == 1) && ( win->edit.edition_mode == FALSE) && ( tab->first_selected == &tab->entry[i]) && ( tab->first_selected->icon != ICON_BACK) && ( !tab->first_selected->next_selected) && ( app->mbut == 0))
			init_edit_mode( win, tab->first_selected, file_area.g_x, file_area.g_y, app);
		else
			goto on_icon;
	}
	else if( in_icn)
	{
	
	on_icon:

		if( win->edit.edition_mode == TRUE)
			exit_edit_mode( win, tab->first_selected, file_area.g_x, file_area.g_y, app);

		if( tab->first_selected)
		{
			while( tab->first_selected)
			{
				entry_ptr = tab->first_selected;
				remove_selected_entry( tab, tab->first_selected);

				if( entry_ptr == &tab->entry[i])
					already_selected = 1;
				else
					redraw_area( win, file_area.g_x, file_area.g_y, &entry_ptr->box_pos, app);
			}
		}

		tab->first_selected = &tab->entry[i];

		if( !already_selected)
			redraw_area( win, file_area.g_x, file_area.g_y, &tab->first_selected->box_pos, app);

		update_infobar( win, &win_area, app);

		if( app->nb_click == 2 && app->mbut == 1)
		{
			WinFile_Exec_Entry( win, &win_area, &file_area, app);
		}
		else if( tab->view_mode == LIST_MODE)
			in_icn = IS_IN( mousex, mousey, tab->entry[i].icn_pos.g_x, tab->entry[i].box_pos.g_y, tab->entry[i].txt_pos.g_x + tab->entry[i].txt_pos.g_w, tab->entry[i].box_pos.g_h);
	}
	else if( in_file_area)
	{
		if( tab->first_selected)
		{
			if( win->edit.edition_mode == TRUE)
				exit_edit_mode( win, tab->first_selected, file_area.g_x, file_area.g_y, app);
			
			while( tab->first_selected)
			{
				entry_ptr = tab->first_selected;
				remove_selected_entry( tab, tab->first_selected);
				redraw_area( win, file_area.g_x, file_area.g_y, &entry_ptr->box_pos, app);
			}

			update_infobar( win, &win_area, app);
		}
	}

	/* a wait loop while the mouse button is pressed */
	while( app->mbut)
	{	
		mt_graf_mkstate( &mousex, &mousey, &app->mbut, &app->mkstate, app->aes_global);
		
		if( app->mbut != 1)
			continue;
		
		if(( in_tab == TRUE) && win->tab->next && (( mousex != app->mx) || ( mousey != app->my)))
		{
			WinFile_Tab_Drag_and_Drop( win, tab, mousex, mousey, &file_area, app);
			break;
		}
		else if( in_file_area == TRUE && in_icn == FALSE && in_txt == FALSE)
		{	
			WinFile_Rubberbox( win, &file_area, &win_area, app);
			break;
		}	
	}	
}


