#include "general.h"
#include "file/file.h"


/*==================================================================================*
 * rename_entry_file:																*
 *		rename a file or directory. 												*
 *----------------------------------------------------------------------------------*
 * 	win			-> Pointer to the target tab.										*
 * 	new_name	-> new entry's name.												*
 * 	app			-> Application's descriptor.										*
 *----------------------------------------------------------------------------------*
 *	return: 	0 if success or GEMDOS error code.									*
 *==================================================================================*/

int32 rename_entry_file( TAB *tab, char *new_name, APPVar *app)
{
	int32 rval;

	rval = Frename( 0, tab->first_selected->name, new_name);

	if( rval != 0)
		return rval;

	return 0L;
}

