#include "general.h"
#include "misc.h"
#include "winfile/WinFile.h"
#include "icon/icon.h"


inline int16 rect16_intersect( const RECT16 *r1, GRECT *r2)
{
	int16 x, y, ret;

	x = MAX( r2->g_x + 1, r1->x1);
	y = MIN( r2->g_x + r2->g_w - 1, r1->x2) - x;

	ret = ( 0 <= y);

	if( ret)
	{
		x = MAX( r2->g_y + 1, r1->y1);
		y = MIN( r2->g_y + r2->g_h - 1, r1->y2) - x;

		ret = ( 0 <= y);
	}

	return ret;
}

inline void Draw_Rubberbox( WINDOW *win, GRECT *file_area, RECT16 *write_area, APPVar *app)
{
	GRECT r1;
	int16 xyarray[10];
	int16 *pt = xyarray;

	if( win->selected_tab->view_mode == LIST_MODE)
		return;
		
	*pt++ = write_area->x1;
	*pt++ = write_area->y1;
	*pt++ = write_area->x2;
	*pt++ = xyarray[1];
	*pt++ = xyarray[2];
	*pt++ = write_area->y2;
	*pt++ = xyarray[0];
	*pt++ = xyarray[5];
	*pt++ = xyarray[0];
	*pt++ = xyarray[1];
	
	vsl_type( app->vdi_handle, USERLINE);
	vswr_mode( app->vdi_handle, MD_XOR);
	v_hide_c( app->vdi_handle);

	mt_wind_get_grect( win->handle, WF_FIRSTXYWH, &r1, app->aes_global);
	
	while( r1.g_w && r1.g_h) 
	{
		if( rc_intersect( file_area, &r1))
		{
			ClipOn( app, &r1); 
			v_pline( app->vdi_handle, 5, xyarray);
		}	
		mt_wind_get_grect( win->handle, WF_NEXTXYWH, &r1, app->aes_global);
	}	
	
	v_show_c( app->vdi_handle, 1);
	ClipOn( app, ( GRECT*)&app->x);
	vsl_type( app->vdi_handle, SOLID);
	vswr_mode( app->vdi_handle, MD_REPLACE);
}


void WinFile_Rubberbox( WINDOW *win, GRECT *file_area, GRECT *win_area, APPVar *app)
{
	TAB *tab = win->selected_tab;	
	int32 row = ( int32)( file_area->g_h / tab->h_u);
	RECT16 delete_rect, write_rect;
	int16 selected, i, update_info;

	delete_rect.y2 = delete_rect.y1 = write_rect.y1 = app->my;
	delete_rect.x2 = delete_rect.x1 = write_rect.x1 = app->mx;
	
	while( !mt_wind_update (BEG_MCTRL, app->aes_global));

	do
	{
		mt_graf_mkstate( &write_rect.x2, &write_rect.y2, &app->mbut, &app->mkstate, app->aes_global);

		if(( win->handle != 0) && ( write_rect.y2 < file_area->g_y) && ( tab->ypos > 0L))
		{
			mt_evnt_timer( 50L, app->aes_global);
			Draw_Rubberbox( win, file_area, &delete_rect, app);
			delete_rect.y1 += tab->h_u;
			write_rect.y1  += tab->h_u;
			app->aes_buff[4] = WA_UPLINE;
			win->WinArrow( win, app);
		}
		else if(( win->handle != 0) && ( write_rect.y2 > file_area->g_y + file_area->g_h - 1) && ( tab->ypos < tab->ypos_max - row) && ( tab->ypos_max > row))
		{
			mt_evnt_timer( 50L, app->aes_global);
			Draw_Rubberbox( win, file_area, &delete_rect, app);
			delete_rect.y1 -= tab->h_u;
			write_rect.y1  -= tab->h_u;
			app->aes_buff[4] = WA_DNLINE;
			win->WinArrow( win, app);
		}
		else if(( delete_rect.y2 == write_rect.y2) && ( delete_rect.x2 == write_rect.x2))
			continue;
		else
			Draw_Rubberbox( win, file_area, &delete_rect, app);

		delete_rect.x1 = MIN( write_rect.x1, write_rect.x2) - file_area->g_x;
		delete_rect.y1 = MIN( write_rect.y1, write_rect.y2) - file_area->g_y;
		delete_rect.x2 = MAX( write_rect.x1, write_rect.x2) - file_area->g_x;
		delete_rect.y2 = MAX( write_rect.y1, write_rect.y2) - file_area->g_y;

		update_info = 0;

		for( i = 0; i < tab->nbr_icons; i++)
		{
			selected = 0;

			if( tab->view_mode == LIST_MODE && rect16_intersect( &delete_rect, &tab->entry[i].box_pos))
			{
				selected = 1;
			}
			else if( rect16_intersect( &delete_rect, &tab->entry[i].icn_pos))
			{
				selected = 1;
			}
			else if( rect16_intersect( &delete_rect, &tab->entry[i].txt_pos))
			{
				selected = 1;
			}

			if( selected == 1)
			{
				if( !check_selected_entry( tab, &tab->entry[i]))
				{
					add_selected_entry( tab, &tab->entry[i]);
					redraw_area( win, file_area->g_x, file_area->g_y, &tab->entry[i].box_pos, app);
					update_info = 1;
				}
			}
			else
			{
				if( check_selected_entry( tab, &tab->entry[i]))
				{
					remove_selected_entry( tab, &tab->entry[i]);
					redraw_area( win, file_area->g_x, file_area->g_y, &tab->entry[i].box_pos, app);
					update_info = 1;
				}
			}
		}

		if( update_info == 1)
			update_infobar( win, win_area, app);
		
		Draw_Rubberbox( win, file_area, &write_rect, app);

		delete_rect = write_rect;

	} while( app->mbut == 1);

    Draw_Rubberbox( win, file_area, &delete_rect, app);
	delete_rect.x2 = delete_rect.x1;
	delete_rect.y2 = delete_rect.y1;
    Draw_Rubberbox( win, file_area, &delete_rect, app);

	mt_wind_update( END_MCTRL, app->aes_global);
}

