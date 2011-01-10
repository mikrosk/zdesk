#ifndef COLOR_H
#define COLOR_H

#ifdef __cplusplus
extern "C" {
#endif

void 	save_colors( APPVar *app);
int16 	remap_color( int32 value, APPVar *app);
uint32 	color_lookup( uint32 rgb, int16 *trans, APPVar *app);
void 	cnvpal_mono( img_info *info, dec_data * data, APPVar *app);
void 	cnvpal_1_2( img_info *info, dec_data * data, APPVar *app);
void 	cnvpal_4_8( img_info *info, dec_data * data, APPVar *app);
void 	cnvpal_15( img_info *info, dec_data * data, APPVar *app);
void 	cnvpal_high( img_info *info, dec_data * data, APPVar *app);
void 	cnvpal_true( img_info *info, dec_data * data, APPVar *app);


#ifdef __cplusplus
}
#endif

#endif

