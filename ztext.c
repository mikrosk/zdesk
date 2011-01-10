#include "general.h"

extern int16 ofwf[256];


/*==================================================================================*
 * zstrncpy:																		*
 *----------------------------------------------------------------------------------*
 * 		Copy <n> bytes from <src> to <dst>, including the terminating \0'-byte.		* 
 *		If <src> is longer than <n-1>, it will be truncated with a '\0'-byte at		* 
 * 		position n-1.																* 
 *		Example: zstrncpy(b, "string", 1) would put a '\0'-byte at b[0] and 		*
 *		return. 																	*
 *		Basically, think of the <n> as a limiter on the number of bytes at <dst> 	*
 *		we're allowed to touch, and	then fit a string-copying idea on top of that.	*
 *		Works for me, and a	lot better than the standard strncpy(), too.			*
 *----------------------------------------------------------------------------------*
 * returns: <dst>																	*
 *----------------------------------------------------------------------------------*
 * 	dst:			Pointer to destination text.									*
 * 	src:			Pointer to source text.											*
 *	n:				Limiter on the number of bytes at <dst>							*
 *==================================================================================*/

char *zstrncpy( char *dst, const char *src, size_t n)
{
	register const char *source 	= src;
	register char *dest			 	= dst;
	register size_t count		 	= n;

	if( dest && !source && count)
		*dest = '\0';
	else if( dest && source && (count >= 1))
	{
		for( count--; count > 0 && *source != '\0'; count--)
			*dest++ = *source++;
		*dest = '\0';
	}
	return dst;
}



boolean dir_parent( char *dir)
{
	int len = strlen( dir) - 1;

	if( len <= 3)
	   return FALSE;        

	if( dir[len] == '\\')
		dir[len--] = '\0';

	while( len && ( dir[len] != '\\'))
		dir[len--] = '\0';

	if( dir[len] == '\\')
		return TRUE;

	return FALSE;   	
}



/*==================================================================================*
 * void get_directory_name:															*
 *		find the current directory name from a full path name.						*
 *----------------------------------------------------------------------------------*
 * 	fullpathname:	The full path name.												*
 * 	current_dir:	The current directory name will be wrote in this buffer.		*
 * 	buffer_size:	The current directory name size in bytes.						* 
 *----------------------------------------------------------------------------------*
 * return: -																		* 
 *==================================================================================*/

void get_directory_name( char *fullpathname, char *current_dir, size_t buffer_size)
{
	int len, i;

	i = len = strlen( fullpathname) - 1;

	if( len < 3)
	{
		zstrncpy( current_dir, ( const char *)fullpathname, 3);
		return;
	}

	if( fullpathname[len] == '\\')
		len--;

	while(( fullpathname[len-1] != '\\') && len)
		len--;

	zstrncpy( current_dir, ( const char *)&fullpathname[len], buffer_size);

	i -= len;

	if( current_dir[i] == '\\')
		current_dir[i] = '\0';
}



/*==================================================================================*
 * int get_directory_name:															*
 *		Check if a directory is in a full path.										*
 *----------------------------------------------------------------------------------*
 * 	path_to_dir:	The path to the directory.										*
 * 	dir:			The directory.													*
 * 	fullpath:		The full path.													* 
 *----------------------------------------------------------------------------------*
 * return: -1 is the dir is not in the full path else dir the positon in the path.	* 
 *==================================================================================*/
/*
int	dir_is_in_fullpath( char *path_to_dir, char *dir, char *fullpath)
{
	register char *c1 = fullpath, *c2 = path_to_dir;
	register int dir_position = 0;
	
	while( *c1 != '\0' && *c1 == *c2)
	{
		c1++; 
		c2++;
		
		if( *c1 == '\\')
			dir_position++;
	}
	
	if(( *c2 != '\0') || ( *c1 != dir[0]))
		return -1;

	c2 = dir;
	
	while ( *c1 != '\0' && *c1 == *c2)
	{
		c1++; 
		c2++;
	}

	if(( *c2 == '\0') && ( *c1 == '\\'))
		return dir_position;	
	
	return -1;
}
*/




