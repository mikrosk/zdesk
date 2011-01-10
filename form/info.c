#include "general.h"
#include "evnt_loop.h"
#include "misc.h"
#include "menu.h"
#include "winfile/WinFile.h"
#include "icon/icon.h"
#include "file/file.h"
#include "ztext.h"
#include <mint/dcntl.h>



/*==================================================================================*
 * info_box_mouse:																	*
 *		Handle the mouse event.														*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void info_box_mouse( WINDOW *win, APPVar *app)
{
	int16	res, popup_res, attrib_to_change = -1, right_to_change = -1;
	int32	new_prgflags = -1;
	char	*txt;
	GRECT	win_area;
	OBJECT	*flags_popup, *info_rsc = app->infobox->rsc;	
	
	mt_wind_get_grect( win->handle, WF_WORKXYWH, &win_area, app->aes_global);

	info_rsc[INFOBOX_ROOT].ob_x = win_area.g_x;
	info_rsc[INFOBOX_ROOT].ob_y = win_area.g_y;
	
	res = mt_objc_find( info_rsc, INFOBOX_ROOT, MAX_DEPTH, app->mx, app->my, app->aes_global);
	
	if(( res < 0) || ( info_rsc[res].ob_flags & OF_HIDETREE) || ( info_rsc[res].ob_state & OS_DISABLED))
		return;
	
	switch( res)
	{
		case INFOBOX_PERMISSIONS:
			mt_objc_change( info_rsc, INFOBOX_PERMISSIONS, 0, app->x, app->y, app->w, app->h, OS_SELECTED, TRUE, app->aes_global);
			while( app->mbut)
				mt_graf_mkstate( &app->mx, &app->my, &app->mbut, &app->mkstate, app->aes_global);
			info_rsc[INFOBOX_PERMISSIONS].ob_state  =  OS_NORMAL;
			info_rsc[INFOBOX_ATTRIBUTES].ob_flags  &= ~OF_HIDETREE;
			info_rsc[INFOBOX_RIGHTS_BOX].ob_flags  &= ~OF_HIDETREE;
			info_rsc[INFOBOX_FLAGS_BOX].ob_flags   |=  OF_HIDETREE;		
			info_rsc[INFOBOX_PERMISSIONS].ob_flags |=  OF_HIDETREE;	
			win_area.g_y += info_rsc[INFOBOX_FLAGS_BOX].ob_y;
			*(GRECT*)&app->aes_buff[4] = win_area;
			win->WinRedraw( win, app);
			break;
		case INFOBOX_ATTRIBUTES:
			mt_objc_change( info_rsc, INFOBOX_ATTRIBUTES, 0, app->x, app->y, app->w, app->h, OS_SELECTED, TRUE, app->aes_global);
			while( app->mbut)
				mt_graf_mkstate( &app->mx, &app->my, &app->mbut, &app->mkstate, app->aes_global);	
			info_rsc[INFOBOX_ATTRIBUTES].ob_state   =  OS_NORMAL;
			info_rsc[INFOBOX_ATTRIBUTES].ob_flags  |=  OF_HIDETREE;
			info_rsc[INFOBOX_RIGHTS_BOX].ob_flags  |=  OF_HIDETREE;
			info_rsc[INFOBOX_FLAGS_BOX].ob_flags   &= ~OF_HIDETREE;		
			info_rsc[INFOBOX_PERMISSIONS].ob_flags &= ~OF_HIDETREE;	
			win_area.g_y += info_rsc[INFOBOX_FLAGS_BOX].ob_y;
			*(GRECT*)&app->aes_buff[4] = win_area;
			win->WinRedraw( win, app);			
			break;
		case INFOBOX_FASTLOAD:
			new_prgflags = 0x01;
			break;			
		case INFOBOX_LOAD_IN_TTRAM:
			new_prgflags = 0x02;
			break;	
		case INFOBOX_MALLOC_IN_TTRAM:
			new_prgflags = 0x04;
			break;				
		case INFOBOX_SHARED:
			new_prgflags = 0x1000;
			break;		
		case INFOBOX_FLAGS_POPUP:
			mt_rsrc_gaddr( R_TREE, FLAGS, &flags_popup, app->aes_global);
			
			flags_popup[ROOT].ob_x = win_area.g_x + info_rsc[INFOBOX_FLAGS_BOX].ob_x + info_rsc[INFOBOX_FLAGS_POPUP].ob_x;
			flags_popup[ROOT].ob_y = win_area.g_y + info_rsc[INFOBOX_FLAGS_BOX].ob_y + info_rsc[INFOBOX_FLAGS_POPUP].ob_y;
			
			popup_res = mt_form_popup( flags_popup, 0, 0, app->aes_global);

			if( popup_res == -1)
				break;
			
			txt = flags_popup[popup_res].ob_spec.free_string;

			while( *txt == ' ')
				txt++;
			
			if( strcmp( txt, info_rsc[INFOBOX_FLAGS_POPUP].ob_spec.free_string) == 0)
				break;
			
			zstrncpy( info_rsc[INFOBOX_FLAGS_POPUP].ob_spec.free_string, txt, 15);

			app->infobox->prg_header.ph_prgflags &= ~0x30;
			
			if( popup_res == 2)			// Global
				app->infobox->prg_header.ph_prgflags |= 0x10;		
			else if( popup_res == 2)	// Super
				app->infobox->prg_header.ph_prgflags |= 0x20;	
			else if( popup_res == 3)	// Read Only					
				app->infobox->prg_header.ph_prgflags |= 0x30;		
		
			new_prgflags = 0;
			break;	
		case INFOBOX_READONLY:
			attrib_to_change = FA_RDONLY;
			break;	
		case INFOBOX_HIDDEN:
			attrib_to_change = FA_HIDDEN;
			break;	
		case INFOBOX_OWNER_READ:
			right_to_change = S_IRUSR;
			break;
		case INFOBOX_OWNER_WRITE:
			right_to_change = S_IWUSR;
			break;			
		case INFOBOX_OWNER_EXEC:
			right_to_change = S_IXUSR;
			break;			
		case INFOBOX_GROUP_READ:
			right_to_change = S_IRGRP;
			break;			
		case INFOBOX_GROUP_WRITE:
			right_to_change = S_IWGRP;
			break;					
		case INFOBOX_GROUP_EXEC:
			right_to_change = S_IXGRP;
			break;					
		case INFOBOX_OTHERS_READ:
			right_to_change = S_IROTH;
			break;					
		case INFOBOX_OTHERS_WRITE:
			right_to_change = S_IWOTH;
			break;				
		case INFOBOX_OTHERS_EXEC:
			right_to_change = S_IXOTH;
			break;				
		case INFOBOX_OPEN_WITH:
			break;	
		default:
			break;
	}
	
	if( right_to_change > -1)
	{
		int16 right_access = app->infobox->file_xattr.mode & (DEFAULT_DIRMODE | S_ISUID | S_ISGID | S_ISVTX);
		
		if( info_rsc[res].ob_state & OS_SELECTED)
		{	
			info_rsc[res].ob_state &= ~OS_SELECTED;
			right_access &= ~right_to_change;
		}	
		else
		{
			info_rsc[res].ob_state |= OS_SELECTED;
			right_access |= right_to_change;
		}		
		
		app->aes_buff[4] = win_area.g_x + info_rsc[INFOBOX_RIGHTS_BOX].ob_x + info_rsc[res].ob_x;
		app->aes_buff[5] = win_area.g_y + info_rsc[INFOBOX_RIGHTS_BOX].ob_y + info_rsc[res].ob_y;
		app->aes_buff[6] = info_rsc[res].ob_width;
		app->aes_buff[7] = info_rsc[res].ob_height;
		win->WinRedraw( win, app);	
		
		if( mychdir( app->infobox->file_path) != 0)
			return;
		
		if( Fchmod( app->infobox->file_name, right_access) != 0)
			return;
		
		Fxattr( 0, app->infobox->file_name, &app->infobox->file_xattr);
	}
	else if( attrib_to_change > -1)
	{
		if( info_rsc[res].ob_state & OS_SELECTED)
		{	
			info_rsc[res].ob_state &= ~OS_SELECTED;
			app->infobox->file_xattr.attr &= ~attrib_to_change;
		}	
		else
		{
			info_rsc[res].ob_state |= OS_SELECTED;
			app->infobox->file_xattr.attr |= attrib_to_change;
		}		
		
		app->aes_buff[4] = win_area.g_x + info_rsc[res].ob_x;
		app->aes_buff[5] = win_area.g_y + info_rsc[res].ob_y;
		app->aes_buff[6] = info_rsc[res].ob_width;
		app->aes_buff[7] = info_rsc[res].ob_height;
		win->WinRedraw( win, app);

		if( mychdir( app->infobox->file_path) != 0)
			return;		
		
		( void)Fattrib( app->infobox->file_name, 1, app->infobox->file_xattr.attr);
	}
	else if( new_prgflags > -1)
	{
		if( new_prgflags > 0) 
		{
			if( info_rsc[res].ob_state & OS_SELECTED)
			{	
				info_rsc[res].ob_state &= ~OS_SELECTED;
				app->infobox->prg_header.ph_prgflags &= ~new_prgflags;
			}	
			else
			{
				info_rsc[res].ob_state |= OS_SELECTED;
				app->infobox->prg_header.ph_prgflags |= new_prgflags;
			}	
		}
	
		app->aes_buff[4] = win_area.g_x + info_rsc[INFOBOX_FLAGS_BOX].ob_x + info_rsc[res].ob_x;
		app->aes_buff[5] = win_area.g_y + info_rsc[INFOBOX_FLAGS_BOX].ob_y + info_rsc[res].ob_y;
		app->aes_buff[6] = info_rsc[res].ob_width;
		app->aes_buff[7] = info_rsc[res].ob_height;
		win->WinRedraw( win, app);
		
		if( mychdir( app->infobox->file_path) != 0)
			return;		

		new_prgflags = Fopen( app->infobox->file_name, 1);
		
		if( new_prgflags > 0)
		{	
			Fwrite(( int16)new_prgflags, sizeof(PH), &app->infobox->prg_header);
			Fclose(( int16)new_prgflags);
		}		
	}
	
	while( app->mbut)
		mt_graf_mkstate( &app->mx, &app->my, &app->mbut, &app->mkstate, app->aes_global);
}



/*==================================================================================*
 * info_box_close:																	*
 *		Close the info box and free up memory.										*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target window.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void info_box_close( WINDOW *win, APPVar *app)
{
	WinFile_Close( win, app);
	gfree(( void*)app->infobox);	
	app->infobox = NULL;
}



/*==================================================================================*
 * void set_info_box_data:															*
 *		Fill the datas in the info box.												*
 *----------------------------------------------------------------------------------*
 * 		entry, directory				-> the file infos.							* 
 *		app								-> Application's descriptor.				*
 *----------------------------------------------------------------------------------*
 * return:  FALSE if error else TRUE.												*
 *==================================================================================*/

