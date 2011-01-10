#include "general.h"

/*==================================================================================*
 * void zdebug:																		*
 *		print debug information.													*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		...	-> 	text and parameter are the same that standard "printf" function.	*
 *----------------------------------------------------------------------------------*
 * return:	 																		*	
 *		--																			*
 *==================================================================================*/
void zdebug( const char *format, ...) 
{
#ifdef DEBUG
	#include <stdarg.h>

	va_list args;
	char fo_buff[255], *p;
	
	va_start( args, format);
	vsprintf( fo_buff, format, args);
	p = fo_buff;
	va_end( args);

	v_gtext( 1, 0, 758, p);
#endif	
}



