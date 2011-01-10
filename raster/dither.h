#ifndef DITHER_H
#define DITHER_H

#ifdef __cplusplus
extern "C" {
#endif

inline uint8 ditherize_32 ( uint8 *rgb, int16 *err, int8 **buf, APPVar *app);
inline uint8 dither_true ( uint8 *rgb, int16 *err, int8 **buf, APPVar *app);
inline uint8 dither_gray( uint8 *gray, int16 *err, int8 **buf, APPVar *app);

#ifdef __cplusplus
}
#endif

#endif


