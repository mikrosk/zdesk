#ifndef PIXEL_H
#define PIXEL_H

#ifdef __cplusplus
extern "C" {
#endif

void getPixel_32( int x, int y, uint32 line_size, uint8 *red, uint8 *green, uint8 *blue, uint8 *src);
void setPixel_32( int x, int y, uint32 line_size, uint8 red, uint8 green, uint8 blue, uint8 *dst);
void getPixel_32r( int x, int y, uint32 line_size, uint8 *red, uint8 *green, uint8 *blue, uint8 *src);
void setPixel_32r( int x, int y, uint32 line_size, uint8 red, uint8 green, uint8 blue, uint8 *dst);
void getPixel_32z( int x, int y, uint32 line_size, uint8 *red, uint8 *green, uint8 *blue, uint8 *src);
void setPixel_32z( int x, int y, uint32 line_size, uint8 red, uint8 green, uint8 blue, uint8 *dst);
void getPixel_24( int x, int y, uint32 line_size, uint8 *red, uint8 *green, uint8 *blue, uint8 *src);
void setPixel_24( int x, int y, uint32 line_size, uint8 red, uint8 green, uint8 blue, uint8 *dst);
void getPixel_24r( int x, int y, uint32 line_size, uint8 *red, uint8 *green, uint8 *blue, uint8 *src);
void setPixel_24r( int x, int y, uint32 line_size, uint8 red, uint8 green, uint8 blue, uint8 *dst);
void getPixel_16( int x, int y, uint32 line_size, uint8 *red, uint8 *green, uint8 *blue, uint8 *src);
void setPixel_16( int x, int y, uint32 line_size, uint8 red, uint8 green, uint8 blue, uint8 *dst);
void getPixel_16r( int x, int y, uint32 line_size, uint8 *red, uint8 *green, uint8 *blue, uint8 *src);
void setPixel_16r( int x, int y, uint32 line_size, uint8 red, uint8 green, uint8 blue, uint8 *dst);

#ifdef __cplusplus
}
#endif

#endif

