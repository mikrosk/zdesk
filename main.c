#include "general.h"
#include "appl.h"
#include "evnt_loop.h"

/*==================================================================================*
 * int main:																		*
 *		The main function:															*
 *----------------------------------------------------------------------------------*
 * input:																			*
 *		--																			*
 *----------------------------------------------------------------------------------*
 * returns: 																		*
 *      EXIT_FAILURE if error else EXIT_SUCCESS.									*
 *==================================================================================*/

int main( void)
{
	APPVar *app;

	app = ApplInit();

	if( app == NULL)
		return EXIT_FAILURE;

	EvntLoop( app);

	return EXIT_SUCCESS;
}
