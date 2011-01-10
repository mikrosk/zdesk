#ifndef RASTER_H
#define RASTER_H

#ifdef __cplusplus
extern "C" {
#endif


void raster_mono ( dec_data * info, void * _dst, APPVar *app);
void raster_D1 ( dec_data * info, void * _dst, APPVar *app);
void dither_D1 ( dec_data * info, void * _dst, APPVar *app);
void gscale_D1 ( dec_data * info, void * _dst, APPVar *app);

void raster_D2 ( dec_data * info, void * _dst, APPVar *app);
void dither_D2 ( dec_data * info, void * _dst, APPVar *app);
void gscale_D2 ( dec_data * info, void * _dst, APPVar *app);

void raster_I4 ( dec_data * info, void * _dst, APPVar *app);
void dither_I4 ( dec_data * info, void * _dst, APPVar *app);
void gscale_I4 ( dec_data * info, void * _dst, APPVar *app);

void raster_I8 ( dec_data * info, void * _dst, APPVar *app);
void dither_I8 ( dec_data * info, void * _dst, APPVar *app);
void gscale_I8 ( dec_data * info, void * _dst, APPVar *app);

void raster_P8 ( dec_data * info, void * _dst, APPVar *app);
void dither_P8 ( dec_data * info, void * _dst, APPVar *app);
void gscale_P8 ( dec_data * info, void * _dst, APPVar *app);

void raster_15 ( dec_data * info, void * _dst, APPVar *app);
void dither_15 ( dec_data * info, void * _dst, APPVar *app);
void gscale_15 ( dec_data * info, void * _dst, APPVar *app);

void raster_15r ( dec_data * info, void * _dst, APPVar *app);
void dither_15r ( dec_data * info, void * _dst, APPVar *app);
void gscale_15r ( dec_data * info, void * _dst, APPVar *app);

void raster_16 ( dec_data * info, void * _dst, APPVar *app);
void dither_16 ( dec_data * info, void * _dst, APPVar *app);
void gscale_16 ( dec_data * info, void * _dst, APPVar *app);

void raster_16r ( dec_data * info, void * _dst, APPVar *app);
void dither_16r ( dec_data * info, void * _dst, APPVar *app);
void gscale_16r ( dec_data * info, void * _dst, APPVar *app);

void raster_24 ( dec_data * info, void * _dst, APPVar *app);
void dither_24 ( dec_data * info, void * _dst, APPVar *app);
void gscale_24 ( dec_data * info, void * _dst, APPVar *app);

void raster_24r ( dec_data * info, void * _dst, APPVar *app);
void dither_24r ( dec_data * info, void * _dst, APPVar *app);
void gscale_24r ( dec_data * info, void * _dst, APPVar *app);

void raster_32 ( dec_data * info, void * _dst, APPVar *app);
void dither_32 ( dec_data * info, void * _dst, APPVar *app);
void gscale_32 ( dec_data * info, void * _dst, APPVar *app);

void raster_32r ( dec_data * info, void * _dst, APPVar *app);
void dither_32r ( dec_data * info, void * _dst, APPVar *app);
void gscale_32r ( dec_data * info, void * _dst, APPVar *app);

void raster_32z ( dec_data * info, void * _dst, APPVar *app);
void dither_32z ( dec_data * info, void * _dst, APPVar *app);
void gscale_32z ( dec_data * info, void * _dst, APPVar *app);

int16 smooth_resize( MFDB *source_image, MFDB *resized_image, APPVar *appl);

#ifdef __cplusplus
}
#endif

#endif

