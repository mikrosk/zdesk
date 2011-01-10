#ifndef DESKTOP_H
#define DESKTOP_H

#ifdef __cplusplus
extern "C" {
#endif

WINDOW *Desktop_Init( APPVar *app);
int32 scan_desktop( TAB *tab, APPVar *app);

#ifdef __cplusplus
}
#endif

#endif




