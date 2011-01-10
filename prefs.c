#include "general.h"

/*==================================================================================*
 * prefs_read:																		*
 *	Reads the content of the preference file.										*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		pref	-> Preferences.														*
 * 		appl	-> Application's descriptor.										*
 *----------------------------------------------------------------------------------*
 * return:	 																		*
 * 		--																			*
 *==================================================================================*/

void prefs_read( PREFERENCE *pref, APPVar *appl)
{
	char *env_home, filename[MAX_PATH];
	FILE *inffile;
	int32 filepos;
	int16  len;	
	boolean valid_entry_found;

	
	/* window options */
	pref->show_toolbar					= FALSE;
	pref->show_pathbar					= FALSE;
	pref->show_sidebar					= FALSE;
	pref->show_infobar					= TRUE;
	
	/* file sorting */
	pref->sortingmode					= MENU_BAR_BY_NAME;
	pref->sort_dirs_before_files		= TRUE;
	
	/* file area */
	pref->view_mode						= ICON_MODE;
	pref->smooth_thumbnail				= FALSE;
	pref->show_hidden					= FALSE;
	pref->win_background_color			= WHITE;
	pref->win_txt_color					= BLACK;
	pref->win_highlight_color			= YELLOW;
	pref->win_highlight_style			= FIS_USER;
	pref->win_txt_highlight_color		= BLACK;
	pref->file_separator_style			= 7;
	pref->file_separator_color			= LBLACK;
	
	/* the columns in list mode */
	pref->selected_column_color			= LWHITE;
	pref->resize_column_in_realtime		= TRUE;

	/* buttons and widgets */
	pref->button_off_background			= LWHITE;
	pref->button_off_light_color		= WHITE;
	pref->button_off_dark_color			= LBLACK;
	pref->button_off_text_color			= BLACK;
	pref->button_off_text_shadow_color	= WHITE;
	pref->button_on_background			= LBLACK;
	pref->button_on_light_color			= WHITE;
	pref->button_on_dark_color 			= BLACK;
	pref->button_on_text_color			= BLACK;
	pref->button_on_text_shadow_color	= LWHITE;
	pref->object_text_is_3D				= FALSE;
	
	/* tabs */
	pref->show_tabbar					= FALSE;
	pref->tab_off_background_color		= LYELLOW;
	pref->tab_off_text_color			= BLACK;
	pref->tab_on_highlight_color		= YELLOW;	/* -1 if none */
	pref->default_tab_path				= T_CDRIVE;
	pref->tab_options					= 2;
	
	/* drives */
	pref->ram_disk_option				= 1;
	strcpy( pref->skip_drive, "BU");	
	
	/* desktop */
	pref->desk_pic[0]					= '\0';
	pref->desk_background_color			= LBLACK;	


	mt_shel_envrn( &env_home, "HOME=", appl->aes_global);

	/* Home directory exist? */
	if( env_home != NULL)
	{
		strcpy( filename, env_home);
		len = ( int16)strlen( filename) - 1;

		if (( filename[len] != '\\') && ( filename[len] != '/'))
			strcat( filename, "\\");

		strcat( filename, "zdesk.inf");

		if (( inffile = fopen( filename, "rb+")) != NULL)
			goto loop;

	}

	/* With ftell, we MUST open the file in binary mode and not in text mode !!! */
	if (( inffile = fopen( "zdesk.inf", "rb+")) == NULL)
		return;

loop:
		
	do 
	{
		valid_entry_found = FALSE;
			
		filepos = ftell( inffile); 

		/* window options */
		
		if ( fscanf( inffile, "show_toolbar=%d ", &pref->show_toolbar) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);		
		
		if ( fscanf( inffile, "show_pathbar=%d ", &pref->show_pathbar) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);		
		
		if ( fscanf( inffile, "show_sidebar=%d ", &pref->show_sidebar) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);		
		
		if ( fscanf( inffile, "show_infobar=%d ", &pref->show_infobar) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);				
		

		/* file sorting */
		
		if ( fscanf( inffile, "sortingmode=%d ", &pref->sortingmode) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);		
		
		if ( fscanf( inffile, "sort_dirs_before_files=%d ", &pref->sort_dirs_before_files) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);
		
		
		/* file area */		
		
		if ( fscanf( inffile, "view_mode=%d ", &pref->view_mode) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);		

		if ( fscanf( inffile, "smooth_thumbnail=%d ", &pref->smooth_thumbnail) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);		
		
		if ( fscanf( inffile, "show_hidden=%d ", &pref->show_hidden) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);
		
		if ( fscanf( inffile, "win_background_color=%d ", &pref->win_background_color) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);
		
		if ( fscanf( inffile, "win_txt_color=%d ", &pref->win_txt_color) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);
		
		if ( fscanf( inffile, "win_highlight_color=%d ", &pref->win_highlight_color) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);	
		
		if ( fscanf( inffile, "win_highlight_style=%d ", &pref->win_highlight_style) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);			
		
		if ( fscanf( inffile, "win_txt_highlight_color=%d ", &pref->win_txt_highlight_color) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);		
		
		if ( fscanf( inffile, "file_separator_style=%d ", &pref->file_separator_style) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);			
		
		if ( fscanf( inffile, "file_separator_color=%d ", &pref->file_separator_color) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);

		
		/* the columns in list mode */		
		
		if ( fscanf( inffile, "selected_column_color=%d ", &pref->selected_column_color) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);			
		
		if ( fscanf( inffile, "resize_column_in_realtime=%d ", &pref->resize_column_in_realtime) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);	
		
		
		/* buttons and widgets */
		
		if ( fscanf( inffile, "pref->button_off_background=%d ", &pref->button_off_background) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);				
		
		if ( fscanf( inffile, "button_off_light_color=%d ", &pref->button_off_light_color) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);			
		
		if ( fscanf( inffile, "button_off_dark_color=%d ", &pref->button_off_dark_color) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);	
		
		if ( fscanf( inffile, "button_off_text_color=%d ", &pref->button_off_text_color) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);			
		
		if ( fscanf( inffile, "button_off_text_shadow_color=%d ", &pref->button_off_text_shadow_color) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);	
		
		if ( fscanf( inffile, "button_on_background=%d ", &pref->button_on_background) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);	
		
		if ( fscanf( inffile, "button_on_light_color=%d ", &pref->button_on_light_color) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);	
		
		if ( fscanf( inffile, "button_on_dark_color=%d ", &pref->button_on_dark_color) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);	
		
		if ( fscanf( inffile, "button_on_text_color=%d ", &pref->button_on_text_color) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);	
		
		if ( fscanf( inffile, "button_on_text_shadow_color=%d ", &pref->button_on_text_shadow_color) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);	
		
		if ( fscanf( inffile, "object_text_is_3D=%d ", &pref->object_text_is_3D) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);			
		
		
		/* tabs */

		if ( fscanf( inffile, "show_tabbar=%d ", &pref->show_tabbar) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);				
		
		if ( fscanf( inffile, "tab_off_background_color=%d ", &pref->tab_off_background_color) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);	
		
		if ( fscanf( inffile, "tab_off_text_color=%d ", &pref->tab_off_text_color) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);			

		if ( fscanf( inffile, "tab_on_highlight_color=%d ", &pref->tab_on_highlight_color) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);			
		
		if ( fscanf( inffile, "default_tab_path=%d ", &pref->default_tab_path) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);
		
		if ( fscanf( inffile, "tab_options=%d ", &pref->tab_options) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);			
		

		/* drives */
		
		if ( fscanf( inffile, "ram_disk_option=%d ", &pref->ram_disk_option) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);
		
		if ( fscanf( inffile, "skip_drive=%s ", pref->skip_drive) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);
		
		
		/* desktop */
		
		if ( fscanf( inffile, "desk_pic=%s ", pref->desk_pic) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);

		if ( fscanf( inffile, "desk_background_color=%d ", &pref->desk_background_color) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);


		if ( fscanf( inffile, "%s ", filename) == 1)
		{
			valid_entry_found = TRUE;
			continue;
		}
		else
			fseek( inffile, filepos, SEEK_SET);

	} while ( valid_entry_found); 

	fclose( inffile);
}



