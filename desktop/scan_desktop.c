#include "general.h"
#include "ztext.h"
#include "icon/icon.h"
#include "file/file.h"

#define ALLOC_NDIRENTRIES (4)

/*==================================================================================*
 * int32 scan_desktop:																*
 *		Creates a list with the available drives and files in the "desktop" dir.	*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		tab     	-> the target tab.												*
 * 		app			-> Application's descriptor.									*
 *----------------------------------------------------------------------------------*
 * return: 0 if ok else GEMDOS error code.											*
 *==================================================================================*/

int32 scan_desktop( TAB *tab, APPVar *app)
{
	int 	current_drv, drv, allocated_entries = 0;
	uint32 	drives, tmp_d;
	char 	*env_home, *txt;
	Entry 	*entry = NULL;
	int16	len, row = 0, column = 0;
	int32	error = -EERROR;

	mt_shel_envrn( &env_home, "HOME=", app->aes_global);

	/* Home directory exist? */
	if( env_home != NULL)
	{	
		strcpy( tab->directory, env_home);
		unix2dos( tab->directory);
	}	
	else
		strcpy( tab->directory, app->zdesk_path);

	len = ( int16)strlen( tab->directory) - 1;

	if( tab->directory[len] != '\\')
		strcat( tab->directory, "\\");

	strcat( tab->directory, "desktop");
	
	if( mychdir( tab->directory) != 0)
	{
		error = ( int32)Dcreate( tab->directory);

		if( error != 0L)
			return error;

		if( mychdir( tab->directory) != 0)
			return error;			
	}

	/* read contents of directory and build list */
	drives = Drvmap();

	tmp_d = drives;

	for( current_drv = 0; current_drv < 32; current_drv++, tmp_d >>= 1)
	{
		if( tmp_d & 0x1)
		{
			drv = 'A' + current_drv;
			
			if( strchr( app->pref.skip_drive, drv))
				continue;
		
			/* allocate new memory for entries if needed */
			if( tab->nbr_icons >= allocated_entries)
			{
				if( tab->nbr_icons == 0)
				{
					if(( tab->entry = ( Entry *)gmalloc( ALLOC_NDIRENTRIES * sizeof( Entry))) == NULL)
					{
						error = -ENOMEM;
						goto abort;
					}
				}
				else
				{
					if (( entry = ( Entry *)grealloc( tab->entry, ( allocated_entries + ALLOC_NDIRENTRIES) * sizeof( Entry))) == NULL)
					{
						error = -ENOMEM;
						goto abort;
					}

					tab->entry = entry;
				}

				allocated_entries += ALLOC_NDIRENTRIES;
			}
	
			/* fill out and add new entry */
			entry = &tab->entry[tab->nbr_icons];

			entry->thumbnail.fd_addr	= NULL;
			entry->next_selected 		= NULL;
			entry->file_size 			= 0;
			entry->m_time 				= 0;

			sprintf( entry->name, "%c:\\", drv);

			if( Dreadlabel( entry->name, entry->name_shown, ENTRY_NAME_SHOW_CHARS - 1) != 0)
			{
				if(( drv == 'A') || ( drv == 'B'))
					mt_rsrc_gaddr( R_STRING, FLOPPY, &txt, app->aes_global);
				else
					mt_rsrc_gaddr( R_STRING, DRIVE, &txt, app->aes_global);			

				sprintf( entry->name_shown, txt, drv);
			}
		

			entry->tsize[0] = '\0';
			entry->tdate[0] = '\0';

			entry->status		= ES_DRIVE;

			entry->icon_txt_w = entry->real_txt_w = get_text_width(( const char *)entry->name_shown);
	
			if( entry->icon_txt_w > tab->w_u - ( app->x_space << 1))
				entry->icon_txt_w = name_shorter( tab->w_u - ( app->x_space << 1), entry->name_shown);

			entry->box_pos.g_x = column * tab->w_u;
			entry->box_pos.g_y = row * tab->h_u;
			entry->box_pos.g_w = tab->w_u;
			entry->box_pos.g_h = tab->h_u;

			entry->icn_pos.g_x = entry->box_pos.g_x + (( tab->w_u - tab->icon_size + 1) >> 1);
			entry->icn_pos.g_y = entry->box_pos.g_y + app->y_space;
			entry->icn_pos.g_w = tab->icon_size;
			entry->icn_pos.g_h = tab->icon_size;
			
			entry->txt_pos.g_x = entry->box_pos.g_x + (( tab->w_u - entry->icon_txt_w + 1) >> 1);
			entry->txt_pos.g_y = entry->box_pos.g_y + tab->h_u - app->y_space - app->hcell;
			entry->txt_pos.g_w = entry->icon_txt_w;
			entry->txt_pos.g_h = app->hcell;
			
			set_icon( entry->name, entry, app);

			tab->nbr_icons++;

			if( tab->xpos_max - 1 >= column)
			{
				column++;
			}
			else
			{
				column = 0;
				row++;
			}
		}
	}

	if(( app->pref.ram_disk_option & RAMDISK_MOUNTASDRIVE) && ( check_file( "U:\\ram") & ES_DIR))
	{
		if( tab->nbr_icons >= allocated_entries)
		{
			if (( entry = ( Entry *)grealloc( tab->entry, ( allocated_entries + ALLOC_NDIRENTRIES) * sizeof( Entry))) == NULL)
			{
				error = -ENOMEM;
				goto abort;
			}
				
			tab->entry = entry;
			
			allocated_entries += ALLOC_NDIRENTRIES;
		}
	
		/* fill out and add new entry */
		entry = &tab->entry[tab->nbr_icons];
		
		entry->thumbnail.fd_addr	= NULL;
		entry->next_selected 		= NULL;
		entry->file_size 			= 0;
		entry->m_time 				= 0;
		
		mt_rsrc_gaddr( R_STRING, RAM_DISK, &txt, app->aes_global);
		
		strcpy( entry->name, "U:\\ram");
		strcpy( entry->name_shown, txt);		
		
		entry->tsize[0] = '\0';
		entry->tdate[0] = '\0';
		
		entry->status		= ES_DRIVE;
		
		entry->icon_txt_w = entry->real_txt_w = get_text_width(( const char *)entry->name_shown);
		
		if( entry->icon_txt_w > tab->w_u - ( app->x_space << 1))
			entry->icon_txt_w = name_shorter( tab->w_u - ( app->x_space << 1), entry->name_shown);
		
		entry->box_pos.g_x = column * tab->w_u;
		entry->box_pos.g_y = row * tab->h_u;
		entry->box_pos.g_w = tab->w_u;
		entry->box_pos.g_h = tab->h_u;
		
		entry->icn_pos.g_x = entry->box_pos.g_x + (( tab->w_u - tab->icon_size + 1) >> 1);
		entry->icn_pos.g_y = entry->box_pos.g_y + app->y_space;
		entry->icn_pos.g_w = tab->icon_size;
		entry->icn_pos.g_h = tab->icon_size;
		
		entry->txt_pos.g_x = entry->box_pos.g_x + (( tab->w_u - entry->icon_txt_w + 1) >> 1);
		entry->txt_pos.g_y = entry->box_pos.g_y + tab->h_u - app->y_space - app->hcell;
		entry->txt_pos.g_w = entry->icon_txt_w;
		entry->txt_pos.g_h = app->hcell;
		
		entry->icon			= ICON_RAMDISK;
		entry->mini_icon	= ICON_RAMDISK_MINI;
		
		tab->nbr_icons++;
		
		if( tab->xpos_max - 1 >= column)
		{
			column++;
		}
		else
		{
			column = 0;
			row++;
		}
	}	
	
	return 0L;


abort:
	if( tab->entry)
		gfree(( void*)tab->entry);
	
	return error;
}


