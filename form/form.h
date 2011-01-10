#ifndef FORM_H
#define FORM_H

#ifdef __cplusplus
extern "C" {
#endif

void show_alert( int8 *txt, int32 error, APPVar *app);
void open_about_box( APPVar *app);
void open_info_box( WINDOW *winfile, APPVar *app);

#ifdef __cplusplus
}
#endif

#endif
