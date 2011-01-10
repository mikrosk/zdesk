#include "general.h"
#include "ztext.h"
#include "edit.h"
#include "winfile/WinFile.h"
#include "icon/icon.h"
#include "misc.h"



inline void draw_column_title( WINDOW *win, GRECT *column_title_area, GRECT *clip, APPVar *app)
{
	int16 pxy[6], txt_x, txt_y, background, top_line, lower_line, title_shadow, title_color;
	TAB *tab = win->selected_tab;
	
	txt_y = column_title_area->g_y + (( COLUMN_TITLE_HEIGHT - app->hcell + 1) >> 1);
	
	pxy[0] = MAX( column_title_area->g_x + 1, clip->g_x);	
	pxy[1] = clip->g_y;
	pxy[2] = MIN( column_title_area->g_x + tab->name_column_width - 2, clip->g_x + clip->g_w - 1);
	pxy[3] = clip->g_y + clip->g_h - 1;
	
	if( pxy[0] <= pxy[2])
	{
		if( tab->sortingmode == MENU_BAR_BY_NAME)
		{
			background	= ( int16)app->pref.button_on_background;		
			top_line	= ( int16)app->pref.button_on_dark_color;				
			lower_line	= ( int16)app->pref.button_on_light_color;
			title_color = ( int16)app->pref.button_on_text_color;
			title_shadow = ( int16)app->pref.button_on_text_shadow_color;
		}
		else
		{
			background	= ( int16)app->pref.button_off_background;		
			top_line	= ( int16)app->pref.button_off_light_color;
			lower_line	= ( int16)app->pref.button_off_dark_color;
			title_color = ( int16)app->pref.button_off_text_color;
			title_shadow = ( int16)app->pref.button_off_text_shadow_color;				
		}
		
		vsf_color( app->vdi_handle, background);
		vr_recfl( app->vdi_handle, pxy);

		pxy[0] = column_title_area->g_x;
		pxy[3] = column_title_area->g_y;
		pxy[1] = pxy[3] + column_title_area->g_h - 1;
		pxy[2] = pxy[0];
		pxy[4] = pxy[0] + tab->name_column_width - 1;
		pxy[5] = pxy[3];
		
		vsl_color( app->vdi_handle, top_line);
		v_pline( app->vdi_handle, 3, pxy);

		pxy[0] = pxy[4];
		pxy[4] = pxy[2];		
		pxy[3] = pxy[1];
		pxy[1] = pxy[5];
		pxy[2] = pxy[0];
		pxy[5] = pxy[3];		

		vsl_color( app->vdi_handle, lower_line);
		v_pline( app->vdi_handle, 3, pxy);		
		

		draw_text( column_title_area->g_x + 5, txt_y, -1, title_color, ( app->pref.object_text_is_3D) ? title_shadow : -1, ( const char *)app->name_title, app);
	}

	if( tab->size_column_width)
	{
		pxy[0] = MAX( column_title_area->g_x + tab->name_column_width + 1, clip->g_x);
		pxy[2] = MIN( column_title_area->g_x + tab->name_column_width + tab->size_column_width - 2, clip->g_x + clip->g_w - 1);	
		
		if( pxy[0] <= pxy[2])
		{
			if( tab->sortingmode == MENU_BAR_BY_SIZE)
			{
				background	= ( int16)app->pref.button_on_background;		
				top_line	= ( int16)app->pref.button_on_dark_color;				
				lower_line	= ( int16)app->pref.button_on_light_color;
				title_color = ( int16)app->pref.button_on_text_color;
				title_shadow = ( int16)app->pref.button_on_text_shadow_color;
			}
			else
			{
				background	= ( int16)app->pref.button_off_background;		
				top_line	= ( int16)app->pref.button_off_light_color;
				lower_line	= ( int16)app->pref.button_off_dark_color;
				title_color = ( int16)app->pref.button_off_text_color;
				title_shadow = ( int16)app->pref.button_off_text_shadow_color;				
			}
			
			vsf_color( app->vdi_handle, background);
			vr_recfl( app->vdi_handle, pxy);
			
			pxy[0] = column_title_area->g_x + tab->name_column_width;
			pxy[3] = column_title_area->g_y;
			pxy[1] = pxy[3] + column_title_area->g_h - 1;
			pxy[2] = pxy[0];
			pxy[4] = pxy[0] + tab->size_column_width - 1;
			pxy[5] = pxy[3];
			
			vsl_color( app->vdi_handle, top_line);
			v_pline( app->vdi_handle, 3, pxy);
			
			pxy[0] = pxy[4];
			pxy[4] = pxy[2];		
			pxy[3] = pxy[1];
			pxy[1] = pxy[5];
			pxy[2] = pxy[0];
			pxy[5] = pxy[3];		
			
			vsl_color( app->vdi_handle, lower_line);
			v_pline( app->vdi_handle, 3, pxy);		
			
			txt_x = column_title_area->g_x + tab->name_column_width + tab->size_column_width - app->size_title_txt_w - 5;
			
			draw_text( txt_x, txt_y, -1, title_color, ( app->pref.object_text_is_3D) ? title_shadow : -1, ( const char *)app->size_title, app);
		}
	}	
	
	if( tab->date_column_width)
	{
		pxy[0] = MAX( column_title_area->g_x + tab->name_column_width + tab->size_column_width + 1, clip->g_x);
		pxy[2] = MIN( column_title_area->g_x + tab->name_column_width + tab->size_column_width + tab->date_column_width - 2, clip->g_x + clip->g_w - 1);		
		
		if( pxy[0] <= pxy[2])
		{
			if( tab->sortingmode == MENU_BAR_BY_DATE)
			{
				background	= ( int16)app->pref.button_on_background;		
				top_line	= ( int16)app->pref.button_on_dark_color;				
				lower_line	= ( int16)app->pref.button_on_light_color;
				title_color = ( int16)app->pref.button_on_text_color;
				title_shadow = ( int16)app->pref.button_on_text_shadow_color;
			}
			else
			{
				background	= ( int16)app->pref.button_off_background;		
				top_line	= ( int16)app->pref.button_off_light_color;
				lower_line	= ( int16)app->pref.button_off_dark_color;
				title_color = ( int16)app->pref.button_off_text_color;
				title_shadow = ( int16)app->pref.button_off_text_shadow_color;				
			}
			
			vsf_color( app->vdi_handle, background);
			vr_recfl( app->vdi_handle, pxy);
			
			pxy[0] = column_title_area->g_x + tab->name_column_width + tab->size_column_width;
			pxy[3] = column_title_area->g_y;
			pxy[1] = pxy[3] + column_title_area->g_h - 1;
			pxy[2] = pxy[0];
			pxy[4] = pxy[0] + tab->date_column_width - 1;
			pxy[5] = pxy[3];
			
			vsl_color( app->vdi_handle, top_line);
			v_pline( app->vdi_handle, 3, pxy);
			
			pxy[0] = pxy[4];
			pxy[4] = pxy[2];		
			pxy[3] = pxy[1];
			pxy[1] = pxy[5];
			pxy[2] = pxy[0];
			pxy[5] = pxy[3];		
			
			vsl_color( app->vdi_handle, lower_line);
			v_pline( app->vdi_handle, 3, pxy);		
			
			txt_x = column_title_area->g_x + tab->name_column_width + tab->size_column_width + tab->date_column_width - app->date_title_txt_w - 5;
			
			draw_text( txt_x, txt_y, -1, title_color, ( app->pref.object_text_is_3D) ? title_shadow : -1, ( const char *)app->date_title, app);
		}
	}	
	
	pxy[0] = MAX( column_title_area->g_x + tab->name_column_width + tab->size_column_width + tab->date_column_width + 1, clip->g_x);	
	pxy[2] = clip->g_x + clip->g_w - 1;
	
	if( pxy[0] <= pxy[2])
	{
		vsf_color( app->vdi_handle, ( int16)app->pref.button_off_background);
		vr_recfl( app->vdi_handle, pxy);
		
		pxy[0] = column_title_area->g_x + tab->name_column_width + tab->size_column_width + tab->date_column_width;
		pxy[3] = column_title_area->g_y;
		pxy[1] = pxy[3] + column_title_area->g_h - 1;
		pxy[2] = pxy[0];
		pxy[4] = clip->g_x + clip->g_w - 1;
		pxy[5] = pxy[3];
		
		vsl_color( app->vdi_handle, ( int16)app->pref.button_off_light_color);
		v_pline( app->vdi_handle, 3, pxy);
		
		pxy[2] = pxy[4];				
		pxy[3] = pxy[1];		
		
		vsl_color( app->vdi_handle, ( int16)app->pref.button_off_dark_color);
		v_pline( app->vdi_handle, 2, pxy);
	}
}



