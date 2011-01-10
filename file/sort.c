#include "general.h"
#include "ztext.h"


static int cmp_entries_name( const void *obj_a, const void *obj_b)
{
	Entry *entry_a = (Entry *)obj_a;
	Entry *entry_b = (Entry *)obj_b;
	int cmp_val;

	cmp_val = strcasecmp( entry_a->name, entry_b->name);

	return cmp_val;
}


static int cmp_entries_size( const void *obj_a, const void *obj_b)
{
	Entry *entry_a = (Entry *)obj_a;
	Entry *entry_b = (Entry *)obj_b;
	int cmp_val;
	uint32 size_a, size_b;

	size_a = entry_a->file_size;
	size_b = entry_b->file_size;

	if ( size_a < size_b)
		cmp_val = -1;
	else if ( size_a == size_b)
		cmp_val = 0;
	else 
		cmp_val = 1;	    	

	return cmp_val;
}


static int cmp_entries_date( const void *obj_a, const void *obj_b)
{
	Entry *entry_a = (Entry *)obj_a;
	Entry *entry_b = (Entry *)obj_b;
	int cmp_val;
	uint32 date_a, date_b;

	date_a = entry_a->m_time;
	date_b = entry_b->m_time;

	if( date_a < date_b)
		cmp_val = -1;
	else if( date_a == date_b)
		cmp_val = 0;
	else 
		cmp_val = 1;	    	

	return cmp_val;
}


static int cmp_entries_dirsbeforefiles( const void *obj_a, const void *obj_b)
{
	Entry *entry_a = (Entry *)obj_a;
	Entry *entry_b = (Entry *)obj_b;
	int cmp_val;


	if( entry_a->icon == ICON_FOLDER)
		cmp_val = (( entry_b->icon == ICON_FOLDER) ? 0 : -1);
	else
		cmp_val = (( entry_b->icon == ICON_FOLDER) ?  1 :  0);

	return cmp_val;
} 


void sort_entries( TAB *tab)
{
	int16 nb_of_dirs = 0, nbr_icons = tab->nbr_icons;
	Entry *entry = tab->entry;

	if( nbr_icons < 2)
		return;

	if( strcmp(( const char *)entry[0].name, "..") == 0)
	{
		nbr_icons--;
		entry = &tab->entry[1];

		if( nbr_icons < 2)
			return;
	}

	switch( tab->sortingmode)
	{
		case MENU_BAR_BY_NAME:
			if( tab->sort_dirs_before_files == TRUE) 
			{
				/* get number of directories */
				qsort( entry, nbr_icons, sizeof( Entry), cmp_entries_dirsbeforefiles);
				while(( nb_of_dirs < nbr_icons) && ( entry[nb_of_dirs].icon == ICON_FOLDER))
					nb_of_dirs++;
				/* sort directories */
				if(nb_of_dirs > 1)
					qsort( entry, nb_of_dirs, sizeof( Entry), cmp_entries_name);
				/* sort files */
				if( nbr_icons - nb_of_dirs > 1)
					qsort( &entry[nb_of_dirs], nbr_icons - nb_of_dirs, sizeof( Entry), cmp_entries_name);
			}
			else
				qsort( entry, nbr_icons, sizeof( Entry), cmp_entries_name);
			
			break;

		case MENU_BAR_BY_SIZE:
			if( tab->sort_dirs_before_files == TRUE) 
			{			
				/* get number of directories */
				qsort( entry, nbr_icons, sizeof(Entry), cmp_entries_dirsbeforefiles);
				while(( nb_of_dirs < nbr_icons) && ( entry[nb_of_dirs].icon == ICON_FOLDER))
					nb_of_dirs++;
				/* sort directories */
				if( nb_of_dirs > 1)
					qsort( entry, nb_of_dirs, sizeof(Entry), cmp_entries_size);
				/* sort files */
				if( nbr_icons - nb_of_dirs > 1)
					qsort(&entry[nb_of_dirs], nbr_icons - nb_of_dirs, sizeof(Entry), cmp_entries_size);
			}
			else
				qsort( entry, nbr_icons, sizeof( Entry), cmp_entries_size);

			break;

		case MENU_BAR_BY_DATE:	
			if( tab->sort_dirs_before_files == TRUE) 
			{			
				/* get number of directories */
				qsort( entry, nbr_icons, sizeof(Entry), cmp_entries_dirsbeforefiles);
				while(( nb_of_dirs < nbr_icons) && ( entry[nb_of_dirs].icon == ICON_FOLDER))
					nb_of_dirs++;
				/* sort directories */
				if( nb_of_dirs > 1)
					qsort( entry, nb_of_dirs, sizeof(Entry), cmp_entries_date);
				/* sort files */
				if( nbr_icons - nb_of_dirs > 1)
					qsort( &entry[nb_of_dirs], nbr_icons - nb_of_dirs, sizeof(Entry), cmp_entries_date);
			}
			else
				qsort( entry, nbr_icons, sizeof( Entry), cmp_entries_date);
			
			break;
		
		default:
			break;
	}
}
