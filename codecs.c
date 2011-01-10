#include "general.h"
#include "codecs.h"
#include <mint/cookie.h>



/*==================================================================================*
 * void codecs_quit:																*
 *		unload the codec and mem.ldg.												*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		app			-> Application's descriptor.									*
 *----------------------------------------------------------------------------------*
 * returns: 																		*
 *      --																			*
 *==================================================================================*/

void codecs_quit( APPVar *app)
{
	int i;

	for( i = 0; i < app->codecs.codecs_nbr; i++)
		ldg_close( app->codecs.codec_ptr[i], app->aes_global);
	
	if( app->ldg_mem.ldg)
		ldg_close( app->ldg_mem.ldg, app->aes_global);	
}



/*==================================================================================*
 * int16 codecs_init:																*
 *		load the codecs and mem.ldg													*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		app			-> Application's descriptor.									*
 *----------------------------------------------------------------------------------*
 * returns: 																		*
 *      0 if the codec are found( or available)	else -1.							*
 *==================================================================================*/

int32 codecs_init( APPVar *app)
{
	struct dbuf 
	{
		int32 ino;
		char name[MAXNAMLEN + 1];
	} dbuf;	
	
	char 	*extention_ptr;
	int16	codecs_dir_exist = FALSE;
	int32	dir_handle;
	
	app->codecs.codecs_nbr = 0;
	app->ldg_mem.ldg = NULL;
	
	/* we search in the current directory */
	app->ldg_mem.ldg = ldg_open( "mem.ldg", app->aes_global);		
		
	if( app->ldg_mem.ldg)
	{
		if( Dsetpath( "codecs") == 0)
			codecs_dir_exist = TRUE;
	}

	/* we search in the default global directory */
	if( codecs_dir_exist == FALSE)
	{	
		if( app->ldg_mem.ldg == NULL)
			app->ldg_mem.ldg = ldg_open( "C:\\gemsys\\ldg\\mem.ldg", app->aes_global);
		
		if( app->ldg_mem.ldg)
		{	
			Dsetdrv( 2);

			if( Dsetpath( "\\gemsys\\ldg\\codecs") == 0)
				codecs_dir_exist = TRUE;
		}	
	}		

	if( app->ldg_mem.ldg == NULL)
		return -1;
		
	if( codecs_dir_exist == FALSE)
	{	
		ldg_close( app->ldg_mem.ldg, app->aes_global);
		return -1;	
	}			

	app->ldg_mem.ldg_malloc 	= ldg_find( "ldg_malloc", app->ldg_mem.ldg);
	app->ldg_mem.ldg_realloc	= ldg_find( "ldg_realloc", app->ldg_mem.ldg);
	app->ldg_mem.ldg_free 		= ldg_find( "ldg_free", app->ldg_mem.ldg);	
	app->ldg_mem.ldg_allocmode	= ldg_find( "ldg_allocmode", app->ldg_mem.ldg);
	
	if( !app->ldg_mem.ldg_malloc || !app->ldg_mem.ldg_realloc || !app->ldg_mem.ldg_free || !app->ldg_mem.ldg_allocmode)
	{
		ldg_close( app->ldg_mem.ldg, app->aes_global);
		app->ldg_mem.ldg = NULL;
		return -1;
	}
	
	app->ldg_mem.ldg_allocmode( 0L, ( int32)app->aes_global[2]);	
	
	
	dir_handle = Dopendir( ".", 0);

	if(( dir_handle & 0xFF000000L) == 0xFF000000L)
		goto abort;

	while(( Dreaddir((int)( MAXNAMLEN + 1 + sizeof( int32)), dir_handle, (char *)&dbuf) == 0) && app->codecs.codecs_nbr < 49)
	{
		if(( strcmp( dbuf.name, ".") == 0) || ( strcmp( dbuf.name, "..") == 0))
			continue;

		extention_ptr = strrchr( dbuf.name + 1, ( int)'.');
		
		if( extention_ptr == NULL)
			continue;
		
		extention_ptr++;
		
		if( extention_ptr[0] == '\0')
			continue;		

		if( strcasecmp( extention_ptr, "LDG") == 0)
		{
			if(( app->codecs.codec_ptr[app->codecs.codecs_nbr] = ldg_open( dbuf.name, app->aes_global)))
			{
				if(( app->codecs.codec_init = ldg_find( "plugin_init", app->codecs.codec_ptr[app->codecs.codecs_nbr])))
				{
					app->codecs.codec_init();
					app->codecs.codecs_nbr++;
				}
				else
					ldg_close( app->codecs.codec_ptr[app->codecs.codecs_nbr], app->aes_global);
			}
		}
	}

	abort:

	Dclosedir( dir_handle);

	if( app->codecs.codecs_nbr > 0)
		return 0L;

	ldg_close( app->ldg_mem.ldg, app->aes_global);
	return -1;
}


