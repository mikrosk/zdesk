#include "general.h"
#include "misc.h"
#include "ztext.h"
#include "winfile/WinFile.h"


/*==================================================================================*
 * void draw_tab_popup_item:														*
 *		draw the wanted item in the popup.											*
 *----------------------------------------------------------------------------------*
 * 	win						-> The popup windows.									* 
 * 	app						-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void draw_tab_popup_item( int16 item, APPVar *app)
{
	int16 title_color, title_shadow;
	
	v_hide_c( app->vdi_handle);
	
	app->pxy[0] = app->popup.item_position[item].g_x;
	app->pxy[1] = app->popup.item_position[item].g_y;
	app->pxy[2] = app->pxy[0] + app->popup.item_position[item].g_w - 1;
	app->pxy[3] = app->pxy[1] + app->popup.item_position[item].g_h - 1;
	
	if( item == app->popup.selected)
	{
		vsf_color( app->vdi_handle, ( int16)app->pref.button_on_background);
		title_color = ( int16)app->pref.button_on_text_color;
		title_shadow = ( app->pref.object_text_is_3D) ? ( int16)app->pref.button_on_text_shadow_color : -1;		
	}
	else
	{
		vsf_color( app->vdi_handle, ( int16)app->pref.button_off_background);
		title_color = ( int16)app->pref.button_off_text_color;
		title_shadow = ( app->pref.object_text_is_3D) ? ( int16)app->pref.button_off_text_shadow_color : -1;				
	}			
		
	vr_recfl( app->vdi_handle, app->pxy);
	
	app->icon_rsc[app->popup.item_icon[item]].ob_x = app->popup.item_position[item].g_x + 3;
	app->icon_rsc[app->popup.item_icon[item]].ob_y = app->popup.item_position[item].g_y + 1;
		
	mt_objc_draw( app->icon_rsc, app->popup.item_icon[item], 1, app->popup.item_position[item].g_x, app->popup.item_position[item].g_y, app->popup.item_position[item].g_w, app->popup.item_position[item].g_h, app->aes_global);
	
	draw_text( app->popup.item_position[item].g_x + 21, app->popup.item_position[item].g_y + 5, -1, title_color, title_shadow, ( const char *)app->popup.item_name[item], app);		
	
	v_show_c( app->vdi_handle, 1);
}


/*==================================================================================*
 * void open_tab_popup:																*
 *		Create a popup for the hidden tabs.											*
 *----------------------------------------------------------------------------------*
 * 	winfile					-> The parent windows.									* 
 * 	x, y					-> The popup position.									*
 * 	app						-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void open_tab_popup( WINDOW *winfile, int16 x, int16 y, APPVar *app)
{
	int16	i, ypos, popup_width, current_button, is_in_popup;
	TAB		*tab;
	
	app->popup.nbr_items			= 0;
	app->popup.selected				= -1;
	app->popup.old_selected			= -1;
	app->popup.popup_position.g_x	= x;
	app->popup.popup_position.g_y	= y;		
	app->popup.popup_position.g_w	= 0;
	app->popup.popup_position.g_h	= 4;	

	
	for( i = 0, tab = winfile->tab; tab && i < MAX_POPUP_ITEM; tab = tab->next)
	{	
		if( tab->tab_xpos > -1)
			continue;
		
		zstrncpy( app->popup.item_name[i], tab->name, POPUP_ITEM_CHARS);
		popup_width = get_text_width( app->popup.item_name[i]);
		
		if( popup_width > app->popup.popup_position.g_w)
			app->popup.popup_position.g_w = popup_width;
		
		app->popup.item_icon[i]	= tab->tab_icon;
		
		app->popup.nbr_items++;
		
		app->popup.popup_position.g_h += 17;
		
		i++;
	}
	
	app->popup.popup_position.g_w += 28; /* line + line + 3 + icon + 2 + text + 3 + line + line */
	
	while( app->popup.popup_position.g_x + app->popup.popup_position.g_w > app->x + app->w)
		app->popup.popup_position.g_x--;

	while( app->popup.popup_position.g_x < app->x)
		app->popup.popup_position.g_x++;	
	
	while( app->popup.popup_position.g_y + app->popup.popup_position.g_h > app->y + app->h)
		app->popup.popup_position.g_y--;	
	
	
	while( !mt_wind_update (BEG_MCTRL, app->aes_global));
	mt_form_dial( FMD_START, 0, 0, 0, 0, app->popup.popup_position.g_x, app->popup.popup_position.g_y, app->popup.popup_position.g_w, app->popup.popup_position.g_h, app->aes_global);	
	
	v_hide_c( app->vdi_handle);
	
	app->pxy[0] = app->popup.popup_position.g_x;
	app->pxy[1] = app->popup.popup_position.g_y;
	app->pxy[2] = app->pxy[0] + app->popup.popup_position.g_w - 1;
	app->pxy[3] = app->pxy[1];
	app->pxy[4] = app->pxy[2];
	app->pxy[5] = app->popup.popup_position.g_y + app->popup.popup_position.g_h - 1;
	app->pxy[6] = app->pxy[0];
	app->pxy[7] = app->pxy[5];	
	app->pxy[8] = app->pxy[0];
	app->pxy[9] = app->pxy[1];	
	vsl_color( app->vdi_handle, BLACK);
	v_pline( app->vdi_handle, 5, app->pxy);	
	
	app->pxy[0] = app->popup.popup_position.g_x + 1;
	app->pxy[1] = app->popup.popup_position.g_y + app->popup.popup_position.g_h - 2;
	app->pxy[2] = app->popup.popup_position.g_x + app->popup.popup_position.g_w - 2;
	app->pxy[3] = app->pxy[1];
	app->pxy[4] = app->pxy[2];
	app->pxy[5] = app->popup.popup_position.g_y + 1;
	vsl_color( app->vdi_handle, ( int16)app->pref.button_off_dark_color);
	v_pline( app->vdi_handle, 3, app->pxy);	
	
	app->pxy[4] = app->pxy[2];
	app->pxy[2] = app->pxy[0];	
	app->pxy[3] = app->popup.popup_position.g_y + 1;	
	app->pxy[5] = app->pxy[3];	
	vsl_color( app->vdi_handle, ( int16)app->pref.button_off_light_color);
	v_pline( app->vdi_handle, 3, app->pxy);	
	
	v_show_c( app->vdi_handle, 1);
	
	ypos = app->popup.popup_position.g_y + 2;
	
	for( i = 0; i < app->popup.nbr_items; i++, ypos += 17)
	{	
		app->popup.item_position[i].g_x = app->popup.popup_position.g_x + 2;
		app->popup.item_position[i].g_y = ypos;
		app->popup.item_position[i].g_w = app->popup.popup_position.g_w - 4;
		app->popup.item_position[i].g_h = 17;

		draw_tab_popup_item( i, app);
	}
	
	mt_graf_mkstate( &app->mx, &app->my, &current_button, &app->mkstate, app->aes_global);
	app->mbut = current_button;
	
	while( app->mbut == current_button)
	{	
		is_in_popup = FALSE;
		
		for( i = 0; i < app->popup.nbr_items; i++)
		{
			if( IS_IN( app->mx, app->my, app->popup.item_position[i].g_x, app->popup.item_position[i].g_y, app->popup.item_position[i].g_w, app->popup.item_position[i].g_h))
			{
				if( i != app->popup.selected)
				{
					app->popup.old_selected	= app->popup.selected;
					app->popup.selected		= i;
					
					if( app->popup.old_selected > -1)
						draw_tab_popup_item( app->popup.old_selected, app);
					
					draw_tab_popup_item( app->popup.selected, app);
				}
				
				is_in_popup = TRUE;
				break;
			}
		}
		
		if( is_in_popup == FALSE)
		{
			app->popup.old_selected	= app->popup.selected;
			app->popup.selected		= -1;
			
			if( app->popup.old_selected > -1)
				draw_tab_popup_item( app->popup.old_selected, app);
		}
		
		mt_graf_mkstate( &app->mx, &app->my, &app->mbut, &app->mkstate, app->aes_global);
	}
	
	
	mt_form_dial( FMD_FINISH, 0, 0, 0, 0, app->popup.popup_position.g_x, app->popup.popup_position.g_y, app->popup.popup_position.g_w, app->popup.popup_position.g_h, app->aes_global);
	mt_wind_update (END_MCTRL, app->aes_global);	

	if( app->popup.selected == -1)
		return;

	for( i = 0, tab = winfile->tab; tab && i < MAX_POPUP_ITEM; tab = tab->next)
	{	
		if( tab->tab_xpos > -1)
			continue;
			
		if( app->popup.selected == i)
		{
			winfile->selected_tab = tab;
			update_tabs( winfile, -1, app);
			WinFile_Select_Tab( winfile, tab, app);
			break;
		}			

		i++;
	}	
}




