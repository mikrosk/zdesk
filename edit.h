#ifndef EDIT_H
#define EDIT_H

#ifdef __cplusplus
extern "C" {
#endif

int curs_left( EDIT *edit);
int curs_right( EDIT *edit);
int char_del( EDIT *edit);
int char_put( EDIT *edit, int16 c);
int16 cursor_position( WINDOW *win);
void redraw_icon_txt( WINDOW *win, Entry *entry, int16 window_x, int16 window_y, APPVar *app);
void init_edit_mode( WINDOW *win, Entry *entry, int16 window_x, int16 window_y, APPVar *app);
void exit_edit_mode( WINDOW *win, Entry *entry, int16 window_x, int16 window_y, APPVar *app);

#ifdef __cplusplus
}
#endif

#endif



