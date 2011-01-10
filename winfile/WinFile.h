#ifndef WINFILE_H
#define WINFILE_H

#ifdef __cplusplus
extern "C" {
#endif

/* WinFile_New.c */
void WinFile_New( char *current_fullpath_dir, char *new_dir, int16 x, int16 y, int16 w, int16 h, APPVar *app);

	
/* WinFile_Tab.c */
TAB *WinFile_NewTab( WINDOW *win, char *current_fullpath_dir, char *new_dir, int16 tabbar_width, APPVar *app);
void WinFile_Select_Tab( WINDOW *win, TAB *tab, APPVar *app);
void WinFile_Close_Tab( WINDOW *win, TAB *tab, APPVar *app);
void WinFile_Tab_Drag_and_Drop( WINDOW *win, TAB *tab, int16 mousex, int16 mousey, GRECT *file_area, APPVar *app);

	
/* WinFile_Tabbar.c */
inline void draw_tabbar( WINDOW *win, GRECT *tabbar_area, GRECT *clip, APPVar *app);
void redraw_tabbar( WINDOW *win, GRECT *file_area, APPVar *app);
void update_tabs( WINDOW *win, int16 tabbar_area_width, APPVar *app);

	
/* WinFile_Update_Icon_Position.c */
void WinFile_Update_Icon_Position( WINDOW *win, GRECT *rect, APPVar *app);


/* WinFile_Redraw.c */
void WinFile_Redraw( WINDOW *win, APPVar *app);


/* WinFile_Infobar.c */
inline void draw_infobar( WINDOW *win, GRECT *info_rect, GRECT *clip, APPVar *app);
void update_infobar( WINDOW *win, GRECT *rect, APPVar *app);
void redraw_infobar( WINDOW *win, GRECT *rect, APPVar *app);


/* WinFile_Keyb.c */
void WinFile_Keyb( WINDOW *win, APPVar *app);


/* WinFile_Sizer.c */
void WinFile_Sizer( WINDOW *win, APPVar *app);


/* WinFile_Close.c */
void WinFile_Close( WINDOW *win, APPVar *app);


/* WinFile_Other_Event.c */
void WinFile_Bottom( WINDOW *win, APPVar *app);
void WinFile_Top( WINDOW *win, APPVar *app);
void WinFile_Move( WINDOW *win, APPVar *app);
void WinFile_Full( WINDOW *win, APPVar *app);


/* WinFile_Mouse.c */
void WinFile_Mouse( WINDOW *win, APPVar *app);


/* WinFile_Iconify.c */
void WinFile_Iconify( WINDOW *win, APPVar *app);
void WinFile_Uniconify( WINDOW *win, APPVar *app);
void WinFile_Alliconify( WINDOW *win, APPVar *app);
void WinFile_Iconify_Redraw( WINDOW *win, APPVar *app);


/* WinFile_Slider.c */
void WinFile_Slider( WINDOW *win, int32 win_height, APPVar *app);
void WinFile_Vslide( WINDOW *win, APPVar *app);
void WinFile_Arrow( WINDOW *win, APPVar *app);


/* WinFile_Entry.c */
void add_selected_entry( TAB *tab, Entry *entry);
void remove_selected_entry( TAB *tab, Entry *entry);
inline boolean check_selected_entry( TAB *tab, Entry *entry);
void remove_all_selected_entry( TAB *tab);
int16 return_entry_nbr( TAB *tab, Entry *entry);


/* WinFile_Exec_Entry.c */
void WinFile_Exec_Entry( WINDOW *win, GRECT *win_area, GRECT *file_area, APPVar *app);


/* WinFile_Popup.c */
/* void WinFile_Popup( WINDOW *win, WINDICON *wicones); */


/* WinFile_Rubberbox.c */
void WinFile_Rubberbox( WINDOW *win, GRECT *file_area, GRECT *win_area, APPVar *app);

	
/* WinFile_Frame_Sizer.c */	
void WinFile_Column_Sizer( WINDOW *win, int16 mousex, int16 mousey, GRECT *working_area, APPVar *app);

	
/* WinFile_Calc_Areas.c */	
void WinFile_Calc_Areas( WINDOW *win, GRECT *full_area, GRECT *file_area, APPVar *app);	
	

/* WinFile_Popup.c */
void open_tab_popup( WINDOW *winfile, int16 x, int16 y, APPVar *app);
	
	
#ifdef __cplusplus
}
#endif

#endif



