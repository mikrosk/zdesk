#ifndef EVENT_H
#define EVENT_H

#ifdef __cplusplus
extern "C" {
#endif
	
WINDOW *WindHandle( APPVar *app, int16 handle);
void EvntLoop( APPVar *app);

#ifdef __cplusplus
}
#endif

#endif