inline void draw_background( WINDOW *win, GRECT *win_rect, GRECT *clip, APPVar *app)
{
	int16 pxy[4];	
	TAB *tab = win->selected_tab;
	
	pxy[1] = clip->g_y;
	pxy[3] = pxy[1] + clip->g_h - 1;	
	
	if( tab->view_mode == LIST_MODE)
	{
		pxy[0] = MAX( win_rect->g_x, clip->g_x);

		if( tab->sortingmode == MENU_BAR_BY_NAME)
		{
			pxy[2] = MIN( win_rect->g_x + tab->name_column_width, clip->g_x + clip->g_w) - 1;

			if( pxy[0] <= pxy[2])
			{
				vsf_color( app->vdi_handle, ( int16)app->pref.selected_column_color);
				vr_recfl( app->vdi_handle, pxy);
			}

			pxy[0] = MAX( win_rect->g_x + tab->name_column_width, clip->g_x);
			pxy[2] = clip->g_x + clip->g_w - 1;

			if( pxy[0] <= pxy[2])
			{
				vsf_color( app->vdi_handle, ( int16)app->pref.win_background_color);
				vr_recfl( app->vdi_handle, pxy);
			}
		}
		else if((( tab->sortingmode == MENU_BAR_BY_SIZE) && !tab->size_column_width) ||
		   (( tab->sortingmode == MENU_BAR_BY_DATE) && !tab->date_column_width))
		{
			pxy[2] = clip->g_x + clip->g_w - 1;
			
			vsf_color( app->vdi_handle, ( int16)app->pref.win_background_color);
			vr_recfl( app->vdi_handle, pxy);
		}			
		else if( tab->sortingmode == MENU_BAR_BY_SIZE)
		{
			pxy[2] = MIN( win_rect->g_x + tab->name_column_width, clip->g_x + clip->g_w) - 1;
			
			if( pxy[0] <= pxy[2])
			{
				vsf_color( app->vdi_handle, ( int16)app->pref.win_background_color);
				vr_recfl( app->vdi_handle, pxy);
			}			
			
			pxy[0] = MAX( win_rect->g_x + tab->name_column_width, clip->g_x);
			pxy[2] = MIN( win_rect->g_x + tab->name_column_width + tab->size_column_width, clip->g_x + clip->g_w) - 1;
			
			if( pxy[0] <= pxy[2])
			{
				vsf_color( app->vdi_handle, ( int16)app->pref.selected_column_color);
				vr_recfl( app->vdi_handle, pxy);
			}
			
			pxy[0] = MAX( win_rect->g_x + tab->name_column_width + tab->size_column_width, clip->g_x);
			pxy[2] = clip->g_x + clip->g_w - 1;
			
			if( pxy[0] <= pxy[2])
			{
				vsf_color( app->vdi_handle, ( int16)app->pref.win_background_color);
				vr_recfl( app->vdi_handle, pxy);
			}				
			
		}
		else if( tab->sortingmode == MENU_BAR_BY_DATE)
		{
			pxy[2] = MIN( win_rect->g_x + tab->name_column_width + tab->size_column_width, clip->g_x + clip->g_w) - 1;
			
			if( pxy[0] <= pxy[2])
			{
				vsf_color( app->vdi_handle, ( int16)app->pref.win_background_color);
				vr_recfl( app->vdi_handle, pxy);
			}			
			
			pxy[0] = MAX( win_rect->g_x + tab->name_column_width + tab->size_column_width, clip->g_x);
			pxy[2] = MIN( win_rect->g_x + tab->name_column_width + tab->size_column_width + tab->date_column_width, clip->g_x + clip->g_w) - 1;
			
			if( pxy[0] <= pxy[2])
			{
				vsf_color( app->vdi_handle, ( int16)app->pref.selected_column_color);
				vr_recfl( app->vdi_handle, pxy);
			}
			
			pxy[0] = MAX( win_rect->g_x + tab->name_column_width + tab->size_column_width + tab->date_column_width, clip->g_x);
			pxy[2] = clip->g_x + clip->g_w - 1;
			
			if( pxy[0] <= pxy[2])
			{
				vsf_color( app->vdi_handle, ( int16)app->pref.win_background_color);
				vr_recfl( app->vdi_handle, pxy);
			}				
		}
	}
	else
	{
		pxy[0] = clip->g_x;
		pxy[2] = pxy[0] + clip->g_w - 1;
		
		vsf_color( app->vdi_handle, ( int16)app->pref.win_background_color);
		vr_recfl( app->vdi_handle, pxy);
	}
}