int set_info_box_data( Entry *entry, char *directory, APPVar *app)
{
	int		len;
	int32	prgflag_read = FALSE;
	uint16	current_date;
	char	*txt;
	OBJECT	*flags_popup, *info_rsc = app->infobox->rsc;
	
	if( mychdir( directory) != 0)
		return FALSE;
	
	if( Fxattr( 0, entry->name, &app->infobox->file_xattr) != 0)
		return FALSE;	

	app->infobox->icon = entry->icon;
	app->infobox->mini_icon = entry->mini_icon;		
	
	strcpy( app->infobox->file_path, directory);
	strcpy( app->infobox->file_name, entry->name);
	
	zstrncpy( info_rsc[INFOBOX_NAME].ob_spec.tedinfo->te_ptext, entry->name, 34);
	
	len = ( int16)strlen( entry->name);
	
	if( len >= 33)
	{
		info_rsc[INFOBOX_NAME].ob_spec.tedinfo->te_ptext[30] = '.';
		info_rsc[INFOBOX_NAME].ob_spec.tedinfo->te_ptext[31] = '.';
		info_rsc[INFOBOX_NAME].ob_spec.tedinfo->te_ptext[32] = '.';
		info_rsc[INFOBOX_NAME].ob_spec.tedinfo->te_ptext[33] = '\0';
	}	
	
	zstrncpy( info_rsc[INFOBOX_PATH].ob_spec.tedinfo->te_ptext, directory, 44);
	
	len = ( int16)strlen( directory);
	
	if( len >= 43)
	{
		info_rsc[INFOBOX_PATH].ob_spec.tedinfo->te_ptext[40] = '.';
		info_rsc[INFOBOX_PATH].ob_spec.tedinfo->te_ptext[41] = '.';
		info_rsc[INFOBOX_PATH].ob_spec.tedinfo->te_ptext[42] = '.';
		info_rsc[INFOBOX_PATH].ob_spec.tedinfo->te_ptext[43] = '\0';
	}		
	
	zstrncpy( info_rsc[INFOBOX_SIZE].ob_spec.tedinfo->te_ptext, entry->tsize, 27);
	zstrncpy( info_rsc[INFOBOX_MODIFIED].ob_spec.tedinfo->te_ptext, entry->tdate, 27);
	
	current_date = Tgetdate();
	
	dostime_to_text( info_rsc[INFOBOX_CREATED].ob_spec.tedinfo->te_ptext, &app->infobox->file_xattr.ctime, current_date, app);
	
	if( app->infobox->file_xattr.attr & FA_RDONLY)
		info_rsc[INFOBOX_READONLY].ob_state |= OS_SELECTED;
	else
		info_rsc[INFOBOX_READONLY].ob_state &= ~OS_SELECTED;
	
	if( app->infobox->file_xattr.attr & FA_HIDDEN)
		info_rsc[FA_HIDDEN].ob_state |= OS_SELECTED;
	else
		info_rsc[FA_HIDDEN].ob_state &= ~OS_SELECTED;
	
	if(( app->infobox->file_xattr.mode & DEFAULT_DIRMODE) & S_IRUSR)
		info_rsc[INFOBOX_OWNER_READ].ob_state |= OS_SELECTED;
	else
		info_rsc[INFOBOX_OWNER_READ].ob_state &= ~OS_SELECTED;
	
	if(( app->infobox->file_xattr.mode & DEFAULT_DIRMODE) & S_IWUSR)
		info_rsc[INFOBOX_OWNER_WRITE].ob_state |= OS_SELECTED;
	else
		info_rsc[INFOBOX_OWNER_WRITE].ob_state &= ~OS_SELECTED;
	
	if(( app->infobox->file_xattr.mode & DEFAULT_DIRMODE) & S_IXUSR)
		info_rsc[INFOBOX_OWNER_EXEC].ob_state |= OS_SELECTED;
	else
		info_rsc[INFOBOX_OWNER_EXEC].ob_state &= ~OS_SELECTED;
	
	if(( app->infobox->file_xattr.mode & DEFAULT_DIRMODE) & S_IRGRP)
		info_rsc[INFOBOX_GROUP_READ].ob_state |= OS_SELECTED;
	else
		info_rsc[INFOBOX_GROUP_READ].ob_state &= ~OS_SELECTED;
	
	if(( app->infobox->file_xattr.mode & DEFAULT_DIRMODE) & S_IWGRP)
		info_rsc[INFOBOX_GROUP_WRITE].ob_state |= OS_SELECTED;
	else
		info_rsc[INFOBOX_GROUP_WRITE].ob_state &= ~OS_SELECTED;
	
	if(( app->infobox->file_xattr.mode & DEFAULT_DIRMODE) & S_IXGRP)
		info_rsc[INFOBOX_GROUP_EXEC].ob_state |= OS_SELECTED;
	else
		info_rsc[INFOBOX_GROUP_EXEC].ob_state &= ~OS_SELECTED;
	
	if(( app->infobox->file_xattr.mode & DEFAULT_DIRMODE) & S_IROTH)
		info_rsc[INFOBOX_OTHERS_READ].ob_state |= OS_SELECTED;
	else
		info_rsc[INFOBOX_OTHERS_READ].ob_state &= ~OS_SELECTED;
	
	if(( app->infobox->file_xattr.mode & DEFAULT_DIRMODE) & S_IWOTH)
		info_rsc[INFOBOX_OTHERS_WRITE].ob_state |= OS_SELECTED;
	else
		info_rsc[INFOBOX_OTHERS_WRITE].ob_state &= ~OS_SELECTED;
	
	if(( app->infobox->file_xattr.mode & DEFAULT_DIRMODE) & S_IXOTH)
		info_rsc[INFOBOX_OTHERS_EXEC].ob_state |= OS_SELECTED;
	else
		info_rsc[INFOBOX_OTHERS_EXEC].ob_state &= ~OS_SELECTED;
	
	if( S_ISREG( app->infobox->file_xattr.mode))
	{	
		int32 fd = Fopen( entry->name, 0);
		
		if( fd > 0)
		{	
			if( Fread(( int16)fd, sizeof(PH), &app->infobox->prg_header) == sizeof(PH))
			{	
				if(	app->infobox->prg_header.ph_branch == 0x601a)	
					prgflag_read = TRUE;
			}	

			Fclose(( int16)fd);
		}
	}

	if( prgflag_read == FALSE)
	{	
		info_rsc[INFOBOX_OPEN_WITH].ob_state &= ~OS_DISABLED;
		
		info_rsc[INFOBOX_FASTLOAD].ob_state |= OS_DISABLED;
		info_rsc[INFOBOX_LOAD_IN_TTRAM].ob_state |= OS_DISABLED;
		info_rsc[INFOBOX_MALLOC_IN_TTRAM].ob_state |= OS_DISABLED;
		info_rsc[INFOBOX_SHARED].ob_state |= OS_DISABLED;
		info_rsc[INFOBOX_FLAGS_POPUP].ob_state |= OS_DISABLED;				
		info_rsc[INFOBOX_FASTLOAD].ob_state &= ~OS_SELECTED;
		info_rsc[INFOBOX_LOAD_IN_TTRAM].ob_state &= ~OS_SELECTED;	
		info_rsc[INFOBOX_MALLOC_IN_TTRAM].ob_state &= ~OS_SELECTED;	
		info_rsc[INFOBOX_SHARED].ob_state &= ~OS_SELECTED;
		strcpy( info_rsc[INFOBOX_FLAGS_POPUP].ob_spec.free_string, " ");
	}	
	else 
	{
		info_rsc[INFOBOX_OPEN_WITH].ob_state |= OS_DISABLED;
		
		info_rsc[INFOBOX_FASTLOAD].ob_state &= ~OS_DISABLED;
		info_rsc[INFOBOX_LOAD_IN_TTRAM].ob_state &= ~OS_DISABLED;
		info_rsc[INFOBOX_MALLOC_IN_TTRAM].ob_state &= ~OS_DISABLED;
		info_rsc[INFOBOX_SHARED].ob_state &= ~OS_DISABLED;
		info_rsc[INFOBOX_FLAGS_POPUP].ob_state &= ~OS_DISABLED;
		
		if( app->infobox->prg_header.ph_prgflags & 0x0001)
			info_rsc[INFOBOX_FASTLOAD].ob_state |= OS_SELECTED;
		else
			info_rsc[INFOBOX_FASTLOAD].ob_state &= ~OS_SELECTED;
		
		if( app->infobox->prg_header.ph_prgflags & 0x0002)
			info_rsc[INFOBOX_LOAD_IN_TTRAM].ob_state |= OS_SELECTED;
		else
			info_rsc[INFOBOX_LOAD_IN_TTRAM].ob_state &= ~OS_SELECTED;	
		
		if( app->infobox->prg_header.ph_prgflags & 0x0004)
			info_rsc[INFOBOX_MALLOC_IN_TTRAM].ob_state |= OS_SELECTED;
		else
			info_rsc[INFOBOX_MALLOC_IN_TTRAM].ob_state &= ~OS_SELECTED;		

		if( app->infobox->prg_header.ph_prgflags & 0x1000)
			info_rsc[INFOBOX_SHARED].ob_state |= OS_SELECTED;
		else
			info_rsc[INFOBOX_SHARED].ob_state &= ~OS_SELECTED;	
		
		
		mt_rsrc_gaddr( R_TREE, FLAGS, &flags_popup, app->aes_global);
		
		if(( app->infobox->prg_header.ph_prgflags & 0x00F0) == 0x00)	  // Private
			txt = flags_popup[FLAGS_PRIVATE].ob_spec.free_string;
		else if(( app->infobox->prg_header.ph_prgflags & 0x00F0) == 0x10) // Global
			txt = flags_popup[FLAGS_GLOBAL].ob_spec.free_string;		
		else if(( app->infobox->prg_header.ph_prgflags & 0x00F0) == 0x20) // Super
			txt = flags_popup[FLAGS_SUPER].ob_spec.free_string;	
		else											// Read Only 0x30
			txt = flags_popup[FLAGS_RONLY].ob_spec.free_string;
		
		while( *txt == ' ')
			txt++;
		
		zstrncpy( info_rsc[INFOBOX_FLAGS_POPUP].ob_spec.free_string, txt, 15);
	}

	return TRUE;
}



