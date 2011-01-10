#ifndef MISC_H
#define MISC_H

#ifdef __cplusplus
extern "C" {
#endif

void DummyEvnt( WINDOW *win, APPVar *app);
inline void ClipOn( APPVar *app, GRECT *r);
int16 ApplWrite( APPVar *app, int16 to, int16 msg, int16 b3, int16 b4, int16 b5, int16 b6, int16 b7);
void redraw_area( WINDOW *win, int16 window_x, int16 window_y, GRECT *area, APPVar *app);
int16 suitable_mouse_form( APPVar *app);
	
#ifdef __cplusplus
}
#endif

#endif


