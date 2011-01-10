#ifndef PREF_H
#define PREF_H

#ifdef __cplusplus
extern "C" {
#endif

void prefs_read( PREFERENCE *pref, APPVar *appl);
void prefs_write( PREFERENCE *pref, APPVar *appl);

#ifdef __cplusplus
}
#endif

#endif
