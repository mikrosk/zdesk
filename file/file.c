#include "general.h"
#include "ztext.h"
#include "mfdb.h"
#include "file/sort.h"
#include "icon/icon.h"


#define ALLOC_NDIRENTRIES 4		/* number of entries that should be allocated at once */


/*==================================================================================*
 * int32 check_file:																*
 *		check if the file exists.													*
 *----------------------------------------------------------------------------------*
 * 	full_path_name			-> Full path to the file.								* 
 *----------------------------------------------------------------------------------*
 * return:																			* 
 *		FALSE if the file doesn't exist else the file type( LINK or DIR).			*
 *==================================================================================*/

int32 check_file( char *full_path_name)
{
	int32 status;
	XATTR file_xattr;
	
	if( Fxattr( 0, full_path_name, &file_xattr) != 0)
		return FALSE;
	
	status = TRUE;
	status |= ( S_ISLNK( file_xattr.mode) ? ES_LINK : 0);
	status |= ( S_ISDIR( file_xattr.mode) ? ES_DIR : 0);	
	
	return status;
}


/*==================================================================================*
 * int32 mychdir:																	*
 *		like the standard chdir but simpler( without malloc and unix to dos			*
  *		path conversion).															*
 *----------------------------------------------------------------------------------*
 * 	fullpath_dir		-> The target path.											* 
 *----------------------------------------------------------------------------------*
 * return:																			* 
 *		0 if success or -1.															*
 *==================================================================================*/

int mychdir( char *fullpath_dir)
{
	int16 drv = fullpath_dir[0] - 'A';

	Dsetdrv(drv);

	if( Dsetpath( &fullpath_dir[2]) == 0)
		return 0;

	return -1;
}



/*==================================================================================*
 * int32 scan_dir_file:																*
 *		scan a directory. 															*
 *----------------------------------------------------------------------------------*
 * 	tab						-> Pointer to the target Tab.							*
 * 	current_fullpath_dir	-> Full path to the parent directory.					* 
 * 	new_dir					-> The directory to scan.								*
 * 	app						-> Application's descriptor.							*
 *----------------------------------------------------------------------------------*
 * return:																			* 
 *		0 if success or GEMDOS error code.											*
 *==================================================================================*/