/*==================================================================================*
 * void unix2dos:																	*
 *		Convert path from Unix to DOS( if necessary).								*
 *----------------------------------------------------------------------------------*
 * input:																			*
 * 		p					-> the path to convert.									*
 *----------------------------------------------------------------------------------*
 * return: --																		*
 *==================================================================================*/

void unix2dos( char *p)
{
	char *q;
	
	if( p[0] == '/')
	{
		if( p[2] == '/')
		{
			p[0] = p[1];
			p[1] = ':';
		} 
		else 
		{
			q = p;
			
			while( *q++);
			
			while( q >= p) 
			{
				q[2] = q[0];
				q--;
			}
			p[0] = 'U';
			p[1] = ':';
		}
	}
	
	q = p;
	
	p[0] = toupper( p[0]);
	
	while( *q) 
	{
		if( *q == '/') 
			*q = '\\';
		
		q++;
	}
}


/*==================================================================================*
 * byte_to_text:																	*
 *		convert <size> to a Human readable string.									*
 *----------------------------------------------------------------------------------*
 * 	txt:			We write the text here.											*
 * 	size:			File size in byte.												*
 *----------------------------------------------------------------------------------*
 * return: -																		* 
 *==================================================================================*/

void byte_to_text( char *txt, double size)
{
	if( size < 1024.0)
		sprintf( txt, "%.0lf bytes", size);
	else if( size < 1048576.0)
	{
		size = size / 1024.0;
		sprintf( txt, "%.1lf KB", size);
	}
	else if( size < 1073741824.0)
	{
		size = size / 1048576.0;
		sprintf( txt, "%.1lf MB", size);
	}
	else
	{
		size = size / 1073741824.0;
		sprintf( txt, "%.1lf GB", size);
	}
}


/*==================================================================================*
 * dostime_to_text:																	*
 *		convert <dostime> to a Human readable string.								*
 *----------------------------------------------------------------------------------*
 * returns: -																		*
 *----------------------------------------------------------------------------------*
 * 	txt:			We write the text here.											*
 * 	file_time:		file time in DOS format.										*
 * 	current_date:	Current date in DOS format.										* 
 *==================================================================================*/
void dostime_to_text( char *txt, uint32 *file_time, uint16 current_date, APPVar *app)
{
	_DATETIME *ftime = (_DATETIME *)file_time;
	static char *month[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	int16 current_day, current_month, current_year;
	
	current_day = current_date & 31;
	current_month = ( current_date >> 5) & 15;
	current_year = ( current_date >> 9) & 127;
	
	// TODO: yesterday for the 31-10-2010 if we are the 01-11-2010
	
	if(( ftime->day == current_day) && ( ftime->month == current_month) && ( ftime->year == current_year))
		sprintf( txt, "%s, %.2d:%.2d", app->file_today, ftime->hour, ftime->minute);
	else if(( ftime->day == current_day - 1) && ( ftime->month == current_month) && ( ftime->year == current_year))
		sprintf( txt, "%s, %.2d:%.2d", app->file_yesterday, ftime->hour, ftime->minute);
	else
		sprintf( txt, "%d %s %d, %.2d:%.2d", ftime->day, month[( ftime->month & 0x0f) - 1], 1980 + ftime->year, ftime->hour, ftime->minute);
}


int16 get_text_width( const char *str)
{
	int16 ch, text_width = 0;
	
	while(( ch = *str++) != 0)
	{	
		if( ch < 0)
			ch += 256;
		
		text_width += ofwf[ch];
	}
	
	return text_width;
}



int16 name_shorter( int16 max_size, char *str)
{
	int16 len, current_len;
	len = ( int16)strlen( str);
	
	if( len < 5)
		return get_text_width(( const char *)str);
	
	do
	{
		str[len - 3] = '.';
		str[len - 2] = '.';
		str[len - 1] = '.';
		str[len]	 = '\0';
		
		len--;
		
		current_len = get_text_width(( const char *)str);
		
	} while(( current_len > max_size) && ( len > 4));
	
	return current_len;
}