/*==================================================================================*
 * prefs_write:																		*
 *	Save the preference.															*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		pref	-> Preferences.														*
 * 		appl	-> Application's descriptor.										*
 *----------------------------------------------------------------------------------*
 * return:																			* 													
 * 		--																			*
 *==================================================================================*/

void prefs_write( PREFERENCE *pref, APPVar *appl)
{
	char	*env_home, filename[MAX_PATH];
	FILE 	*inffile;
	int16  	len;	

// return;
	
	mt_shel_envrn( &env_home, "HOME=", appl->aes_global);

	/* Home directory exist? */
	if( env_home != NULL)
		strcpy( filename, env_home);
	else
		strcpy( filename, ( char *)appl->zdesk_path);

	len = ( int16)strlen( filename) - 1;

	if ((filename[len] != '\\') && (filename[len] != '/'))
		strcat( filename, "\\");

	strcat( filename, "zdesk.inf");

	if (( inffile = fopen( filename, "wb+")) == NULL)
		return;

	
	/* window options */
	fprintf( inffile, "show_toolbar=%d\r\n", pref->show_toolbar);	
	fprintf( inffile, "show_pathbar=%d\r\n", pref->show_pathbar);
	fprintf( inffile, "show_sidebar=%d\r\n", pref->show_sidebar);
	fprintf( inffile, "show_infobar=%d\r\n", pref->show_infobar);
	
	/* file sorting */
	fprintf( inffile, "sortingmode=%d\r\n", pref->sortingmode);
	fprintf( inffile, "sort_dirs_before_files=%d\r\n", pref->sort_dirs_before_files);	
	
	/* file area */
	fprintf( inffile, "view_mode=%d\r\n", pref->view_mode);
	fprintf( inffile, "smooth_thumbnail=%d\r\n", pref->smooth_thumbnail);	
	fprintf( inffile, "show_hidden=%d\r\n", pref->show_hidden);
	fprintf( inffile, "win_background_color=%d\r\n", pref->win_background_color);
	fprintf( inffile, "win_txt_color=%d\r\n", pref->win_txt_color);
	fprintf( inffile, "win_highlight_color=%d\r\n", pref->win_highlight_color);
	fprintf( inffile, "win_highlight_style=%d\r\n", pref->win_highlight_style);	
	fprintf( inffile, "win_txt_highlight_color=%d\r\n", pref->win_txt_highlight_color);
	fprintf( inffile, "file_separator_style=%d\r\n", pref->file_separator_style);	
	fprintf( inffile, "file_separator_color=%d\r\n", pref->file_separator_color);	
	
	/* the columns in list mode */
	fprintf( inffile, "selected_column_color=%d\r\n", pref->selected_column_color);
	fprintf( inffile, "resize_column_in_realtime=%d\r\n", pref->resize_column_in_realtime);	
	
	/* buttons and widgets */
	fprintf( inffile, "button_off_background=%d\r\n", pref->button_off_background);	
	fprintf( inffile, "button_off_light_color=%d\r\n", pref->button_off_light_color);
	fprintf( inffile, "button_off_dark_color=%d\r\n", pref->button_off_dark_color);	
	fprintf( inffile, "button_off_text_color=%d\r\n", pref->button_off_text_color);	
	fprintf( inffile, "button_off_text_shadow_color=%d\r\n", pref->button_off_text_shadow_color);	
	fprintf( inffile, "button_on_background=%d\r\n", pref->button_on_background);	
	fprintf( inffile, "button_on_light_color=%d\r\n", pref->button_on_light_color);
	fprintf( inffile, "button_on_dark_color=%d\r\n", pref->button_on_dark_color);
	fprintf( inffile, "button_on_text_color=%d\r\n", pref->button_on_text_color);
	fprintf( inffile, "button_on_text_shadow_color=%d\r\n", pref->button_on_text_shadow_color);
	fprintf( inffile, "object_text_is_3D=%d\r\n", pref->object_text_is_3D);		
	
	/* tabs */
	fprintf( inffile, "show_tabbar=%d\r\n", pref->show_tabbar);	
	fprintf( inffile, "tab_off_background_color=%d\r\n", pref->tab_off_background_color);	
	fprintf( inffile, "tab_off_text_color=%d\r\n", pref->tab_off_text_color);
	fprintf( inffile, "tab_on_highlight_color=%d\r\n", pref->tab_on_highlight_color);	
	fprintf( inffile, "default_tab_path=%d\r\n", pref->default_tab_path);
	fprintf( inffile, "tab_options=%d\r\n", pref->tab_options);	
	
	/* drives */
	fprintf( inffile, "ram_disk_option=%d\r\n", pref->ram_disk_option);	
	fprintf( inffile, "skip_drive=%s\r\n", pref->skip_drive);	
	
	/* desktop */
	fprintf( inffile, "desk_pic=%s\r\n", pref->desk_pic);
	fprintf( inffile, "desk_background_color=%d\r\n", pref->desk_background_color);

	fclose( inffile);
}