/*==================================================================================*
 * void info_box_redraw:															*
 *		redraw the info box.														*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		win     			-> the target WINDOW.									*
 * 		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return: -																		*
 *==================================================================================*/ 

void info_box_redraw( WINDOW *win, APPVar *app)
{
	GRECT win_area;
	GRECT r1, rdraw = *( GRECT*)&app->aes_buff[4];
	
	/* check the graf clipping rectangle */
	if( !rc_intersect(( GRECT*)&app->x, &rdraw))
		return;

	mt_wind_get_grect( win->handle, WF_WORKXYWH, &win_area, app->aes_global);

	if( !rc_intersect( &win_area, &rdraw))
		return;	
	
	app->infobox->rsc[INFOBOX_ROOT].ob_x = win_area.g_x;
	app->infobox->rsc[INFOBOX_ROOT].ob_y = win_area.g_y;
	app->icon_rsc[app->infobox->icon].ob_x = win_area.g_x + app->infobox->rsc[INFOBOX_ICON].ob_x + 8;
	app->icon_rsc[app->infobox->icon].ob_y = win_area.g_y + app->infobox->rsc[INFOBOX_ICON].ob_y + 8;
	app->icon_rsc[app->infobox->icon].ob_state = OS_NORMAL;
	
	while( !mt_wind_update( BEG_UPDATE, app->aes_global));	
	mt_wind_get_grect( win->handle, WF_FIRSTXYWH, &r1, app->aes_global);	
	v_hide_c( app->vdi_handle);

	while( r1.g_w && r1.g_h) 
	{
		if( rc_intersect( &rdraw, &r1))
		{	
			ClipOn( app, &r1);
			mt_objc_draw_grect( app->infobox->rsc, ROOT, MAX_DEPTH, &r1, app->aes_global);
			draw_cicon( app->infobox->icon, app);
		}
		
		mt_wind_get_grect( win->handle, WF_NEXTXYWH, &r1, app->aes_global);
	}

	ClipOn( app, ( GRECT*)&app->x);
	v_show_c( app->vdi_handle, 1);
	mt_wind_update( END_UPDATE, app->aes_global);
}



