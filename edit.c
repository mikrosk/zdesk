#include "general.h"
#include "ztext.h"
#include "misc.h"

extern int16 ofwf[256];


int char_put( EDIT *edit, int16 c) 
{
	if( edit->len >= MAXNAMLEN - 2)
		return 0;
	
	if( edit->len == edit->cursor_row)
	{
		edit->buffer[edit->len++] = c;
		edit->buffer[edit->len] = '\0';
		edit->cursor_row++;
	}
	else
	{
		int len;
		
		for( len = edit->len; len > edit->cursor_row; len--)
			edit->buffer[len] = edit->buffer[len-1];
		
		edit->buffer[edit->cursor_row++] = ( char)c;
		edit->len++;
		edit->buffer[edit->len] = '\0';	
	}

	return 1;
}


int char_del( EDIT *edit) 
{
	if(( edit->len < 1) || ( edit->cursor_row < 1))
		return 0;
	
	if( edit->len == edit->cursor_row)
	{
		edit->len--;
		edit->buffer[edit->len] = '\0';
		edit->cursor_row--;
	}
	else
	{
		int cursor_row;
		
		for( cursor_row = edit->cursor_row; cursor_row < edit->len; cursor_row++)
			edit->buffer[cursor_row-1] = edit->buffer[cursor_row];
		
		edit->cursor_row--;
		edit->len--;
		edit->buffer[edit->len] = '\0';
	}
	
	return 1;
}


int curs_left( EDIT *edit) 
{
	if( edit->cursor_row < 1)
		return 0;
	
	edit->cursor_row--;
	
	return 1;
}

int curs_right( EDIT *edit) 
{
	if( edit->cursor_row >= edit->len)
		return 0;
	
	edit->cursor_row++;
	
	return 1;	
}




/*==================================================================================*
 * cursor_position:																	*
 * 		return the cursor position in pixel.										*
 *----------------------------------------------------------------------------------*
 * win:					->	The Target Window.										*
 *----------------------------------------------------------------------------------*
 * return: the pixel position in pixel.												*
 *==================================================================================*/

int16 cursor_position( WINDOW *win)
{
	int16 ch, cursor_xpos = 0;
	int cursor = win->edit.cursor_row;
	char *string = 	win->edit.buffer;
	
	while((( ch = *string++) != 0) && cursor--)
	{	
		if( ch < 0)
			ch += 256;
			
		cursor_xpos += ofwf[ch];
	}
	
	return cursor_xpos;
}



/*==================================================================================*
 * redraw_icon_txt:																	*
 *		redraw the icon's text.														*
 *----------------------------------------------------------------------------------*
 * win     				-> the target window.										*
 * entry 				-> the target entry.										*
 * window_y, window_x 	-> the window's position.									*
 * app					-> Application's descriptor.								*
 *----------------------------------------------------------------------------------*
 * return: -																		*
 *==================================================================================*/

void redraw_icon_txt( WINDOW *win, Entry *entry, int16 window_x, int16 window_y, APPVar *app)
{
    GRECT r1;

	if( win->selected_tab->view_mode == ICON_MODE)
		r1.g_x = entry->txt_pos.g_x - 10;
	else
		r1.g_x = entry->txt_pos.g_x - 5;

	r1.g_w = entry->txt_pos.g_w + 20;
	r1.g_y = entry->txt_pos.g_y - 3;
	r1.g_h = entry->txt_pos.g_h + 6;

	redraw_area( win, window_x, window_y, &r1, app);
}



/*==================================================================================*
 * init_edit_mode:																	*
 *		initialize the 'edition mode'.												*
 *----------------------------------------------------------------------------------*
 * win     				-> the target window.										*
 * entry 				-> the target entry.										*
 * window_y, window_x 	-> the window's position.									*
 * app					-> Application's descriptor.								*
 *----------------------------------------------------------------------------------*
 * returns: -																		*
 *==================================================================================*/

void init_edit_mode( WINDOW *win, Entry *entry, int16 window_x, int16 window_y, APPVar *app)
{
	zstrncpy( win->edit.buffer,( const char *)entry->name, MAXNAMLEN);
	
	win->edit.len = strlen( win->edit.buffer);
	win->edit.cursor_row = win->edit.len;	
	win->edit.edition_mode = TRUE;

	entry->txt_pos.g_w = entry->real_txt_w;
	
	if( win->selected_tab->view_mode == ICON_MODE) 
		entry->txt_pos.g_x = entry->box_pos.g_x + (( win->selected_tab->w_u - entry->txt_pos.g_w + 1) >> 1);		
	
	redraw_icon_txt( win, entry, window_x, window_y, app);
}



/*==================================================================================*
 * exit_edit_mode:																	*
 *		Exit 'edition mode' and refresh the icon's area.							*
 *----------------------------------------------------------------------------------*
 * win     				-> the target window.										*
 * entry 				-> the target entry.										*
 * window_y, window_x 	-> the window's position.									*
 * app					-> Application's descriptor.								*
 *----------------------------------------------------------------------------------*
 * return: -																		*
 *==================================================================================*/

void exit_edit_mode( WINDOW *win, Entry *entry, int16 window_x, int16 window_y, APPVar *app)
{
    GRECT r1;

	r1.g_x = MIN( entry->box_pos.g_x, entry->txt_pos.g_x - 11);
	r1.g_w = MAX( entry->box_pos.g_w, entry->txt_pos.g_w + 26);
	r1.g_y = MIN( entry->box_pos.g_y, entry->txt_pos.g_y - 3);
	r1.g_h = MAX( entry->box_pos.g_h, entry->txt_pos.g_h + 6);

	win->edit.edition_mode = FALSE;

	redraw_area( win, window_x, window_y, &r1, app);
}

