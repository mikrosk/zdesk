#ifndef MENU_H
#define MENU_H

#ifdef __cplusplus
extern "C" {
#endif

void set_menu_status( WINDOW *win, APPVar *app);
void open_new_tab( WINDOW *win, APPVar *app);	
void close_tab( WINDOW *win, TAB *tab, APPVar *app);
void show_as_list( WINDOW *win, APPVar *app);
void show_as_icon( WINDOW *win, APPVar *app);
void show_infobar( WINDOW *win, APPVar *app);
void show_tabbar( WINDOW *win, APPVar *app);

	
#ifdef __cplusplus
}
#endif

#endif