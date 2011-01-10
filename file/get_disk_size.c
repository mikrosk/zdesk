#include "general.h"
#include "ztext.h"


/*==================================================================================*
 * get_disk_size_file:																*
 *		get information about the disk size.										*
 *----------------------------------------------------------------------------------*
 * return: --																		*
 *==================================================================================*/

void get_disk_size_file( TAB *tab, APPVar *app)
{
	_DISKINFO	buf;
	int16 		drv;
	double 		disk_free_bytes;

	/*
	if( tab->directory[0] == 'U' && ( strcmp( tab->directory, "U:\\ram") != 0))
	{
		tab->free_size_txt_w = 0;
		return;
	}
	*/
	
	drv = ( tab->directory[0] & 0x5F) - 'A' + 1;
	Dfree( &buf, drv);
	
	disk_free_bytes = ( double)buf.b_free * ( double)buf.b_secsiz;
	disk_free_bytes *= ( double)buf.b_clsiz;

	byte_to_text( tab->free_size_txt, disk_free_bytes);
	
	strcat( tab->free_size_txt, ( const char *)app->info_available);
	tab->free_size_txt_w = get_text_width(( const char *)tab->free_size_txt);
}