int32 scan_dir_file( TAB *tab, char *current_fullpath_dir, char *new_dir, APPVar *app)
{
	struct dbuf 
	{
		int32 ino;
		char name[MAX_PATH + MAXNAMLEN + 1];
	} dbuf;

	int16 		dum, allocated_entries = 0, nbr_icons = 0;
	Entry 		*entry, *new_entry = NULL;
	int32		dir_handle, xattr_error, error = -EERROR;
	uint16		current_date;
	XATTR		file_xattr;
	double		total_files_size = 0;
	char 		new_fullpath_dir[MAX_PATH];

	if( current_fullpath_dir != NULL)
	{	
		strcpy( new_fullpath_dir, ( const char *)current_fullpath_dir);
		strcat( new_fullpath_dir, new_dir);
	}
	else
		strcpy( new_fullpath_dir, new_dir);
		
	if( mychdir( new_fullpath_dir) != 0)
		return error;

	dir_handle = Dopendir( ".", 0);

	if(( dir_handle & 0xFF000000L) == 0xFF000000L)
		goto abort;

	current_date = Tgetdate();
	
	while( 1)
	{
		error = Dxreaddir( MAXNAMLEN + 1 + sizeof( int32), dir_handle, (char *)&dbuf, &file_xattr, &xattr_error);

		if( error == -ENMFILES) 
			break;
		else if( error != 0)
			goto abort;

		if( xattr_error != 0)
			continue;

		if( strcmp( dbuf.name, ".") == 0)
			continue;

		if( nbr_icons >= allocated_entries)
		{
			if( nbr_icons == 0)
			{
				if(( new_entry = ( Entry *)gmalloc( ALLOC_NDIRENTRIES * sizeof( Entry))) == NULL)
				{
					error = -ENOMEM;
					goto abort;
				}
			}
			else
			{
				if(( entry = ( Entry *)grealloc( new_entry, ( allocated_entries + ALLOC_NDIRENTRIES) * sizeof( Entry))) == NULL)
				{
					error = -ENOMEM;
					goto abort;
				}
				
				new_entry = entry;
			}

			allocated_entries += ALLOC_NDIRENTRIES;
		}

		entry = &new_entry[nbr_icons];

		entry->thumbnail.fd_addr	= NULL;
		entry->next_selected 		= NULL;
		entry->file_size 			= file_xattr.size;

		zstrncpy( entry->name, ( const char *)dbuf.name, MAXNAMLEN);
		zstrncpy( entry->name_shown, ( const char *)dbuf.name, ENTRY_NAME_SHOW_CHARS);

		dostime_to_text( entry->tdate, &file_xattr.mtime, current_date, app);
		entry->m_time = ( file_xattr.mtime << 16) | ( file_xattr.mtime >> 16);
		
		entry->status	= ( file_xattr.attr & FA_HIDDEN ? ES_HIDDEN : 0);		
		entry->status	|= ( S_ISLNK( file_xattr.mode) ? ES_LINK : 0);
		entry->status	|= ( file_xattr.attr & FA_RDONLY ? ES_LOCKED : 0);

		byte_to_text( entry->tsize, ( double)file_xattr.size);
		
		total_files_size += ( double)file_xattr.size;		
		
		if( entry->status & ES_LINK)
		{
			Fxattr( 0, entry->name, &file_xattr);
			entry->status |= ( S_ISDIR( file_xattr.mode) ? ES_DIR : 0);
			Freadlink( MAX_PATH + MAXNAMLEN, dbuf.name, entry->name);
			set_icon(( char *)dbuf.name, entry, app);
		}
		else
		{
			entry->status |= ( S_ISDIR( file_xattr.mode) ? ES_DIR : 0);
			set_icon(( char *)entry->name, entry, app);
		}

		entry->tsize_txt_w = get_text_width(( const char *)entry->tsize);
		entry->tdate_txt_w = get_text_width(( const char *)entry->tdate);
		
		entry->icon_txt_w = entry->real_txt_w = get_text_width(( const char *)entry->name);

		if(( tab->view_mode == ICON_MODE) && ( entry->icon_txt_w > tab->w_u - ( app->x_space << 1)))
			entry->icon_txt_w = name_shorter( tab->w_u - ( app->x_space << 1), entry->name_shown);
		else if(( tab->view_mode == LIST_MODE) && ( entry->icon_txt_w > ( tab->name_column_width - ( tab->icon_size + app->x_space + app->x_space + 1))))
			entry->icon_txt_w = name_shorter( tab->name_column_width - ( 17 + app->x_space + app->x_space), entry->name_shown);		
		
		nbr_icons++;
	}

	Dclosedir( dir_handle);

	if( current_fullpath_dir == NULL)
		strcpy( tab->directory, ( const char *)new_fullpath_dir);
	else if( strcmp( new_dir, "..") == 0)
		dir_parent( tab->directory);
	else if( strcmp( new_dir, ".") != 0)
	{
		strcpy( tab->directory, new_fullpath_dir);
		strcat( tab->directory, "\\");
	}

	byte_to_text( tab->total_files_size, total_files_size);

	if( tab->nbr_icons)
	{
		for( dum = 0; dum < tab->nbr_icons; dum++)
			delete_mfdb( &tab->entry[dum].thumbnail);

		gfree(( void*)tab->entry);
	}

	tab->first_selected = NULL;
	tab->entry = new_entry;
	tab->nbr_icons = nbr_icons;
	
	sort_entries( tab);

	return 0L;


abort:
	Dclosedir( dir_handle);

	mychdir( tab->directory);

	if( new_entry)
		gfree(( void*)new_entry);

	return error;
}