/*==================================================================================*
 * void open_info_box:																*
 *		Open the info box if necessary.												*
 *----------------------------------------------------------------------------------*
 * 		winfile     		-> the window where is the file.						* 
 *		app					-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:  --																		*
 *==================================================================================*/

void open_info_box( WINDOW *winfile, APPVar *app)
{
	WINDOW		*win, *last_win;
	int16		dum, handle;
	char		*win_title;
	Entry		*entry;

	if( app->infobox)	
		WinFile_Top( app->infobox->win, app);
		
	if( !winfile || winfile->status & WS_FORM || !winfile->selected_tab || !winfile->selected_tab->first_selected)
		return;	

	entry = winfile->selected_tab->first_selected;
	
	if( entry->status & ES_DRIVE)
		return;

	if( app->infobox)
	{	
		if( set_info_box_data( entry, winfile->selected_tab->directory, app) == TRUE)
		{	
			app->aes_buff[4] = app->x;
			app->aes_buff[5] = app->y;
			app->aes_buff[6] = app->w;
			app->aes_buff[7] = app->h;		
			info_box_redraw( app->infobox->win, app);
		}
		
		return;
	}	
	
	app->infobox = ( INFO_BOX*)gmalloc( sizeof( INFO_BOX));
	
	if( app->infobox == NULL)
	{
		show_alert( NULL, -ENOMEM, app);
		return;
	}	

	win = ( WINDOW*)gmalloc( sizeof( WINDOW));
	
	if( win == NULL)
	{
		gfree(( void*)app->infobox);
		app->infobox = NULL;
		show_alert( NULL, -ENOMEM, app);
		return;
	}	
	
	handle = mt_wind_create( MOVER|NAME|CLOSER, app->x, app->y, app->w, app->h, app->aes_global);

	if( handle < 0) 
	{
		gfree(( void*)win);
		gfree(( void*)app->infobox);
		app->infobox = NULL;		
		show_alert( NULL, EWINDOW, app);
		return;
	}

	win->handle				= handle;
	win->status				= WS_FORM;
	win->tab				= NULL;
	win->selected_tab		= NULL;
	win->nbr_tabs			= 0;	
	win->edit.edition_mode	= FALSE;
	win->WinRedraw			= info_box_redraw;				
	win->WinIconifyRedraw	= DummyEvnt;			
	win->WinClose			= info_box_close;
	win->WinTop				= WinFile_Top;
	win->WinBottom			= WinFile_Bottom;
	win->WinMove			= WinFile_Move;
	win->WinSize			= DummyEvnt;
	win->WinFull			= DummyEvnt;
	win->WinIconify			= DummyEvnt;
	win->WinUniconify		= DummyEvnt;
	win->WinAlliconify		= DummyEvnt;
	win->WinArrow			= DummyEvnt;
	win->WinVslide			= DummyEvnt;
	win->WinKeyboard		= DummyEvnt;
	win->WinMouse			= info_box_mouse;	

	mt_rsrc_gaddr( R_TREE, INFOBOX, &app->infobox->rsc, app->aes_global);
	
	app->infobox->rsc[INFOBOX_RIGHTS_BOX].ob_x  = 14;
	app->infobox->rsc[INFOBOX_ATTRIBUTES].ob_x  = 340 - app->infobox->rsc[INFOBOX_ATTRIBUTES].ob_width;
	app->infobox->rsc[INFOBOX_PERMISSIONS].ob_x = 340 - app->infobox->rsc[INFOBOX_PERMISSIONS].ob_width;
	app->infobox->rsc[INFOBOX_ROOT].ob_width    = 354;
	app->infobox->rsc[INFOBOX_FLAGS_BOX].ob_flags   |= OF_HIDETREE;
	app->infobox->rsc[INFOBOX_PERMISSIONS].ob_flags |= OF_HIDETREE;	
	app->infobox->rsc[INFOBOX_ATTRIBUTES].ob_flags  &= ~OF_HIDETREE;
	app->infobox->rsc[INFOBOX_RIGHTS_BOX].ob_flags  &= ~OF_HIDETREE;

	
	/* Put the windows in the windows list */
	win->next = NULL;

	for( last_win = app->win; last_win->next; last_win = last_win->next);
	
	last_win->next = win;	
	app->infobox->win = win;

	if( set_info_box_data( entry, winfile->selected_tab->directory, app) == FALSE)
	{
		info_box_close( win, app);
		return;
	}
	
	mt_wind_calc( WC_BORDER, MOVER|NAME|CLOSER, 0, 0, app->infobox->rsc[INFOBOX_ROOT].ob_width, app->infobox->rsc[INFOBOX_ROOT].ob_height, &dum, &dum, &win->w_min, &win->h_min, app->aes_global);

	mt_rsrc_gaddr( R_STRING, INFOBOX_TITLE, &win_title, app->aes_global);
	mt_wind_set_str( handle, WF_NAME, win_title, app->aes_global);
	
	if( mt_wind_open( handle, ( app->w > win->w_min) ? ( app->w - win->w_min) >> 1 : 0, ( app->h - app->y > win->h_min) ? ( app->h - app->y - win->h_min) >> 1 : 0, win->w_min, win->h_min, app->aes_global) == 0)
	{
		info_box_close( win, app);
		show_alert( NULL, EWINDOW, app);
		return;
	}

	win->status |= WS_OPEN;
	
	set_menu_status( win, app);
}




