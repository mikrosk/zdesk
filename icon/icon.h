#ifndef ICON_H
#define ICON_H

#ifdef __cplusplus
extern "C" {
#endif

void CDECL draw_cicon( int16 icon, APPVar *app);
void set_icon( char *name, Entry *entry, APPVar *app);
void CDECL draw_icon( WINDOW *win, Entry *entry, boolean selected, int16 x, int16 y, APPVar *app);


#ifdef __cplusplus
}
#endif

#endif