/*==================================================================================*
 * WinFile_Redraw:																	*
 *		WM_REDRAW event for the WINDOW *win.										*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void WinFile_Redraw( WINDOW *win, APPVar *app)
{
	int16 i, pxy[10], txt_color;
	TAB *tab = win->selected_tab;	
	GRECT rdraw = *( GRECT*)&app->aes_buff[4], rlist, rlist1, full_area, file_area, infobar_area, tabbar_area, column_title_area;

	if( !rc_intersect(( GRECT*)&app->x, &rdraw))
		return;	
	
	WinFile_Calc_Areas( win, &full_area, &file_area, app);

	if( !rc_intersect( &full_area, &rdraw))
		return;

	column_title_area.g_x = file_area.g_x;
	column_title_area.g_y = file_area.g_y - COLUMN_TITLE_HEIGHT;
	column_title_area.g_w = file_area.g_w;
	column_title_area.g_h = COLUMN_TITLE_HEIGHT;

	infobar_area.g_x = full_area.g_x;		
	infobar_area.g_y = full_area.g_y + full_area.g_h - INFOBAR_HEIGHT;
	infobar_area.g_w = full_area.g_w;
	infobar_area.g_h = INFOBAR_HEIGHT;	

	tabbar_area.g_x = file_area.g_x;		
	tabbar_area.g_y = file_area.g_y + file_area.g_h;
	tabbar_area.g_w = file_area.g_w;
	tabbar_area.g_h = TABBAR_HEIGHT;	
	
	
	WinFile_Update_Icon_Position( win, &file_area, app);

	while( !mt_wind_update( BEG_UPDATE, app->aes_global));

	mt_wind_get_grect( win->handle, WF_FIRSTXYWH, &rlist1, app->aes_global);
	
	v_hide_c( app->vdi_handle);

	while( rlist1.g_w && rlist1.g_h)
	{
		if( rc_intersect( &rdraw, &rlist1))
		{
			rlist = rlist1;
			
			if(( tab->view_mode == LIST_MODE) && rc_intersect( &column_title_area, &rlist))
			{
				ClipOn( app, &rlist);
				draw_column_title( win, &column_title_area, &rlist, app);
			}
			
			rlist = rlist1;
			
			if( rc_intersect( &file_area, &rlist))
			{
				ClipOn( app, &rlist);

				draw_background( win, &file_area, &rlist, app);
				
				vsf_color( app->vdi_handle, ( int16)app->pref.win_highlight_color);

				if( tab->view_mode == LIST_MODE)
				{
					vsl_type( app->vdi_handle, ( int16)app->pref.file_separator_style);
					vsl_color( app->vdi_handle, ( int16)app->pref.file_separator_color);
					vsf_interior(app->vdi_handle, ( int16)app->pref.win_highlight_style);
					vswr_mode(app->vdi_handle, MD_TRANS);
				}
				
				for( i = 0; i < tab->nbr_icons; i++)
				{
					GRECT entry_pos = tab->entry[i].box_pos;

					if(( tab->entry[i].status & ES_HIDDEN) && !app->pref.show_hidden)
						continue;
					
					entry_pos.g_x += file_area.g_x;
					entry_pos.g_y += file_area.g_y;

					
					if( rc_intersect( &rlist, &entry_pos))
					{	
						if( tab->view_mode == LIST_MODE)
						{
							pxy[0] = entry_pos.g_x;
							pxy[2] = entry_pos.g_x + entry_pos.g_w - 1;
							
							if( !check_selected_entry( tab, &tab->entry[i]))
							{
								txt_color = ( int16)app->pref.win_txt_color;
								draw_icon( win, &tab->entry[i], FALSE, file_area.g_x + tab->entry[i].icn_pos.g_x, tab->entry[i].icn_pos.g_y + file_area.g_y, app);
								draw_text( tab->entry[i].txt_pos.g_x + file_area.g_x, tab->entry[i].txt_pos.g_y + file_area.g_y, tab->entry[i].txt_pos.g_w, txt_color, -1, ( const char *)tab->entry[i].name_shown, app);
							}
							else 
							{
								txt_color = ( int16)app->pref.win_txt_highlight_color;
								
								pxy[1] = file_area.g_y + tab->entry[i].box_pos.g_y;
								pxy[3] = pxy[1] + tab->entry[i].box_pos.g_h - 2;	
								
								vr_recfl( app->vdi_handle, pxy);
								
								draw_icon( win, &tab->entry[i], TRUE, file_area.g_x + tab->entry[i].icn_pos.g_x, tab->entry[i].icn_pos.g_y + file_area.g_y, app);
								
								if( win->edit.edition_mode == FALSE)
									draw_text( tab->entry[i].txt_pos.g_x + file_area.g_x, tab->entry[i].txt_pos.g_y + file_area.g_y, tab->entry[i].txt_pos.g_w, txt_color, -1, ( const char *)tab->entry[i].name_shown, app);
							}

							if( tab->size_column_width)
							{	
								if( tab->entry[i].status & ES_DIR)
									draw_text( file_area.g_x + tab->name_column_width + tab->size_column_width - 17, tab->entry[i].txt_pos.g_y + file_area.g_y, -1, txt_color, -1, "--", app);
								else
									draw_text( file_area.g_x + tab->name_column_width + tab->size_column_width - tab->entry[i].tsize_txt_w - 5, tab->entry[i].txt_pos.g_y + file_area.g_y, -1, txt_color, -1, ( const char *)tab->entry[i].tsize, app);
							}		
							if( tab->date_column_width)
								draw_text( file_area.g_x + tab->name_column_width + tab->size_column_width + tab->date_column_width - tab->entry[i].tdate_txt_w - 5, tab->entry[i].txt_pos.g_y + file_area.g_y, -1, txt_color, -1, ( const char *)tab->entry[i].tdate, app);
							
							pxy[1] = file_area.g_y + tab->entry[i].box_pos.g_y + tab->entry[i].box_pos.g_h - 1;
							pxy[3] = pxy[1];
							
							v_pline( app->vdi_handle, 2, pxy);
						}	
						else
						{
							if( !check_selected_entry( tab, &tab->entry[i]))
							{
								draw_icon( win, &tab->entry[i], FALSE, file_area.g_x + tab->entry[i].icn_pos.g_x, tab->entry[i].icn_pos.g_y + file_area.g_y, app);
								draw_text( tab->entry[i].txt_pos.g_x + file_area.g_x, tab->entry[i].txt_pos.g_y + file_area.g_y, tab->entry[i].txt_pos.g_w, ( int16)app->pref.win_txt_color, -1, ( const char *)tab->entry[i].name_shown, app);
							}						
							else
							{
								draw_icon( win, &tab->entry[i], TRUE, file_area.g_x + tab->entry[i].icn_pos.g_x, tab->entry[i].icn_pos.g_y + file_area.g_y, app);
								
								if( win->edit.edition_mode == FALSE)
								{
									pxy[0] = tab->entry[i].txt_pos.g_x + file_area.g_x - 1;
									pxy[1] = tab->entry[i].txt_pos.g_y + file_area.g_y - 2;
									pxy[2] = tab->entry[i].txt_pos.g_x + tab->entry[i].txt_pos.g_w + file_area.g_x + 2;
									pxy[3] = tab->entry[i].txt_pos.g_y + tab->entry[i].txt_pos.g_h + file_area.g_y + 2;	
									
									if( pxy[0] <= pxy[2])
										vr_recfl( app->vdi_handle, pxy);

									draw_text( tab->entry[i].txt_pos.g_x + file_area.g_x, tab->entry[i].txt_pos.g_y + file_area.g_y, tab->entry[i].txt_pos.g_w, ( int16)app->pref.win_txt_highlight_color, -1, ( const char *)tab->entry[i].name_shown, app);
								}
							}
						}
					}
				}

				if( tab->view_mode == LIST_MODE)
				{	
					vsl_type( app->vdi_handle, SOLID);
					vsf_interior(app->vdi_handle, FIS_SOLID);
					vswr_mode(app->vdi_handle, MD_REPLACE);
				}	

				if( win->edit.edition_mode == TRUE)
				{
					int16 width = MAX( tab->first_selected->txt_pos.g_w, 16);
					int16 xtext = tab->first_selected->txt_pos.g_x + file_area.g_x;
					int16 ytext = tab->first_selected->txt_pos.g_y + file_area.g_y;
			
					pxy[0] = ( tab->view_mode == ICON_MODE) ? ( tab->first_selected->box_pos.g_x + (( tab->w_u - width + 1) >> 1) + file_area.g_x) : xtext;
					pxy[0] -= 2;
					pxy[1] = ytext - 3;
					pxy[2] = width + 4;
					pxy[3] = tab->first_selected->txt_pos.g_h + 5;
	
					if( rc_intersect(( GRECT*)pxy, &rlist))
					{	
						pxy[2] += pxy[0];
						pxy[3] += pxy[1];		
					   	vsf_color( app->vdi_handle, WHITE);
						vr_recfl( app->vdi_handle, pxy);
				
						pxy[6] = pxy[4] = pxy[2];
						pxy[5] = pxy[3];
						pxy[2] = pxy[8] = pxy[0];
						pxy[7] = pxy[9] = pxy[1];

						vsl_color( app->vdi_handle, BLACK);
						v_pline( app->vdi_handle, 5, pxy);
				
						if( tab->first_selected->txt_pos.g_w > 0)
							draw_text( xtext, ytext, tab->first_selected->txt_pos.g_w, BLACK, -1, ( const char *)win->edit.buffer, app);
				
						pxy[0] = xtext + cursor_position( win);
						pxy[2] = pxy[0];

						v_pline( app->vdi_handle, 2, pxy);
					}
				}
			}
			
			rlist = rlist1;

			if(( win->status & WS_TABBAR) && rc_intersect( &tabbar_area, &rlist))
			{
				ClipOn( app, &rlist);
				draw_tabbar( win, &tabbar_area, &rlist, app);
			}

			if(( win->status & WS_INFOBAR) && rc_intersect( &infobar_area, &rlist1))
			{
				ClipOn( app, &rlist1);
				draw_infobar( win, &infobar_area, &rlist1, app);
			}
		}

		mt_wind_get_grect( win->handle, WF_NEXTXYWH, &rlist1, app->aes_global);
	}

	ClipOn( app, ( GRECT*)&app->x);
	v_show_c( app->vdi_handle, 1);
	vsl_color( app->vdi_handle, BLACK);
	mt_wind_update( END_UPDATE, app->aes_global);
}

