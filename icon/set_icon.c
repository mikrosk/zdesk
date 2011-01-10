#include "general.h"
#include "ztext.h"


/*==================================================================================*
 * void set_icon:																	*
 *		Set the entry's icon.														*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		name	->	name of the entry.												*
 * 		entry	->	pointer to the entry.											*
 * 		app		->  Application's pointer.											*
 *----------------------------------------------------------------------------------*
 * returns: 																		*
 *		--																			*
 *==================================================================================*/

void set_icon( char *name, Entry *entry, APPVar *app)
{
	int16 i, j;
	char extention[6], *plugin_name, *extention_ptr;
	
	if( entry->status & ES_DRIVE)
	{
		if(( strcmp( name, "A:\\") == 0) || ( strcmp( name, "B:\\") == 0))
		{
			entry->icon			= ICON_FLOPPY;
			entry->mini_icon	= ICON_FLOPPY_MINI;
		}
		else
		{
			entry->icon			= ICON_HD;
			entry->mini_icon	= ICON_HD_MINI;
		}
		return;
	}

	if( entry->status & ES_DIR)
	{
		if( strcmp( name, "..") == 0)
		{
			entry->icon			= ICON_BACK;
			entry->mini_icon	= ICON_BACK_MINI;
			return;
		}

		entry->icon			= ICON_FOLDER;
		entry->mini_icon	= ICON_FOLDER_MINI;
		return;
	}
	
	entry->icon			= ICON_FILE;
	entry->mini_icon	= ICON_FILE_MINI;

	extention_ptr = strrchr( name + 1, ( int)'.');

	if( extention_ptr == NULL)
		return;

	extention_ptr++;

	if( extention_ptr[0] == '\0')
		return;

	zstrncpy( extention, extention_ptr, 6);
	strupr( extention);


	if(( strcmp( extention, "PRG") == 0)
	|| ( strcmp( extention, "APP") == 0))
	{
		entry->icon			= ICON_APP;
		entry->mini_icon	= ICON_APP_MINI;
		return;
	}

	if( strcmp( extention, "ACC") == 0)
	{
		entry->icon			= ICON_ACC;
		entry->mini_icon	= ICON_ACC_MINI;
		return;
	}

	if(( strcmp( extention, "TTP") == 0)
	|| ( strcmp( extention, "TOS") == 0)
	|| ( strcmp( extention, "GTP") == 0))
	{
		entry->icon			= ICON_TTP;
		entry->mini_icon	= ICON_TTP_MINI;
		return;
	}

	if( strcmp( extention, "RSC") == 0)
	{
		entry->icon			= ICON_RSC;
		entry->mini_icon	= ICON_RSC_MINI;
		return;
	}

	if( strcmp( extention, "HYP") == 0)
	{
		entry->icon			= ICON_HELP;
		entry->mini_icon	= ICON_HELP_MINI;
		return;
	}

	if(( strcmp( extention, "TXT") == 0)
	|| ( strcmp( extention, "DOC") == 0))
	{
		entry->icon			= ICON_TXT;
		entry->mini_icon	= ICON_TXT_MINI;
		return;
	}	
	
	if(( strcmp( extention, "SYS") == 0)
	|| ( strcmp( extention, "XIF") == 0)
	|| ( strcmp( extention, "XDD") == 0)
	|| ( strcmp( extention, "XFS") == 0))
	{
		entry->icon			= ICON_SYS;
		entry->mini_icon	= ICON_SYS_MINI;
		return;
	}

	if(( strcmp( extention, "MOV") == 0)
	|| ( strcmp( extention, "MPEG") == 0)
	|| ( strcmp( extention, "AVI") == 0)
	|| ( strcmp( extention, "MP4") == 0)
	|| ( strcmp( extention, "M2V") == 0)
	|| ( strcmp( extention, "RM")  == 0)
	|| ( strcmp( extention, "MPG") == 0))
	{
		entry->icon			= ICON_VID;
		entry->mini_icon	= ICON_VID_MINI;
		return;
	}

	if(( strcmp( extention, "SLB") == 0)
	|| ( strcmp( extention, "LDG") == 0)
	|| ( strcmp( extention, "LIB") == 0)
	|| ( strcmp( extention, "A") == 0))
	{
		entry->icon			= ICON_LIB;
		entry->mini_icon	= ICON_LIB_MINI;
		return;
	}

	if( strcmp( extention, "O") == 0)
	{
		entry->icon			= ICON_OBJECT;
		entry->mini_icon	= ICON_OBJECT_MINI;
		return;
	}

	if(( strcmp( extention, "ASM") == 0)
	|| ( strcmp( extention, "S") == 0))
	{
		entry->icon			= ICON_ASM;
		entry->mini_icon	= ICON_ASM_MINI;
		return;
	}

	if( strcmp( extention, "C") == 0)
	{
		entry->icon			= ICON_C;
		entry->mini_icon	= ICON_C_MINI;
		return;
	}

	if( strcmp( extention, "H") == 0)
	{
		entry->icon			= ICON_HEADER;
		entry->mini_icon	= ICON_HEADER_MINI;
		return;
	}

	if(( strcmp( extention, "CPP") == 0)
	|| ( strcmp( extention, "CC")  == 0))
	{
		entry->icon			= ICON_CPP;
		entry->mini_icon	= ICON_CPP_MINI;
		return;
	}

	if(( strcmp( extention, "HTM")  == 0)
	|| ( strcmp( extention, "HTML") == 0))
	{
		entry->icon			= ICON_HTML;
		entry->mini_icon	= ICON_HTML_MINI;
		return;
	}

	if(( strcmp( extention, "RTF") == 0)
	|| ( strcmp( extention, "PAP") == 0))
	{
		entry->icon			= ICON_RTF;
		entry->mini_icon	= ICON_RTF_MINI;
		return;
	}

	if(( strcmp( extention, "FNT") == 0)
	|| ( strcmp( extention, "SPD") == 0)
	|| ( strcmp( extention, "TTF") == 0))
	{
		entry->icon			= ICON_FNT;
		entry->mini_icon	= ICON_FNT_MINI;
		return;
	}

	if(( strcmp( extention, "ZIP") == 0)
	|| ( strcmp( extention, "RAR") == 0)
	|| ( strcmp( extention, "TGZ") == 0)
	|| ( strcmp( extention, "LZH") == 0)
	|| ( strcmp( extention, "ZOO") == 0)
	|| ( strcmp( extention, "TAR") == 0)
	|| ( strcmp( extention, "GZ")  == 0))
	{
		entry->icon			= ICON_ARCHIVE;
		entry->mini_icon	= ICON_ARCHIVE_MINI;
		return;
	}

	if(( strcmp( extention, "MP3") == 0)
	|| ( strcmp( extention, "MP2") == 0)
	|| ( strcmp( extention, "WAV") == 0)
	|| ( strcmp( extention, "VOB") == 0)
	|| ( strcmp( extention, "AVR") == 0)
	|| ( strcmp( extention, "RA")  == 0)
	|| ( strcmp( extention, "DVS") == 0)
	|| ( strcmp( extention, "AC3") == 0)
	|| ( strcmp( extention, "OGG") == 0)
	|| ( strcmp( extention, "AIF") == 0))
	{
		entry->icon			= ICON_SOUND;
		entry->mini_icon	= ICON_SOUND_MINI;
		return;
	}

	if( strcmp( extention, "CPX") == 0)
	{
		entry->icon			= ICON_CPX;
		entry->mini_icon	= ICON_CPX_MINI;
		return;
	}

	if( strcmp( extention, "PDF") == 0)
	{
		entry->icon			= ICON_PDF;
		entry->mini_icon	= ICON_PDF_MINI;
		return;
	}

	if( strcmp( extention, "PAL") == 0)
	{
		entry->icon			= ICON_PALETTE;
		entry->mini_icon	= ICON_PALETTE_MINI;
		return;
	}

	for( i = 0; i < app->codecs.codecs_nbr; i++)
	{
		plugin_name = app->codecs.codec_ptr[i]->infos;

		for( j = 0; j < app->codecs.codec_ptr[i]->user_ext; j++)
		{
			if( strncmp( extention, plugin_name, 3) == 0)
			{
				entry->icon			= ICON_IMG;
				entry->mini_icon	= ICON_IMG_MINI;
				return;
			}

			plugin_name += 3;
		}
	}
}

