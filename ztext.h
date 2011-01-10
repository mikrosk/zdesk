#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#ifdef __cplusplus
extern "C" {
#endif

char *zstrncpy( char *dst, const char *src, size_t n);
void byte_to_text( char *txt, double size);
void dostime_to_text( char *txt, uint32 *file_time, uint16 current_date, APPVar *app);
boolean dir_parent( char *dir);
void get_directory_name( char *fullpathname, char *dir, size_t buffer_size);
void unix2dos( char *p);
int16 get_text_width( const char *str);
int16 name_shorter( int16 max_size, char *str);


#ifdef __cplusplus
}
#endif

#endif